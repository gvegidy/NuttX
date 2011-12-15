/****************************************************************************
 * arch/arm/src/stm32/stm32_adc.c
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *           Diego Sanchez <dsanchez@nx-engineering.com>
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

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <arch/board/board.h>
#include <nuttx/arch.h>
#include <nuttx/analog/adc.h>

#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"
#include "stm32_internal.h"
#include "stm32_adc.h"

#ifdef CONFIG_ADC

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Up to 3 ADC interfaces are supported */

#if STM32_NADC < 3
#  undef CONFIG_STM32_ADC3
#endif

#if STM32_NADC < 2
#  undef CONFIG_STM32_ADC2
#endif

#if STM32_NADC < 1
#  undef CONFIG_STM32_ADC1
#endif

#if defined(CONFIG_STM32_ADC1) || defined(CONFIG_STM32_ADC2) || defined(CONFIG_STM32_ADC3)

/* ADC interrupts */

#ifdef CONFIG_STM32_STM32F10XX
#  define ADC_SR_ALLINTS (ADC_SR_AWD | ADC_SR_EOC | ADC_SR_JEOC)
#else
#  define ADC_SR_ALLINTS (ADC_SR_AWD | ADC_SR_EOC | ADC_SR_JEOC | ADC_SR_OVR)
#endif

#ifdef CONFIG_STM32_STM32F10XX
#  define ADC_CR1_ALLINTS (ADC_CR1_AWDIE | ADC_CR1_EOCIE | ADC_CR1_JEOCIE)
#else
#  define ADC_CR1_ALLINTS (ADC_CR1_AWDIE | ADC_CR1_EOCIE | ADC_CR1_JEOCIE | ADC_CR1_OVRIE)
#endif

/* The maximum number of samples */

#define ADC_MAX_SAMPLES 16

/****************************************************************************
 * Private Types
 ****************************************************************************/
 
/* This structure describes the state of one ADC block */

struct stm32_dev_s
{
  uint8_t  irq;       /* Interrupt generated by this ADC block */
  uint8_t  nchannels; /* Number of channels */
  uint8_t  intf;      /* ADC interface number */
  xcpt_t   isr;       /* Interrupt handler for this ADC block */
  uint32_t base;      /* Base address of registers unique to this ADC block */

  uint8_t  chanlist[ADC_MAX_SAMPLES];
  int32_t  buf[8];
  uint8_t  count[8];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* ADC Register access */

static uint32_t adc_getreg(struct stm32_dev_s *priv, int offset);
static void adc_putreg(struct stm32_dev_s *priv, int offset, uint32_t value);
static void adc_rccreset(int regaddr, bool reset);

/* ADC Interrupt Handler */

static int adc_interrupt(FAR struct stm32_dev_s *priv);
#if defined(CONFIG_STM32_STM32F10XX) && (defined(CONFIG_STM32_ADC1) || defined(CONFIG_STM32_ADC2))
static int adc12_interrupt(int irq, void *context);
#endif
#if defined(CONFIG_STM32_STM32F10XX) && defined (CONFIG_STM32_ADC3)
static int adc3_interrupt(int irq, void *context);
#endif
#ifdef CONFIG_STM32_STM32F40XX
static int adc123_interrupt(int irq, void *context);
#endif

/* ADC Driver Methods */

static void adc_reset(FAR struct adc_dev_s *dev);
static int  adc_setup(FAR struct adc_dev_s *dev);
static void adc_shutdown(FAR struct adc_dev_s *dev);
static void adc_rxint(FAR struct adc_dev_s *dev, bool enable);
static int  adc_ioctl(FAR struct adc_dev_s *dev, int cmd, unsigned long arg);
static void adc_enable(FAR struct adc_dev_s *dev, bool enable);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* ADC interface operations */

static const struct adc_ops_s g_adcops =
{
  .ao_reset    = adc_reset,
  .ao_setup    = adc_setup,
  .ao_shutdown = adc_shutdown,
  .ao_rxint    = adc_rxint,
  .ao_ioctl    = adc_ioctl,
};

/* ADC1 state */

#ifdef CONFIG_STM32_ADC1
static struct stm32_dev_s g_adcpriv1 =
{
#ifdef CONFIG_STM32_STM32F10XX
  .irq         = STM32_IRQ_ADC12,
  .isr         = adc12_interrupt,
#else
  .irq         = STM32_IRQ_ADC,
  .isr         = adc123_interrupt,
#endif
  .intf        = 1;
  .base        = STM32_ADC1_BASE,
};

static struct adc_dev_s g_adcdev1 =
{
  .ad_ops = &g_adcops,
  .ad_priv= &g_adcpriv1,
};
#endif

/* ADC2 state */

#ifdef CONFIG_STM32_ADC2
static struct stm32_dev_s g_adcpriv2 =
{
#ifdef CONFIG_STM32_STM32F10XX
  .irq         = STM32_IRQ_ADC12,
  .isr         = adc12_interrupt,
#else
  .irq         = STM32_IRQ_ADC,
  .isr         = adc123_interrupt,
#endif
  .intf        = 2;
  .base        = STM32_ADC2_BASE,
};

static struct adc_dev_s g_adcdev2 =
{
  .ad_ops = &g_adcops,
  .ad_priv= &g_adcpriv2,
};
#endif

/* ADC3 state */

#ifdef CONFIG_STM32_ADC3
static struct stm32_dev_s g_adcpriv3 =
{
#ifdef CONFIG_STM32_STM32F10XX
  .irq         = STM32_IRQ_ADC3,
  .isr         = adc3_interrupt,
#else
  .irq         = STM32_IRQ_ADC,
  .isr         = adc123_interrupt,
#endif
  .intf        = 3;
  .base        = STM32_ADC3_BASE,
};

static struct adc_dev_s g_adcdev3 =
{
  .ad_ops = &g_adcops,
  .ad_priv= &g_adcpriv3,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: adc_getreg
 *
 * Description:
 *   Read the value of an ADC register.
 *
 * Input Parameters:
 *   priv - A reference to the ADC block status
 *   offset - The offset to the register to read
 *
 * Returned Value:
 *
 ****************************************************************************/

static uint32_t adc_getreg(struct stm32_dev_s *priv, int offset)
{
  return getreg32(priv->base + offset);
}

/****************************************************************************
 * Name: adc_getreg
 *
 * Description:
 *   Read the value of an ADC register.
 *
 * Input Parameters:
 *   priv - A reference to the ADC block status
 *   offset - The offset to the register to read
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_putreg(struct stm32_dev_s *priv, int offset, uint32_t value)
{
  putreg32(value, priv->base + offset);
}

/****************************************************************************
 * Name: adc_rccreset
 *
 * Description:
 *   Deinitializes the ADCx peripheral registers to their default 
 *   reset values. It could set all the ADCs configured.
 *
 * Input Parameters:
 *   regaddr - The register to read
 *   reset - Condition, set or reset
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_rccreset(struct stm32_dev_s *priv, bool reset)
{
  uint32_t regval;
  uint32_t adcbit;

  /* Pick the appropriate bit in the APB2 reset register */

#ifdef CONFIG_STM32_STM32F10XX
  /* For the STM32 F1, there is an individual bit to reset each ADC. */

  switch (priv->intf)
    {
#ifdef CONFIG_STM32_ADC1
      case 1:
        adcbit = RCC_APB2RSTR_ADC1RST;
        break;
#endif
#ifdef CONFIG_STM32_ADC2
      case 2:
        adcbit = RCC_APB2RSTR_ADC2RST;
        break;
#endif
#ifdef CONFIG_STM32_ADC3
      case 3:
        adcbit = RCC_APB2RSTR_ADC3RST;
        break;
#endif
      default:
        return;
    }

#else
  /* For the STM32 F4, there is one common reset for all ADC block.
   * THIS will probably cause some problems!
   */

  adcbit = RCC_APB2RSTR_ADCRST;
#endif

  /* Set or clear the selected bit in the APB2 reset register */

  regval = getreg32(STM32_RCC_APB2RSTR);
  if (reset)
    {
      /* Enable  ADC reset state */

      regval |= adcbit;
    }
  else
    {
      /* Release ADC from reset state */

      regval &= ~adcbit;
    }
  putreg32(regval, STM32_RCC_APB2RSTR);
}

/*******************************************************************************
 * Name: adc_enable
 *
 * Description    : Enables or disables the specified ADC peripheral.
 *
 * Input Parameters:
 *
 *   enable - true:  enable ADC convertion
 *            false: disable ADC convertion
 *
 * Returned Value:
 *
 *******************************************************************************/
static void adc_enable(FAR struct adc_dev_s *dev, bool enable)
{
  FAR struct stm32_dev_s *priv = (FAR struct stm32_dev_s *)dev->ad_priv;
  uint32_t regval;

  regval  = adc_getreg(priv, STM32_ADC_CR2_OFFSET);
  if (enable)
    {
      regval |= ADC_CR2_ADON;
    }
  else
    {
      regval &= ~ADC_CR2_ADON;
    }
  adc_putreg(priv, STM32_ADC_CR2_OFFSET, regval);
}

/****************************************************************************
 * Name: adc_reset
 *
 * Description:
 *   Reset the ADC device.  Called early to initialize the hardware. This
 *   is called, before adc_setup() and on error conditions.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_reset(FAR struct adc_dev_s *dev)
{
  adbg("Initializing the ADC to the reset values \n");
  
  FAR struct stm32_dev_s *priv = (FAR struct stm32_dev_s *)dev->ad_priv;
  irqstate_t flags;
  uint32_t regval;
  uint32_t L = priv->nchannels;
  uint32_t ch;
  int offset = 0;
    
  flags = irqsave();

  /* Enable  ADC reset state */

  adc_rccreset(priv, true);

  /* Release ADC from reset state */

  adc_rccreset(priv, false);
  
  /* Initialize the ADC data structures */

  /* Initialize the watchdog high threshold register */

  adc_putreg(priv, STM32_ADC_HTR_OFFSET, 0x00000fff);

  /* Initialize the watchdog low threshold register */

  adc_putreg(priv, STM32_ADC_LTR_OFFSET, 0x00000000);

#ifdef CONFIG_STM32_STM32F40XX  
  /* Initialize ADC Prescaler*/

  regval = getreg32(STM32_ADC_CCR_OFFSET);

  /* PCLK2 divided by 2 */

  regval &= ~ADC_CCR_ADCPRE_MASK;
  putreg32(regval,STM32_ADC_CCR_OFFSET);
#endif

  /* Initialize the same sample time for each ADC 1.5 cycles
   *
   * During sample cycles channel selection bits must remain unchanged.
   *
   * 000: 1.5 cycles
   * 001: 7.5 cycles
   * 010: 13.5 cycles
   * 011: 28.5 cycles
   * 100: 41.5 cycles
   * 101: 55.5 cycles
   * 110: 71.5 cycles
   * 111: 239.5 cycles
   */

  adc_putreg(priv,STM32_ADC_SMPR1_OFFSET,0x00000000);
  adc_putreg(priv,STM32_ADC_SMPR2_OFFSET,0x00000000);
  
  /* ADC CR1 Configuration */

  regval  = adc_getreg(priv, STM32_ADC_CR1_OFFSET);
  regval &= ~ADC_CR1_DUALMOD_MASK;
  regval &= ~ADC_CR1_SCAN;   /* Clear DUALMODE and SCAN bits */
  adc_putreg(priv, STM32_ADC_CR1_OFFSET, regval);

  /* Initialize the ADC_Mode (ADC_Mode_Independent)  */

  regval  = adc_getreg(priv, STM32_ADC_CR1_OFFSET);
  regval |= ADC_CR1_IND;

  /* Initialize the ADC_CR1_SCAN member DISABLE */

  regval &= ~ADC_CR1_SCAN;
  adc_putreg(priv, STM32_ADC_CR1_OFFSET, regval);

  /* ADC1 CR2 Configuration */
  
  /* Set the ADON bit to wake up the ADC from power down mode */

  regval  = adc_getreg(priv, STM32_ADC_CR2_OFFSET);
  regval |= ADC_CR2_ADON;
    
  /* Clear CONT, ALIGN and EXTTRIG bits */

  regval  = adc_getreg(priv, STM32_ADC_CR2_OFFSET);
  regval &= ~ADC_CR2_CONT;
  regval &= ~ADC_CR2_ALIGN;
  regval &= ~ADC_CR2_EXTSEL_MASK;
  adc_putreg(priv, STM32_ADC_CR2_OFFSET, regval);

  /* Set CONT, ALIGN and EXTTRIG bits */
  /* Initialize the ALIGN: Data alignment Right */

  regval  = adc_getreg(priv, STM32_ADC_CR2_OFFSET);
  regval &= ~ADC_CR2_ALIGN;

  /* Initialize the External event select "Timer CC1 event" */

  regval &= ~ADC_CR2_EXTSEL_MASK;

  /* Initialize the ADC_ContinuousConvMode "Single conversion mode" */

  regval &= ~ADC_CR2_CONT;
  adc_putreg(priv, STM32_ADC_CR2_OFFSET, regval);

  /* ADC1 SQR Configuration */

  L = L << 20;
  regval  = adc_getreg(priv, STM32_ADC_SQR1_OFFSET);
  regval &= ~ADC_SQR1_L_MASK; /* Clear L Mask */ 
  regval |= L;                /* SetL, # of convertions */
  adc_putreg(priv, STM32_ADC_SQR1_OFFSET, regval);

  /* Configuration of the channels convertions */

#warning "I can improve the ugly code below with a logic using offsets"
#warning "Or.. better yet, a loop"
  regval = adc_getreg(priv, STM32_ADC_SQR3_OFFSET);
  if (priv->nchannels >= 1)
    {
      ch = priv->chanlist[0];
      ch <<= offset;
      regval &= ~ADC_SQR3_SQ1_MASK; /* clear SQ1 */
      regval |= ch;                 /* Set SQ1 */
      adc_putreg(priv, STM32_ADC_SQR3_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 2)
    {
      ch = priv->chanlist[1];
      ch <<= offset;
      regval &= ~ADC_SQR3_SQ2_MASK; /* clear SQ2 */
      regval |= ch;                 /* Set SQ2 */
      adc_putreg(priv, STM32_ADC_SQR3_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 3)
    {
      ch = priv->chanlist[2];
      ch <<= offset;
      regval &= ~ADC_SQR3_SQ3_MASK; /* clear SQ3 */
      regval |= ch;                 /* SetSQ3 */
      adc_putreg(priv, STM32_ADC_SQR3_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 4)
    {
      ch = priv->chanlist[3];
      ch <<= offset;
      regval &= ~ADC_SQR3_SQ4_MASK; /* clear SQ4 */
      regval |= ch;                 /* SetSQ4 */
      adc_putreg(priv, STM32_ADC_SQR3_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 5)
    {
      ch = priv->chanlist[4];
      ch <<= offset;
      regval &= ~ADC_SQR3_SQ5_MASK; /* clear SQ5 */
      regval |= ch;                 /* SetSQ5 */
      adc_putreg(priv, STM32_ADC_SQR3_OFFSET, regval);
     offset += 5;
    }

  if (priv->nchannels >= 6)
    {
      ch = priv->chanlist[5];
      ch <<= offset;
      regval &= ~ADC_SQR3_SQ6_MASK; /* clear SQ6 */
      regval |= ch;                 /* SetSQ6 */
      adc_putreg(priv, STM32_ADC_SQR3_OFFSET, regval);
      offset = 0;
    }

  if (priv->nchannels >= 7)
    {
      ch = priv->chanlist[6];
      ch <<= offset;
      regval &= ~ADC_SQR2_SQ7_MASK; /* clear SQ7 */
      regval |= ch;                 /* SetSQ7 */
      adc_putreg(priv, STM32_ADC_SQR2_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 8)
    {
      ch = priv->chanlist[7];
      ch <<= offset;
      regval &= ~ADC_SQR2_SQ8_MASK; /* clear SQ8 */
      regval |= ch;                 /* SetSQ8 */
      adc_putreg(priv, STM32_ADC_SQR2_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 9)
    {
      ch = priv->chanlist[8];
      ch <<= offset;
      regval &= ~ADC_SQR2_SQ9_MASK; /* clear SQ9 */
      regval |= ch;                 /* SetSQ9 */
      adc_putreg(priv, STM32_ADC_SQR2_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 10)
    {
      ch = priv->chanlist[9];
      ch <<= offset;
      regval &= ~ADC_SQR2_SQ10_MASK; /* clear SQ10 */
      regval |= ch;                  /* SetSQ10 */
      adc_putreg(priv, STM32_ADC_SQR2_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 11)
    {
      ch = priv->chanlist[10];
      ch <<= offset;
      regval &= ~ADC_SQR2_SQ11_MASK; /* clear SQ11 */
      regval |= ch;                  /* SetSQ11 */
      adc_putreg(priv, STM32_ADC_SQR2_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 12)
    {
      ch = priv->chanlist[11];
      ch <<= offset;
      regval &= ~ADC_SQR2_SQ12_MASK; /* clear SQ12 */
      regval |= ch;                  /* SetSQ12 */
      adc_putreg(priv, STM32_ADC_SQR2_OFFSET, regval);
      offset = 0;
    }

  if (priv->nchannels >= 13)
    {
      ch = priv->chanlist[12];
      ch <<= offset;
      regval &= ~ADC_SQR1_SQ13_MASK; /* clear SQ13 */
      regval |= ch;                  /* SetSQ13 */
      adc_putreg(priv, STM32_ADC_SQR1_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 14)
    {
      ch = priv->chanlist[13];
      ch <<= offset;
      regval &= ~ADC_SQR1_SQ14_MASK; /* clear SQ14 */
      regval |= ch;                  /* SetSQ14 */
      adc_putreg(priv, STM32_ADC_SQR1_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 15)
    {
      ch = priv->chanlist[14];
      ch <<= offset;
      regval &= ~ADC_SQR1_SQ15_MASK; /* clear SQ15 */
      regval |= ch;                  /* SetSQ15 */
      adc_putreg(priv, STM32_ADC_SQR1_OFFSET, regval);
      offset += 5;
    }

  if (priv->nchannels >= 16)
    {
      ch = priv->chanlist[15];
      ch <<= offset;
      regval &= ~ADC_SQR1_SQ16_MASK; /* clear SQ16 */
      regval |= ch;                  /* SetSQ16 */
      adc_putreg(priv, STM32_ADC_SQR1_OFFSET, regval);
    }

  if (priv->nchannels >= 17)
    {
      adbg("ERROR: Number of channels exceeded\n");
    }
  irqrestore(flags);
}

/****************************************************************************
 * Name: adc_setup
 *
 * Description:
 *   Configure the ADC. This method is called the first time that the ADC
 *   device is opened.  This will occur when the port is first opened.
 *   This setup includes configuring and attaching ADC interrupts.  Interrupts
 *   are all disabled upon return.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static int adc_setup(FAR struct adc_dev_s *dev)
{
  FAR struct stm32_dev_s *priv = (FAR struct stm32_dev_s *)dev->ad_priv;
  int ret;
  int i;
  /* Attach the ADC interrupt */

  ret = irq_attach(priv->irq, priv->isr);
  if (ret == OK)
    {
      for (i = 0; i < 8; i++)
      {
        priv->buf[i]   = 0;
        priv->count[i] = 0;
      }
      
      /* Enable the ADC interrupt */

      up_enable_irq(priv->irq);
    }

  return ret;
}

/****************************************************************************
 * Name: adc_shutdown
 *
 * Description:
 *   Disable the ADC.  This method is called when the ADC device is closed.
 *   This method reverses the operation the setup method.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_shutdown(FAR struct adc_dev_s *dev)
{
  FAR struct stm32_dev_s *priv = (FAR struct stm32_dev_s *)dev->ad_priv;

  /* Disable ADC interrupts and detach the ADC interrupt handler */

  up_disable_irq(priv->irq);
  irq_detach(priv->irq);

  /* Disable and reset the ADC module */
}

/****************************************************************************
 * Name: adc_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_rxint(FAR struct adc_dev_s *dev, bool enable)
{
  FAR struct stm32_dev_s *priv = (FAR struct stm32_dev_s *)dev->ad_priv;
  uint32_t regval;

  regval = adc_getreg(priv, STM32_ADC_CR1_OFFSET);
  if (enable)
    {
      /* Enable the end-of-conversion ADC interrupt */

      regval |= ADC_CR1_EOCIE;
    }
  else
    {
      /* Enable all ADC interrupts */

      regval &= ~ADC_CR1_ALLINTS;
    }
  adc_putreg(priv, STM32_ADC_CR1_OFFSET, regval);
}

/****************************************************************************
 * Name: adc_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static int  adc_ioctl(FAR struct adc_dev_s *dev, int cmd, unsigned long arg)
{
  return -ENOTTY;
}

/****************************************************************************
 * Name: adc_interrupt
 *
 * Description:
 *   Common ADC interrupt handler.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static int adc_interrupt(FAR struct stm32_dev_s *priv)
{
  uint32_t adcsr;
  int32_t  value;
  uint8_t  ch;
  int      i;

  /* Identifies the interruption AWD or EOC */
  
  adcsr = adc_getreg(priv, STM32_ADC_SR_OFFSET);
  if ((adcsr & ADC_SR_AWD) != 0)
    {
      adbg(" Analog Watchdog, Value converted out of range!\n");
    }
  
  /* EOC: End of conversion */

  if ((adcsr & ADC_SR_EOC) != 0)
    {
      value  = adc_getreg(priv, STM32_ADC_DR_OFFSET);
      value &= ADC_DR_DATA_MASK;
#error "i is not assigned a value"
      ch     = priv->chanlist[i]; /* Channel converted */
    
      /* Handle the ADC interrupt */

      adc_receive(priv, ch, value);
      priv->buf[ch]   = 0;
      priv->count[ch] = 0;
    }

  return OK;
}

/****************************************************************************
 * Name: adc12_interrupt
 *
 * Description:
 *   ADC12 interrupt handler for the STM32 F1 family.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

#if defined(CONFIG_STM32_STM32F10XX) && (defined(CONFIG_STM32_ADC1) || defined(CONFIG_STM32_ADC2))
static int adc12_interrupt(int irq, void *context)
{
  uint32_t regval;
  uint32_t pending;

  /* Check for pending ADC1 interrupts */

#ifdef CONFIG_STM32_ADC1
  regval  = getreg32(STM32_ADC1_SR);
  pending = regval & ADC_SR_ALLINTS;
  if (pending != 0)
    {
      adc_interrupt(&g_adcpriv1);
      regval &= ~pending;
      putreg32(regval, STM32_ADC1_SR);
    }
#endif

  /* Check for pending ADC2 interrupts */

#ifdef CONFIG_STM32_ADC2
  regval  = getreg32(STM32_ADC2_SR);
  pending = regval & ADC_SR_ALLINTS;
  if (pending != 0)
    {
      adc_interrupt(&g_adcpriv2);
      regval &= ~pending;
      putreg32(regval, STM32_ADC2_SR);
    }
#endif
  return OK;
}
#endif

/****************************************************************************
 * Name: adc3_interrupt
 *
 * Description:
 *   ADC1/2 interrupt handler for the STM32 F1 family.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

#if defined (CONFIG_STM32_STM32F10XX) && defined (CONFIG_STM32_ADC3)
static int adc3_interrupt(int irq, void *context)
{
  uint32_t regval;
  uint32_t pending;

  /* Check for pending ADC3 interrupts */

  regval  = getreg32(STM32_ADC3_SR);
  pending = regval & ADC_SR_ALLINTS;
  if (pending != 0)
    {
      adc_interrupt(&g_adcpriv3);
      regval &= ~pending;
      putreg32(regval, STM32_ADC3_SR);
    }

  return OK;
}
#endif

/****************************************************************************
 * Name: adc123_interrupt
 *
 * Description:
 *   ADC1/2/3 interrupt handler for the STM32 F4 family.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_STM32F40XX
static int adc123_interrupt(int irq, void *context)
{
  uint32_t regval;
  uint32_t pending;

  /* Check for pending ADC1 interrupts */

#ifdef CONFIG_STM32_ADC1
  regval  = getreg32(STM32_ADC1_SR);
  pending = regval & ADC_SR_ALLINTS;
  if (pending != 0)
    {
      adc_interrupt(&g_adcpriv1);
      regval &= ~pending;
      putreg32(regval, STM32_ADC1_SR);
    }
#endif

  /* Check for pending ADC2 interrupts */

#ifdef CONFIG_STM32_ADC2
  regval = getreg32(STM32_ADC2_SR);
  pending = regval & ADC_SR_ALLINTS;
  if (pending != 0)
    {
      adc_interrupt(&g_adcpriv2);
      regval &= ~pending;
      putreg32(regval, STM32_ADC2_SR);
    }
#endif

  /* Check for pending ADC3 interrupts */

#ifdef CONFIG_STM32_ADC3
  regval = getreg32(STM32_ADC3_SR);
  pending = regval & ADC_SR_ALLINTS;
  if (pending != 0)
    {
      adc_interrupt(&g_adcpriv3);
      regval &= ~pending;
      putreg32(regval, STM32_ADC3_SR);
    }
#endif
  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_adcinitialize
 *
 * Description:
 *   Initialize the ADC.  The logic is, save nchannels : # of channels
 *   (conversions) in ADC_SQR1_L
 * Then, take the chanlist array and store it in the SQR Regs, 
 *   chanlist[0] -> ADC_SQR3_SQ1
 *   chanlist[1] -> ADC_SQR3_SQ2
 *   chanlist[2] -> ADC_SQR3_SQ3
 *   chanlist[3] -> ADC_SQR3_SQ4
 *   chanlist[4] -> ADC_SQR3_SQ5
 *   chanlist[5] -> ADC_SQR3_SQ6
 *   ...
 *   chanlist[15]-> ADC_SQR1_SQ16
 *
 * up to
 *   chanlist[nchannels]
 *
 * Input Parameters:
 *   intf      - Could be {1,2,3} for ADC1, ADC2, or ADC3
 *   chanlist  - The list of channels
 *   nchannels - Number of channels
 *
 * Returned Value:
 *   Valid can device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

struct adc_dev_s *stm32_adcinitialize(int intf, uint8_t *chanlist, int nchannels)
{
  FAR struct adc_dev_s   *dev;
  FAR struct stm32_dev_s *priv;
  
#ifdef CONFIG_STM32_ADC1
  if (intf == 1)
    {
      adbg("ADC1 Selected \n");
      dev = &g_adcdev1;
    }
  else
#endif
#ifdef CONFIG_STM32_ADC2
  if (intf == 2)
    {
      adbg("ADC2 Selected \n");
      dev = &g_adcdev2;
    }
  else
#endif
#ifdef CONFIG_STM32_ADC3
  if (intf == 3)
    {
      adbg("ADC3 Selected \n");
      dev = &g_adcdev3;
    }
  else
#endif
    {
      adbg("No ADC interface defined\n");
      return NULL;
    }

  /* Configure the selected ADC */

  priv = dev->ad_priv;
  priv->nchannels = nchannels;
  memcpy(priv->chanlist, chanlist, nchannels);
  return dev;
}

#endif /* CONFIG_STM32_ADC || CONFIG_STM32_ADC2 || CONFIG_STM32_ADC3 */
#endif /* CONFIG_ADC */

