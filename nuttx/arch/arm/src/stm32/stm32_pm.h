/************************************************************************************
 * arch/arm/src/stm32/stm32_pm.h
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_STM32_STM32_PM_H
#define __ARCH_ARM_SRC_STM32_STM32_PM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <semaphore.h>

#include "chip.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

 #ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Before any power state transition is made, the caller should hold this
 * semaphore to assure that there are no concurrent, contradictory power
 * state activities.
 */

#ifdef CONFIG_PM
EXTERN sem_t g_pmsem;
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_pmstop
 *
 * Description:
 *   Enter STOP mode. 
 *
 * Input Parameters:
 *   lpds - true: To further reduce power consumption in Stop mode, put the
 *          internal voltage regulator in low-power mode using the LPDS bit
 *          of the Power control register (PWR_CR).
 *
 * Returned Value:
 *   Zero means that the STOP was successfully entered and the system has
 *   been re-awakened.  The internal volatage regulator is back to its
 *   original state.  Otherwise, STOP mode did not occur and a negated
 *   errno value is returned to indicate the cause of the failure.
 *
 * Assumptions:
 *   The caller holds the PM semaphore (g_pmsem) if this function is used
 *   as part of the NuttX power management logic.
 *
 ****************************************************************************/

EXTERN int stm32_pmstop(bool lpds);

/****************************************************************************
 * Name: stm32_pmstandby
 *
 * Description:
 *   Enter STANDBY mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value.
 *   On success, this function will not return (STANDBY mode can only be
 *   terminated with a reset event).  Otherwise, STANDBY mode did not occur
 *   and a negated errno value is returned to indicate the cause of the
 *   failure.
 *
 * Assumptions:
 *   The caller holds the PM semaphore (g_pmsem) if this function is used
 *   as part of the NuttX power management logic.
 *
 ****************************************************************************/

EXTERN int stm32_pmstandby(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_STM32_STM32_PM_H */
