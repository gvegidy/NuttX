/************************************************************************************
 * arch/arm/src/str71x/str71x_gpio.h
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_STR71X_STR71X_GPIO_H
#define __ARCH_ARM_SRC_STR71X_STR71X_GPIO_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>
#include "str71x_map.h"

/************************************************************************************
 * Definitions
 ************************************************************************************/

/* GPIO registers *******************************************************************/

#define STR71X_GPIO_PC0       (STR71X_GPIO_BASE + 0x0000)  /* 16-bits wide */
#define STR71X_GPIO_PC1       (STR71X_GPIO_BASE + 0x0004)  /* 16-bits wide */
#define STR71X_GPIO_PC2       (STR71X_GPIO_BASE + 0x0008)  /* 16-bits wide */
#define STR71X_GPIO_PD        (STR71X_GPIO_BASE + 0x000c)  /* 16-bits wide */

/* Register bit settings ************************************************************/

#define STR71X_GPIO_HIAINTRI  (0)  /* High impedance analog input tristate   */
#define STR71X_GPIO_INTRITTL  (1)  /* Tristate ttl input                     */
#define STR71X_GPIO_INTRICMOS (2)  /* Tristate cmos input                    */
#define STR71X_GPIO_IPUPDWP   (3)  /* Pull-up/pull-down weak push pull input */
#define STR71X_GPIO_OUTOD     (4)  /* Open drain output                      */
#define STR71X_GPIO_OUTPP     (5)  /* Push pull output                       */
#define STR71X_GPIO_AFOD      (6)  /* Open drain output alternate function   */
#define STR71X_GPIO_AFPP      (7)  /* Push-pull output alternate function    */

#define STR71X_GPIO_LSB       (0x00)
#define STR71X_GPIO_MSB       (0x08)

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public Data
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#endif /* __ARCH_ARM_SRC_STR71X_STR71X_GPIO_H */
