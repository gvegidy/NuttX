/****************************************************************************
 * apps/netutils/ftpc/ftpc_login.c
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
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

#include <string.h>
#include <errno.h>
#include <debug.h>

#include <apps/ftpc.h>

#include "ftpc_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ftpc_login
 *
 * Description:
 *   Log into the server
 *
 ****************************************************************************/

int ftpc_login(SESSION handle, FAR struct ftpc_login_s *login)
{
  FAR struct ftpc_session_s *session = (FAR struct ftpc_session_s *)handle;
  int err;
  int ret;

  /* Verify that we are connected to a server */

  if (!ftpc_connected(session))
    {
      ndbg("Not connected\n");
      err = ENOTCONN;
      goto errout_with_err;
    }

  /* Verify that we are not already logged in to the server */

  if (ftpc_loggedin(session))
    {
      ndbg("Already logged in\n");
      err = EINVAL;
      goto errout_with_err;
    }

  /* Save the login parameter */

  session->uname   = decode_rfc1738(login->uname);
  session->pwd     = decode_rfc1738(login->pwd);
  session->initdir = decode_rfc1738(login->rdir);

  /* Is passive mode requested? */

  FTPC_CLR_PASSIVE(session);
  if (login->pasv)
    {
      FTPC_SET_PASSIVE(session);
    }

  /* The (Re-)login to the server */

  ret = ftpc_relogin(session);
  if (ret != OK)
    {
      ndbg("login failed: %d\n", errno);
      goto errout;
    }

  return OK;

errout_with_err:
  set_errno(err);
errout:
  return ERROR;
}

/****************************************************************************
 * Name: ftpc_relogin
 *
 * Description:
 *   Log in again after a loss of connection
 *
 ****************************************************************************/

int ftpc_relogin(FAR struct ftpc_session_s *session)
{
  int err;
  int ret;

  /* Log into the server */

  FTPC_CLR_LOGGEDIN(session);
  ret = ftpc_cmd(session, "USER %s", session->uname);
  if (ret != OK)
    {
      ndbg("USER %s cmd failed: %d\n", session->uname, errno);
      return ERROR;
    }

  ret = ftpc_cmd(session, "PASS %s", session->pwd);
  if (ret != OK)
    {
      ndbg("PASS %s cmd failed: %d\n", session->pwd, errno);
      return ret;
    }

  FTPC_SET_LOGGEDIN(session);
  session->homedir = ftpc_pwd((SESSION)session);
  session->curdir  = strdup(session->homedir);
  session->prevdir = strdup(session->homedir);
  if (session->initdir)
    {
      ftpc_chdir((SESSION)session, session->initdir);
    }

  return OK;
}
