/********************************************************************************************
 * arch/arm/src/kinetis/kinetis_k60pinset.h
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
 ********************************************************************************************/

#ifndef __ARCH_ARM_SRC_KINETIS_KINETIS_K60PINMUX_H
#define __ARCH_ARM_SRC_KINETIS_KINETIS_K60PINMUX_H

/********************************************************************************************
 * Included Files
 ********************************************************************************************/

#include <nuttx/config.h>

#include "chip.h"

/********************************************************************************************
 * Pre-processor Definitions
 ********************************************************************************************/
/* Reference: Paragraph 10.3.1, p 258, of FreeScale document K60P144M100SF2RM 
 *
 * In most cases, there are alternative configurations for various pins. Those alternative
 * pins are labeled with a suffix like _1, _2, etc. in order to distinguish them.  Logic in
 * the board.h file must select the correct pin configuration for the board by defining a pin
 * configuration (with no suffix) that maps to the correct alternative.
 */

#if defined(CONFIG_ARCH_CHIP_MK60N256VLQ100) || defined(CONFIG_ARCH_CHIP_MK60X256VLQ100) || \
      defined(CONFIG_ARCH_CHIP_MK60N512VLQ100) || defined(CONFIG_ARCH_CHIP_MK60N256VMD100) || \
      defined(CONFIG_ARCH_CHIP_MK60X256VMD100) || defined(CONFIG_ARCH_CHIP_MK60N512VMD100)

#define GPIO_TSI0_CH1             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN0)
#define GPIO_UART0_CTS_1          (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN0)
#define GPIO_FTM0_CH5_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN0)
#define GPIO_JTAG_TCLK            (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN0)
#define GPIO_SWD_CLK              (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN0)
#define GPIO_TSI0_CH2             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN1)
#define GPIO_UART0_RX_1           (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN1)
#define GPIO_FTM0_CH6_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN1)
#define GPIO_JTAG_TDI             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN1)
#define GPIO_TSI0_CH3             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN2)
#define GPIO_UART0_TX_1           (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN2)
#define GPIO_FTM0_CH7_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN2)
#define GPIO_JTAG_TDO             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN2)
#define GPIO_TRACE_SWO            (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN2)
#define GPIO_TSI0_CH4             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN3)
#define GPIO_UART0_RTS_1          (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN3)
#define GPIO_FTM0_CH0_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN3)
#define GPIO_JTAG_TMS             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN3)
#define GPIO_SWD_DIO              (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN3)
#define GPIO_TSI0_CH5             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN4)
#define GPIO_FTM0_CH1_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN4)
#define GPIO_NMI                  (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN4)
#define GPIO_FTM0_CH2_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN5)
#define GPIO_RMII0_RXER           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN5)
#define GPIO_MII0_RXER            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN5)
#define GPIO_CMP2_OUT_1           (GPIO_ALT5   | GPIO_PORTA | GPIO_PIN5)
#define GPIO_I2S0_RX_BCLK_1       (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN5)
#define GPIO_JTAG_TRST            (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN5)
#define GPIO_FTM0_CH3_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN6)
#define GPIO_TRACE_CLKOUT         (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN6)
#define GPIO_ADC0_SE10            (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN7)
#define GPIO_FTM0_CH4_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN7)
#define GPIO_TRACE_D3             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN7)
#define GPIO_ADC0_SE11            (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN8)
#define GPIO_FTM1_CH0_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN8)
#define GPIO_FTM1_QD_PHA_1        (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN8)
#define GPIO_TRACE_D2             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN8)
#define GPIO_FTM1_CH1_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN9)
#define GPIO_MII0_RXD3            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN9)
#define GPIO_FTM1_QD_PHB_1        (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN9)
#define GPIO_TRACE_D1             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN9)
#define GPIO_FTM2_CH0_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN10)
#define GPIO_MII0_RXD2            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN10)
#define GPIO_FTM2_QD_PHA_1        (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN10)
#define GPIO_TRACE_D0             (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN10)
#define GPIO_FTM2_CH1_1           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN11)
#define GPIO_MII0_RXCLK           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN11)
#define GPIO_FTM2_QD_PHB_1        (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN11)
#define GPIO_CMP2_IN0             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN12)
#define GPIO_CAN0_TX_1            (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN12)
#define GPIO_FTM1_CH0_2           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN12)
#define GPIO_RMII0_RXD1           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN12)
#define GPIO_MII0_RXD1            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN12)
#define GPIO_I2S0_TXD_1           (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN12)
#define GPIO_FTM1_QD_PHA_2        (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN12)
#define GPIO_CMP2_IN1             (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN13)
#define GPIO_CAN0_RX_1            (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN13)
#define GPIO_FTM1_CH1_2           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN13)
#define GPIO_RMII0_RXD0           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN13)
#define GPIO_MII0_RXD0            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN13)
#define GPIO_I2S0_TX_FS_1         (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN13)
#define GPIO_FTM1_QD_PHB_2        (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN13)
#define GPIO_SPI0_PCS0_1          (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN14)
#define GPIO_UART0_TX_2           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN14)
#define GPIO_RMII0_CRS_DV         (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN14)
#define GPIO_MII0_RXDV            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN14)
#define GPIO_I2S0_TX_BCLK_1       (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN14)
#define GPIO_SPI0_SCK_1           (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN15)
#define GPIO_UART0_RX_2           (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN15)
#define GPIO_RMII0_TXEN           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN15)
#define GPIO_MII0_TXEN            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN15)
#define GPIO_I2S0_RXD_1           (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN15)
#define GPIO_SPI0_SOUT_1          (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN16)
#define GPIO_UART0_CTS_2          (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN16)
#define GPIO_RMII0_TXD0           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN16)
#define GPIO_MII0_TXD0            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN16)
#define GPIO_I2S0_RX_FS_1         (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN16)
#define GPIO_ADC1_SE17            (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN17)
#define GPIO_SPI0_SIN_1           (GPIO_ALT2   | GPIO_PORTA | GPIO_PIN17)
#define GPIO_UART0_RTS_2          (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN17)
#define GPIO_RMII0_TXD1           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN17)
#define GPIO_MII0_TXD1            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN17)
#define GPIO_I2S0_MCLK_1          (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN17)
#define GPIO_I2S0_CLKIN_1         (GPIO_ALT7   | GPIO_PORTA | GPIO_PIN17)
#define GPIO_EXTAL                (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN18)
#define GPIO_FTM0_FLT2_1          (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN18)
#define GPIO_FTM_CLKIN0           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN18)
#define GPIO_XTAL                 (GPIO_ANALOG | GPIO_PORTA | GPIO_PIN19)
#define GPIO_FTM1_FLT0_1          (GPIO_ALT3   | GPIO_PORTA | GPIO_PIN19)
#define GPIO_FTM_CLKIN1           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN19)
#define GPIO_LPT0_ALT1            (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN19)
#define GPIO_MII0_TXD2            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN24)
#define GPIO_FB_A29               (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN24)
#define GPIO_MII0_TXCLK           (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN25)
#define GPIO_FB_A28               (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN25)
#define GPIO_MII0_TXD3            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN26)
#define GPIO_FB_A27               (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN26)
#define GPIO_MII0_CRS             (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN27)
#define GPIO_FB_A26               (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN27)
#define GPIO_MII0_TXER            (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN28)
#define GPIO_FB_A25               (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN28)
#define GPIO_MII0_COL             (GPIO_ALT4   | GPIO_PORTA | GPIO_PIN29)
#define GPIO_FB_A24               (GPIO_ALT6   | GPIO_PORTA | GPIO_PIN29)

#define GPIO_ADC0_SE8             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN0)
#define GPIO_ADC1_SE8             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN0)
#define GPIO_TSI0_CH0             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN0)
#define GPIO_I2C0_SCL_1           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN0)
#define GPIO_FTM1_CH0_3           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN0)
#define GPIO_RMII0_MDIO           (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN0)
#define GPIO_MII0_MDIO            (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN0)
#define GPIO_FTM1_QD_PHA_3        (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN0)
#define GPIO_ADC0_SE9             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN1)
#define GPIO_ADC1_SE9             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN1)
#define GPIO_TSI0_CH6             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN1)
#define GPIO_I2C0_SDA_1           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN1)
#define GPIO_FTM1_CH1_3           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN1)
#define GPIO_RMII0_MDC            (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN1)
#define GPIO_MII0_MDC             (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN1)
#define GPIO_FTM1_QD_PHB_3        (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN1)
#define GPIO_ADC0_SE12            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN2)
#define GPIO_TSI0_CH7             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN2)
#define GPIO_I2C0_SCL_2           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN2)
#define GPIO_UART0_RTS_3          (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN2)
#define GPIO_ENET0_1588_TMR0      (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN2)
#define GPIO_FTM0_FLT3            (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN2)
#define GPIO_ADC0_SE13            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN3)
#define GPIO_TSI0_CH8             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN3)
#define GPIO_I2C0_SDA_2           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN3)
#define GPIO_UART0_CTS_3          (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN3)
#define GPIO_ENET0_1588_TMR1      (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN3)
#define GPIO_FTM0_FLT0_2          (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN3)
#define GPIO_ADC1_SE10            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN4)
#define GPIO_ENET0_1588_TMR2      (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN4)
#define GPIO_FTM1_FLT0_2          (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN4)
#define GPIO_ADC1_SE11            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN5)
#define GPIO_ENET0_1588_TMR3      (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN5)
#define GPIO_FTM2_FLT0_1          (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN5)
#define GPIO_ADC1_SE12            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN6)
#define GPIO_FB_AD23              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN6)
#define GPIO_ADC1_SE13            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN7)
#define GPIO_FB_AD22              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN7)
#define GPIO_UART3_RTS_1          (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN8)
#define GPIO_FB_AD21              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN8)
#define GPIO_SPI1_PCS1_1          (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN9)
#define GPIO_UART3_CTS_1          (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN9)
#define GPIO_FB_AD20              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN9)
#define GPIO_ADC1_SE14            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN10)
#define GPIO_SPI1_PCS0_1          (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN10)
#define GPIO_UART3_RX_1           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN10)
#define GPIO_FB_AD19              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN10)
#define GPIO_FTM0_FLT1_1          (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN10)
#define GPIO_ADC1_SE15            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN11)
#define GPIO_SPI1_SCK_1           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN11)
#define GPIO_UART3_TX_1           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN11)
#define GPIO_FB_AD18              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN11)
#define GPIO_FTM0_FLT2_2          (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN11)
#define GPIO_TSI0_CH9             (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN16)
#define GPIO_SPI1_SOUT_1          (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN16)
#define GPIO_UART0_RX_3           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN16)
#define GPIO_FB_AD17              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN16)
#define GPIO_EWM_IN_1             (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN16)
#define GPIO_TSI0_CH10            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN17)
#define GPIO_SPI1_SIN_1           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN17)
#define GPIO_UART0_TX_3           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN17)
#define GPIO_FB_AD16              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN17)
#define GPIO_EWM_OUT_1            (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN17)
#define GPIO_TSI0_CH11            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN18)
#define GPIO_CAN0_TX_2            (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN18)
#define GPIO_FTM2_CH0_2           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN18)
#define GPIO_I2S0_TX_BCLK_2       (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN18)
#define GPIO_FB_AD15              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN18)
#define GPIO_FTM2_QD_PHA_2        (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN18)
#define GPIO_TSI0_CH12            (GPIO_ANALOG | GPIO_PORTB | GPIO_PIN19)
#define GPIO_CAN0_RX_2            (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN19)
#define GPIO_FTM2_CH1_2           (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN19)
#define GPIO_I2S0_TX_FS_2         (GPIO_ALT4   | GPIO_PORTB | GPIO_PIN19)
#define GPIO_FB_OE                (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN19)
#define GPIO_FTM2_QD_PHB_2        (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN19)
#define GPIO_SPI2_PCS0_1          (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN20)
#define GPIO_FB_AD31              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN20)
#define GPIO_CMP0_OUT_1           (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN20)
#define GPIO_SPI2_SCK_1           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN21)
#define GPIO_FB_AD30              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN21)
#define GPIO_CMP1_OUT_1           (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN21)
#define GPIO_SPI2_SOUT_1          (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN22)
#define GPIO_FB_AD29              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN22)
#define GPIO_CMP2_OUT_2           (GPIO_ALT6   | GPIO_PORTB | GPIO_PIN22)
#define GPIO_SPI2_SIN_1           (GPIO_ALT2   | GPIO_PORTB | GPIO_PIN23)
#define GPIO_SPI0_PCS5            (GPIO_ALT3   | GPIO_PORTB | GPIO_PIN23)
#define GPIO_FB_AD28              (GPIO_ALT5   | GPIO_PORTB | GPIO_PIN23)

#define GPIO_ADC0_SE14            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN0)
#define GPIO_TSI0_CH13            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN0)
#define GPIO_SPI0_PCS4            (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN0)
#define GPIO_PDB0_EXTRG_1         (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN0)
#define GPIO_I2S0_TXD_2           (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN0)
#define GPIO_FB_AD14              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN0)
#define GPIO_ADC0_SE15            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN1)
#define GPIO_TSI0_CH14            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN1)
#define GPIO_SPI0_PCS3_1          (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN1)
#define GPIO_UART1_RTS_1          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN1)
#define GPIO_FTM0_CH0_2           (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN1)
#define GPIO_FB_AD13              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN1)
#define GPIO_ADC0_SE4B            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN2)
#define GPIO_CMP1_IN0             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN2)
#define GPIO_TSI0_CH15            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN2)
#define GPIO_SPI0_PCS2_2          (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN2)
#define GPIO_UART1_CTS_1          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN2)
#define GPIO_FTM0_CH1_2           (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN2)
#define GPIO_FB_AD12              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN2)
#define GPIO_CMP1_IN1             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN3)
#define GPIO_SPI0_PCS1_1          (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN3)
#define GPIO_UART1_RX_1           (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN3)
#define GPIO_FTM0_CH2_2           (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN3)
#define GPIO_FB_CLKOUT            (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN3)
#define GPIO_SPI0_PCS0_2          (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN4)
#define GPIO_UART1_TX_1           (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN4)
#define GPIO_FTM0_CH3_2           (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN4)
#define GPIO_FB_AD11              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN4)
#define GPIO_CMP1_OUT_2           (GPIO_ALT6   | GPIO_PORTC | GPIO_PIN4)
#define GPIO_SPI0_SCK_2           (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN5)
#define GPIO_LPT0_ALT2            (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN5)
#define GPIO_FB_AD10              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN5)
#define GPIO_CMP0_OUT_2           (GPIO_ALT6   | GPIO_PORTC | GPIO_PIN5)
#define GPIO_CMP0_IN0             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN6)
#define GPIO_SPI0_SOUT_2          (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN6)
#define GPIO_PDB0_EXTRG_2         (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN6)
#define GPIO_FB_AD9               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN6)
#define GPIO_CMP0_IN1             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN7)
#define GPIO_SPI0_SIN_2           (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN7)
#define GPIO_FB_AD8               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN7)
#define GPIO_ADC1_SE4B            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN8)
#define GPIO_CMP0_IN2             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN8)
#define GPIO_I2S0_MCLK_2          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN8)
#define GPIO_I2S0_CLKIN_2         (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN8)
#define GPIO_FB_AD7               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN8)
#define GPIO_ADC1_SE5B            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN9)
#define GPIO_CMP0_IN3             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN9)
#define GPIO_I2S0_RX_BCLK_2       (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN9)
#define GPIO_FB_AD6               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN9)
#define GPIO_FTM2_FLT0_2          (GPIO_ALT6   | GPIO_PORTC | GPIO_PIN9)
#define GPIO_ADC1_SE6B            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN10)
#define GPIO_CMP0_IN4             (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN10)
#define GPIO_I2C1_SCL_1           (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN10)
#define GPIO_I2S0_RX_FS_2         (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN10)
#define GPIO_FB_AD5               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN10)
#define GPIO_ADC1_SE7B            (GPIO_ANALOG | GPIO_PORTC | GPIO_PIN11)
#define GPIO_I2C1_SDA_1           (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN11)
#define GPIO_I2S0_RXD_2           (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN11)
#define GPIO_FB_RW                (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN11)
#define GPIO_UART4_RTS_1          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN12)
#define GPIO_FB_AD27              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN12)
#define GPIO_UART4_CTS_1          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN13)
#define GPIO_FB_AD26              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN13)
#define GPIO_UART4_RX_1           (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN14)
#define GPIO_FB_AD25              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN14)
#define GPIO_UART4_TX_1           (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN15)
#define GPIO_FB_AD24              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN15)
#define GPIO_CAN1_RX_1            (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN16)
#define GPIO_UART3_RX_2           (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN16)
#define GPIO_ENET0_1588_TMR0      (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN16)
#define GPIO_FB_CS5               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN16)
#define GPIO_FB_TSIZ1             (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN16)
#define GPIO_FB_BE23_16_BLS15_8   (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN16)
#define GPIO_CAN1_TX_1            (GPIO_ALT2   | GPIO_PORTC | GPIO_PIN17)
#define GPIO_UART3_TX_2           (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN17)
#define GPIO_ENET0_1588_TMR1      (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN17)
#define GPIO_FB_CS4               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN17)
#define GPIO_FB_TSIZ0             (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN17)
#define GPIO_FB_BE31_24_BLS7_0    (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN17)
#define GPIO_UART3_RTS_2          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN18)
#define GPIO_ENET0_1588_TMR2      (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN18)
#define GPIO_FB_TBST              (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN18)
#define GPIO_FB_CS2               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN18)
#define GPIO_FB_BE15_8_BLS23_16   (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN18)
#define GPIO_UART3_CTS_2          (GPIO_ALT3   | GPIO_PORTC | GPIO_PIN19)
#define GPIO_ENET0_1588_TMR3      (GPIO_ALT4   | GPIO_PORTC | GPIO_PIN19)
#define GPIO_FB_CS3               (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN19)
#define GPIO_FB_BE7_0_BLS31_24    (GPIO_ALT5   | GPIO_PORTC | GPIO_PIN19)
#define GPIO_FB_TA                (GPIO_ALT6   | GPIO_PORTC | GPIO_PIN19)

#define GPIO_SPI0_PCS0_3          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN0)
#define GPIO_UART2_RTS            (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN0)
#define GPIO_FB_ALE               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN0)
#define GPIO_FB_CS1               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN0)
#define GPIO_FB_TS                (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN0)
#define GPIO_ADC0_SE5B            (GPIO_ANALOG | GPIO_PORTD | GPIO_PIN1)
#define GPIO_SPI0_SCK_3           (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN1)
#define GPIO_UART2_CTS            (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN1)
#define GPIO_FB_CS0               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN1)
#define GPIO_SPI0_SOUT_3          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN2)
#define GPIO_UART2_RX             (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN2)
#define GPIO_FB_AD4               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN2)
#define GPIO_SPI0_SIN_3           (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN3)
#define GPIO_UART2_TX             (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN3)
#define GPIO_FB_AD3               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN3)
#define GPIO_SPI0_PCS1_2          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN4)
#define GPIO_UART0_RTS_4          (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN4)
#define GPIO_FTM0_CH4_2           (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN4)
#define GPIO_FB_AD2               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN4)
#define GPIO_EWM_IN_2             (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN4)
#define GPIO_ADC0_SE6B            (GPIO_ANALOG | GPIO_PORTD | GPIO_PIN5)
#define GPIO_SPI0_PCS2_1          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN5)
#define GPIO_UART0_CTS_4          (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN5)
#define GPIO_FTM0_CH5_2           (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN5)
#define GPIO_FB_AD1               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN5)
#define GPIO_EWM_OUT_2            (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN5)
#define GPIO_ADC0_SE7B            (GPIO_ANALOG | GPIO_PORTD | GPIO_PIN6)
#define GPIO_SPI0_PCS3_2          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN6)
#define GPIO_UART0_RX_4           (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN6)
#define GPIO_FTM0_CH6_2           (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN6)
#define GPIO_FB_AD0               (GPIO_ALT5   | GPIO_PORTD | GPIO_PIN6)
#define GPIO_FTM0_FLT0_1          (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN6)
#define GPIO_CMT_IRO              (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN7)
#define GPIO_UART0_TX_4           (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN7)
#define GPIO_FTM0_CH7_2           (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN7)
#define GPIO_FTM0_FLT1_2          (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN7)
#define GPIO_I2C0_SCL_3           (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN8)
#define GPIO_UART5_RX_1           (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN8)
#define GPIO_FB_A16               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN8)
#define GPIO_I2C0_SDA             (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN9)
#define GPIO_UART5_TX_1           (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN9)
#define GPIO_FB_A17               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN9)
#define GPIO_UART5_RTS_1          (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN10)
#define GPIO_FB_A18               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN10)
#define GPIO_SPI2_PCS0_2          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN11)
#define GPIO_UART5_CTS_1          (GPIO_ALT3   | GPIO_PORTD | GPIO_PIN11)
#define GPIO_SDHC0_CLKIN          (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN11)
#define GPIO_FB_A19               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN11)
#define GPIO_SPI2_SCK_2           (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN12)
#define GPIO_SDHC0_D4             (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN12)
#define GPIO_FB_A20               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN12)
#define GPIO_SPI2_SOUT_2          (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN13)
#define GPIO_SDHC0_D5             (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN13)
#define GPIO_FB_A21               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN13)
#define GPIO_SPI2_SIN_2           (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN14)
#define GPIO_SDHC0_D6             (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN14)
#define GPIO_FB_A22               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN14)
#define GPIO_SPI2_PCS1            (GPIO_ALT2   | GPIO_PORTD | GPIO_PIN15)
#define GPIO_SDHC0_D7             (GPIO_ALT4   | GPIO_PORTD | GPIO_PIN15)
#define GPIO_FB_A23               (GPIO_ALT6   | GPIO_PORTD | GPIO_PIN15)

#define GPIO_ADC1_SE4A            (GPIO_ANALOG | GPIO_PORTE | GPIO_PIN0)
#define GPIO_SPI1_PCS1_2          (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN0)
#define GPIO_UART1_TX_2           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN0)
#define GPIO_SDHC0_D1             (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN0)
#define GPIO_I2C1_SDA_2           (GPIO_ALT6   | GPIO_PORTE | GPIO_PIN0)
#define GPIO_ADC1_SE5A            (GPIO_ANALOG | GPIO_PORTE | GPIO_PIN1)
#define GPIO_SPI1_SOUT_2          (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN1)
#define GPIO_UART1_RX_2           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN1)
#define GPIO_SDHC0_D0             (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN1)
#define GPIO_I2C1_SCL_2           (GPIO_ALT6   | GPIO_PORTE | GPIO_PIN1)
#define GPIO_ADC1_SE6A            (GPIO_ANALOG | GPIO_PORTE | GPIO_PIN2)
#define GPIO_SPI1_SCK_2           (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN2)
#define GPIO_UART1_CTS_2          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN2)
#define GPIO_SDHC0_DCLK           (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN2)
#define GPIO_ADC1_SE7A            (GPIO_ANALOG | GPIO_PORTE | GPIO_PIN3)
#define GPIO_SPI1_SIN_2           (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN3)
#define GPIO_UART1_RTS_2          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN3)
#define GPIO_SDHC0_CMD            (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN3)
#define GPIO_SPI1_PCS0_2          (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN4)
#define GPIO_UART3_TX_3           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN4)
#define GPIO_SDHC0_D3             (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN4)
#define GPIO_SPI1_PCS2            (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN5)
#define GPIO_UART3_RX_3           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN5)
#define GPIO_SDHC0_D2             (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN5)
#define GPIO_SPI1_PCS3            (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN6)
#define GPIO_UART3_CTS_3          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN6)
#define GPIO_I2S0_MCLK_3          (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN6)
#define GPIO_I2S0_CLKIN_3         (GPIO_ALT6   | GPIO_PORTE | GPIO_PIN6)
#define GPIO_UART3_RTS_3          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN7)
#define GPIO_I2S0_RXD_3           (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN7)
#define GPIO_UART5_TX_2           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN8)
#define GPIO_I2S0_RX_FS_3         (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN8)
#define GPIO_UART5_RX_2           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN9)
#define GPIO_I2S0_RX_BCLK_3       (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN9)
#define GPIO_UART5_CTS_1          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN10)
#define GPIO_I2S0_TXD_3           (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN10)
#define GPIO_UART5_RTS_2          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN11)
#define GPIO_I2S0_TX_FS_3         (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN11)
#define GPIO_I2S0_TX_BCLK_3       (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN12)
#define GPIO_ADC0_SE17            (GPIO_ANALOG | GPIO_PORTE | GPIO_PIN24)
#define GPIO_CAN1_TX_2            (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN24)
#define GPIO_UART4_TX_2           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN24)
#define GPIO_EWM_OUT_3            (GPIO_ALT6   | GPIO_PORTE | GPIO_PIN24)
#define GPIO_ADC0_SE18            (GPIO_ANALOG | GPIO_PORTE | GPIO_PIN25)
#define GPIO_CAN1_RX_2            (GPIO_ALT2   | GPIO_PORTE | GPIO_PIN25)
#define GPIO_UART4_RX_2           (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN25)
#define GPIO_EWM_IN_3             (GPIO_ALT6   | GPIO_PORTE | GPIO_PIN25)
#define GPIO_UART4_CTS_2          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN26)
#define GPIO_ENET_1588_CLKIN      (GPIO_ALT4   | GPIO_PORTE | GPIO_PIN26)
#define GPIO_RTC_CLKOUT           (GPIO_ALT6   | GPIO_PORTE | GPIO_PIN26)
#define GPIO_USB_CLKIN            (GPIO_ALT7   | GPIO_PORTE | GPIO_PIN26)
#define GPIO_UART4_RTS_2          (GPIO_ALT3   | GPIO_PORTE | GPIO_PIN27)

#else
  /* The pin muxing for other K60 parts is defined in other documents */

#  error "No pin multiplexing for this Kinetis K60 part"
#endif

/********************************************************************************************
 * Public Types
 ********************************************************************************************/

/********************************************************************************************
 * Public Data
 ********************************************************************************************/

/********************************************************************************************
 * Public Functions
 ********************************************************************************************/

#endif /* __ARCH_ARM_SRC_KINETIS_KINETIS_K60PINMUX_H */
