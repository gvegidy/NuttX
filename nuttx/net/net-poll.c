/****************************************************************************
 * net/net-poll.c
 *
 *   Copyright (C) 2008 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#if defined(CONFIG_NET) && !defined(CONFIG_DISABLE_POLL)

#include <sys/types.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>

#include <net/uip/uip.h>
#include <nuttx/net.h>
#include <nuttx/arch.h>

#include <uip/uip-internal.h>

#include "net-internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Network polling can only be supported on TCP and only if read-ahead buffering
 * is enabled (it could be supported on UDP as will if it also had read-ahead
 * buffering.
 */

#if !defined(CONFIG_DISABLE_POLL) && CONFIG_NSOCKET_DESCRIPTORS > 0 && \
    defined(CONFIG_NET_TCP) && CONFIG_NET_NTCP_READAHEAD_BUFFERS > 0
#  define HAVE_NETPOLL 1
#else
#  undef HAVE_NETPOLL
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function: poll_interrupt
 *
 * Description:
 *   This function is called from the interrupt level to perform the actual
 *   TCP receive operation via by the uIP layer.
 *
 * Parameters:
 *   dev      The structure of the network driver that caused the interrupt
 *   conn     The connection structure associated with the socket
 *   flags    Set of events describing why the callback was invoked
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Running at the interrupt level
 *
 ****************************************************************************/

#ifdef HAVE_NETPOLL
static uint16 poll_interrupt(struct uip_driver_s *dev, FAR void *conn,
                             FAR void *pvprivate, uint16 flags)
{
  FAR struct pollfd *fds = (FAR struct pollfd *)pvprivate;

  nvdbg("flags: %04x\n", flags);

  /* 'private' might be null in some race conditions (?) */

  if (fds)
    {
      pollevent_t eventset = 0;

      /* Check for data availability events. */

      if ((flags & UIP_NEWDATA) != 0)
        {
          eventset |= POLLIN & fds->events;
        }

      /* A poll is a sign that we are free to send data. */

      if ((flags & UIP_POLL) != 0)
        {
          eventset |= POLLOUT & fds->events;
        }

      /* Check for a loss of connection events */

      if ((flags & (UIP_CLOSE|UIP_ABORT|UIP_TIMEDOUT)) != 0)
        {
          eventset |= (POLLERR|POLLHUP);
        }

      if (eventset)
        {
          fds->revents |= eventset;
          sem_post(fds->sem);
        }
    }
  return flags;
}
#endif /* HAVE_NETPOLL */

/****************************************************************************
 * Function: net_pollsetup
 *
 * Description:
 *   Setup to monitor events on one TCP/IP socket
 *
 * Input Parameters:
 *   conn  - The TCP/IP connection of interest
 *   fds   - The structure describing the events to be monitored, OR NULL if
 *           this is a request to stop monitoring events.
 *
 * Returned Value:
 *  0: Success; Negated errno on failure
 *
 ****************************************************************************/

#ifdef HAVE_NETPOLL
static inline int net_pollsetup(FAR struct socket *psock, struct pollfd *fds)
{
  FAR struct uip_conn *conn = psock->s_conn;
  FAR struct uip_callback_s *cb;
  irqstate_t flags;
  int ret;

  /* Sanity check */

#ifdef CONFIG_DEBUG
  if (!conn || !fds || psock->private)
    {
      ret = -EINVAL;
      goto errout;
    }
#endif

  /* Some of the  following must be atomic */

  flags = irqsave();

  /* Allocate a TCP/IP callback structure */

  cb = uip_tcpcallbackalloc(conn);
  if (!cb)
    {
      ret = -EBUSY;
      goto errout_with_irq;
    }

  /* Initialize the callbcack structure */

  cb->flags    = UIP_NEWDATA|UIP_POLL|UIP_CLOSE|UIP_ABORT|UIP_TIMEDOUT;
  cb->private  = (FAR void *)fds;
  cb->event    = poll_interrupt;

  /* Save the nps reference in the poll structure for use at teardown as well */

  psock->private = (FAR void *)cb;

  /* Check for read data availability now */

  if (!sq_empty(&conn->readahead))
    {
      fds->revents = fds->events & POLLIN;
      if (fds->revents != 0)
        {
          /* If data is available now, the signal the poll logic */

          sem_post(fds->sem);
        }
    }
  irqrestore(flags);
  return OK;

errout_with_irq:
  irqrestore(flags);
errout:
  return ret;
}
#endif /* HAVE_NETPOLL */

/****************************************************************************
 * Function: net_pollteardown
 *
 * Description:
 *   Teardown monitoring of events on an TCP/IP socket
 *
 * Input Parameters:
 *   conn  - The TCP/IP connection of interest
 *
 * Returned Value:
 *  0: Success; Negated errno on failure
 *
 ****************************************************************************/

#ifdef HAVE_NETPOLL
static inline int net_pollteardown(FAR struct socket *psock)
{
  FAR struct uip_conn *conn = psock->s_conn;
  FAR struct uip_callback_s *cb;
  irqstate_t flags;

  /* Sanity check */

#ifdef CONFIG_DEBUG
  if (!conn || !psock->private)
    {
      return -EINVAL;
    }
#endif

  /* Recover the socket descriptor poll state info from the poll structure */

  cb = (FAR struct uip_callback_s *)psock->private;
  if (cb)
    {
      /* Release the callback */

      flags = irqsave();
      uip_tcpcallbackfree(conn, cb);
      irqrestore(flags);

      /* Release the poll/select data slot */

      psock->private = NULL;
    }

  return OK;
}
#endif /* HAVE_NETPOLL */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: net_poll
 *
 * Description:
 *   The standard poll() operation redirects operations on socket descriptors
 *   to this function.
 *
 * Input Parameters:
 *   fd   - The socket descriptor of interest
 *   fds  - The structure describing the events to be monitored, OR NULL if
 *          this is a request to stop monitoring events.
 *
 * Returned Value:
 *  0: Success; Negated errno on failure
 *
 ****************************************************************************/

#ifndef CONFIG_DISABLE_POLL
int net_poll(int sockfd, struct pollfd *fds)
{
#ifndef HAVE_NETPOLL
  return -ENOSYS;
#else
  FAR struct socket *psock;
  int ret;

  /* Get the underlying socket structure and verify that the sockfd
   * corresponds to valid, allocated socket
   */

  psock = sockfd_socket(sockfd);
  if (!psock || psock->s_crefs <= 0)
    {
      ret = -EBADF;
      goto errout;
    }

#ifdef CONFIG_NET_UDP
  /* poll() not supported for UDP */

  if (psock->s_type != SOCK_STREAM)
    {
      ret = -ENOSYS;
      goto errout;
    }
#endif

  /* Check if we are setting up or tearing down the poll */
  if (fds)
    {
      /* Perform the TCP/IP poll() setup */

      ret = net_pollsetup(psock, fds);
    }
  else
    {
      /* Perform the TCP/IP poll() teardown */

      ret = net_pollteardown(psock);
    }

errout:
  return ret;
#endif /* HAVE_NETPOLL */
}
#endif /* !CONFIG_DISABLE_POLL */

#endif /* CONFIG_NET && !CONFIG_DISABLE_POLL */
