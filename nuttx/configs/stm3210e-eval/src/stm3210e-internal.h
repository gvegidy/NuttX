/************************************************************************************
 * configs/stm3210e_eval/src/stm3210e_internal.h
 * arch/arm/src/board/stm3210e_internal.n
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
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

#ifndef __CONFIGS_STM3210E_EVAL_SRC_STM3210E_INTERNAL_H
#define __CONFIGS_STM3210E_EVAL_SRC_STM3210E_INTERNAL_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>
#include <stdint.h>

/************************************************************************************
 * Definitions
 ************************************************************************************/

/* How many SPI modules does this chip support? The LM3S6918 supports 2 SPI
 * modules (others may support more -- in such case, the following must be
 * expanded).
 */

#if STM32_NSPI < 1
#  undef CONFIG_STM32_SPI1
#  undef CONFIG_STM32_SPI2
#elif STM32_NSPI < 2
#  undef CONFIG_STM32_SPI2
#endif

/* STM3210E-EVAL GPIOs **************************************************************/

/* LEDs */

#define GPIO_LED1       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTF|GPIO_PIN6)
#define GPIO_LED2       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTF|GPIO_PIN7)
#define GPIO_LED3       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTF|GPIO_PIN8)
#define GPIO_LED4       (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_CLEAR|GPIO_PORTF|GPIO_PIN9)

/* BUTTONS -- NOTE that some have EXTI interrupts configured */

#define GPIO_BTN_WAKEUP (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_PORTA|GPIO_PIN0)
#define GPIO_BTN_TAMPER (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_PORTA|GPIO_PIN0)
#define GPIO_BTN_KEY    (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_EXTI|GPIO_PORTG|GPIO_PIN8)
#define GPIO_JOY_KEY    (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_EXTI|GPIO_PORTG|GPIO_PIN7)
#define GPIO_JOY_DOWN   (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_EXTI|GPIO_PORTD|GPIO_PIN3)
#define GPIO_JOY_LEFT   (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_EXTI|GPIO_PORTG|GPIO_PIN14)
#define GPIO_JOY_RIGHT  (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_EXTI|GPIO_PORTG|GPIO_PIN13)
#define GPIO_JOY_UP     (GPIO_INPUT|GPIO_CNF_INFLOAT|GPIO_MODE_INPUT|\
                         GPIO_EXTI|GPIO_PORTG|GPIO_PIN15)

/* SPI FLASH chip select:  PA.4 */

#define GPIO_FLASH_CS   (GPIO_OUTPUT|GPIO_CNF_OUTPP|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_SET|GPIO_PORTB|GPIO_PIN2)

/* USB Soft Connect Pullup: PB.14 */

#define GPIO_USB_PULLUP (GPIO_OUTPUT|GPIO_CNF_OUTOD|GPIO_MODE_50MHz|\
                         GPIO_OUTPUT_SET|GPIO_PORTB|GPIO_PIN14)

/************************************************************************************
 * Public Types
 ************************************************************************************/

/* GPIO settings that will be altered when external memory is selected */

struct extmem_save_s
{
  uint32_t gpiod_crl;
  uint32_t gpiod_crh;
  uint32_t gpioe_crl;
  uint32_t gpioe_crh;
  uint32_t gpiof_crl;
  uint32_t gpiof_crh;
  uint32_t gpiog_crl;
  uint32_t gpiog_crh;
};

/************************************************************************************
 * Public data
 ************************************************************************************/

#ifndef __ASSEMBLY__

/* GPIO configurations common to SRAM and NOR Flash */

#define NCOMMON_CONFIG 37
extern const uint16_t g_commonconfig[NCOMMON_CONFIG];

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_spiinitialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the STM3210E-EVAL board.
 *
 ************************************************************************************/

extern void weak_function stm32_spiinitialize(void);

/************************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called to setup USB-related GPIO pins for the STM3210E-EVAL board.
 *
 ************************************************************************************/

extern void weak_function stm32_usbinitialize(void);

/************************************************************************************
 * Name: stm32_extcontextsave
 *
 * Description:
 *  Save current GPIOs that will used by external memory configurations
 *
 ************************************************************************************/

#ifdef CONFIG_STM32_FSMC
extern void stm32_extcontextsave(struct extmem_save_s *save);

/************************************************************************************
 * Name: stm32_extcontextrestore
 *
 * Description:
 *  Restore GPIOs that were used by external memory configurations
 *
 ************************************************************************************/

extern void stm32_extcontextrestore(struct extmem_save_s *restore);

/************************************************************************************
 * Name: stm32_extmemgpios
 *
 * Description:
 *   Initialize GPIOs for NOR or SRAM
 *
 ************************************************************************************/

extern void stm32_extmemgpios(const uint16_t *gpios, int ngpios);

/************************************************************************************
 * Name: stm32_enablefsmc
 *
 * Description:
 *  enable clocking to the FSMC module
 *
 ************************************************************************************/

extern void stm32_enablefsmc(void);

/************************************************************************************
 * Name: stm32_disablefsmc
 *
 * Description:
 *  enable clocking to the FSMC module
 *
 ************************************************************************************/

extern void stm32_disablefsmc(void);

/************************************************************************************
 * Name: stm32_selectnor
 *
 * Description:
 *   Initialize to access NOR flash
 *
 ************************************************************************************/

extern void stm32_selectnor(void);

/************************************************************************************
 * Name: stm32_deselectnor
 *
 * Description:
 *   Disable NOR FLASH
 *
 ************************************************************************************/

extern void stm32_deselectnor(void);

/************************************************************************************
 * Name: stm32_selectsram
 *
 * Description:
 *   Initialize to access external SRAM
 *
 ************************************************************************************/

extern void stm32_selectsram(void);
/************************************************************************************
 * Name: stm32_deselectsram
 *
 * Description:
 *   Disable external SRAM
 *
 ************************************************************************************/

extern void stm32_deselectsram(void);

/************************************************************************************
 * Name: stm32_selectlcd
 *
 * Description:
 *   Initialize to the LCD
 *
 ************************************************************************************/

extern void stm32_selectlcd(void);

/************************************************************************************
 * Name: stm32_deselectlcd
 *
 * Description:
 *   Disable the LCD
 *
 ************************************************************************************/

extern void stm32_deselectlcd(void);

#endif /* CONFIG_STM32_FSMC */

#endif /* __ASSEMBLY__ */
#endif /* __CONFIGS_STM3210E_EVAL_SRC_STM3210E_INTERNAL_H */

