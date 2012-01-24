/****************************************************************************
 * drivers/usbdev/composite.h
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

#ifndef __DRIVERS_USBDEV_COMPOSITE_H
#define __DRIVERS_USBDEV_COMPOSITE_H 1

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>

#include <nuttx/usb/usb.h>
#include <nuttx/usb/usbdev_trace.h>

#ifdef CONFIG_USBDEV_COMPOSITE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_USBDEV_TRACE_NRECORDS
#  define CONFIG_USBDEV_TRACE_NRECORDS 128
#endif

#ifndef CONFIG_USBDEV_TRACE_INITIALIDSET
#  define CONFIG_USBDEV_TRACE_INITIALIDSET 0
#endif

/* Packet sizes */

#ifndef CONFIG_COMPOSITE_EP0MAXPACKET
#  define CONFIG_COMPOSITE_EP0MAXPACKET 64
#endif

/* Vendor and product IDs and strings */

#ifndef CONFIG_COMPOSITE_COMPOSITE
#  ifndef CONFIG_COMPOSITE_VENDORID
#    warning "CONFIG_COMPOSITE_VENDORID not defined"
#    define CONFIG_COMPOSITE_VENDORID 0x584e
#  endif

#  ifndef CONFIG_COMPOSITE_PRODUCTID
#    warning "CONFIG_COMPOSITE_PRODUCTID not defined"
#    define CONFIG_COMPOSITE_PRODUCTID 0x5342
#  endif

#  ifndef CONFIG_COMPOSITE_VERSIONNO
#    define CONFIG_COMPOSITE_VERSIONNO (0x0399)
#  endif

#  ifndef CONFIG_COMPOSITE_VENDORSTR
#    warning "No Vendor string specified"
#    define CONFIG_COMPOSITE_VENDORSTR  "NuttX"
# endif

#  ifndef CONFIG_COMPOSITE_PRODUCTSTR
#    warning "No Product string specified"
#    define CONFIG_COMPOSITE_PRODUCTSTR "Composite Device"
#  endif

#  undef CONFIG_COMPOSITE_SERIALSTR
#  define CONFIG_COMPOSITE_SERIALSTR "0101"
#endif

#undef CONFIG_COMPOSITE_CONFIGSTR
#define CONFIG_COMPOSITE_CONFIGSTR "Composite"

/* Descriptors **************************************************************/

/* Descriptor strings */

#define COMPOSITE_MANUFACTURERSTRID   (1)
#define COMPOSITE_PRODUCTSTRID        (2)
#define COMPOSITE_SERIALSTRID         (3)
#define COMPOSITE_CONFIGSTRID         (4)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: usbstrg_mkstrdesc
 *
 * Description:
 *   Construct a string descriptor
 *
 ****************************************************************************/

int composite_mkstrdesc(uint8_t id, struct usb_strdesc_s *strdesc);

/****************************************************************************
 * Name: composite_getepdesc
 *
 * Description:
 *   Return a pointer to the composite device descriptor
 *
 ****************************************************************************/

#ifndef CONFIG_COMPOSITE_COMPOSITE
FAR const struct usb_devdesc_s *composite_getdevdesc(void);
#endif

/****************************************************************************
 * Name: composite_mkcfgdesc
 *
 * Description:
 *   Construct the composite configuration descriptor
 *
 ****************************************************************************/

#ifdef CONFIG_USBDEV_DUALSPEED
int16_t composite_mkcfgdesc(uint8_t *buf, uint8_t speed, uint8_t type);
#else
int16_t composite_mkcfgdesc(uint8_t *buf);
#endif

/****************************************************************************
 * Name: composite_getqualdesc
 *
 * Description:
 *   Return a pointer to the composite qual descriptor
 *
 ****************************************************************************/

#ifdef CONFIG_USBDEV_DUALSPEED
FAR const struct usb_qualdesc_s *composite_getqualdesc(void);
#endif

#endif /* CONFIG_USBDEV_COMPOSITE */
#endif /* __DRIVERS_USBDEV_COMPOSITE_H */
