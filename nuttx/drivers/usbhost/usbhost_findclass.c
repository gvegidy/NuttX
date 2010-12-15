/****************************************************************************
 * drivers/usbhost/usbhost_findclass.c
 *
 *   Copyright (C) 2010 Gregory Nutt. All rights reserved.
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
#include <stdbool.h>

#include <nuttx/usb/usb.h>
#include <nuttx/usb/usbhost.h>
#include <arch/irq.h>

#include "usbhost_registry.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: usbhost_idmatch
 *
 * Description:
 *   Check if the class ID matches what the host controller found.
 *
 * Input Parameters:
 *   classid - ID info for the class under consideration.
 *   devid - ID info reported by the device.
 *
 * Returned Values:
 *   TRUE - the class will support this device.
 *
 ****************************************************************************/

static bool usbhost_ismatch(const struct usbhost_id_s *classid,
                            const struct usbhost_id_s *devid)
{
  /* The base class ID, subclass and protocol have to match up in any event */

  if (devid->base     == classid->base &&
      devid->subclass == classid->subclass &&
      devid->proto    == clsssid->proto)
    {
      /* If this is a vendor-specific class ID, then the VID and PID have to
       * match as well.
       */

      if (devid->base == USB_CLASS_VENDOR_SPEC)
        {
          /* Vendor specific... do the VID and PID also match? */

          if (devid->vid == classid->vid && devid->pid == classid->pid)
            {
              /* Yes.. then we have a match */

              return true;
            }
        }
      else
        {
          /* Not vendor specific?  Then we have a match */

          return true;
        }
    }

  /* No match.. not supported */

  return false;
}
  
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: usbhost_findclass
 *
 * Description:
 *   Find a USB host class implementation previously registered by
 *   usbhost_registerclass().  On success, an instance of struct
 *   usbhost_registry_s will be returned.  That instance will contain all of
 *   the information that will be needed to obtain and bind a struct
 *   usbhost_class_s instance for the device.
 *
 * Input Parameters:
 *   id - Identifies the USB device class that has connect to the USB host.
 *
 * Returned Values:
 *   On success this function will return a non-NULL instance of struct
 *   usbhost_registry_s.  NULL will be returned on failure.  This function
 *   can only fail if (1) id is NULL, or (2) no USB host class is registered
 *   that matches the device class ID.
 *
 ****************************************************************************/

const struct usbhost_registry_s *usbhost_findclass(const struct usbhost_id_s *id)
{
  struct usbhost_registry_s *class;
  irqstate_t flags;
  int ndx;

  /* g_classregistry is a singly-linkedlist of class ID information added by
   * calls to usbhost_registerclass().  Since this list is accessed from USB
   * host controller interrupt handling logic, accesses to this list must be
   * protected by disabling interrupts.
   */

  flags = irqsave();

  /* Examine each register class in the linked list */

  for (class = g_classregistry; class; class = class->flink)
    {
      /* If the registered class supports more than one ID, subclass, or
       * protocol, then try each.
       */

      for (ndx = 0; ndx < class->nids; ndx++)
        {
          /* Did we find a matching ID? */

          if (usbhost_idmatch(&class->id[ndx], id))
            {
              /* Yes.. restore interrupts and return the class info */

              irqrestore(flags);
              return class;
            }
        }
    }

  /* Not found... restore interrupts and return NULL */

  irqrestore(flags);
  return NULL;
}

