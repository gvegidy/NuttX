/************************************************************
 * irq.h
 *
 *   Copyright (C) 2007 Gregory Nutt. All rights reserved.
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
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
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
 ************************************************************/

/* This file should never be included directed but, rather,
 * only indirectly through nuttx/irq.h
 */

#ifndef __ARCH_C5471_IRQ_H
#define __ARCH_C5471_IRQ_H

/************************************************************
 * Included Files
 ************************************************************/

/************************************************************
 * Definitions
 ************************************************************/

/* IRQ Stack Frame Format:
 *
 * Context is always saved/restored in the same way:
 *
 *   (1) stmia rx, {r0-r14}
 *   (2) then the PC and CPSR
 *
 * This results in the following set of indices that
 * can be used to access individual registers in the
 * xcp.regs array:
 */

#define REG_R0                   (0)
#define REG_R1                   (1)
#define REG_R2                   (2)
#define REG_R3                   (3)
#define REG_R4                   (4)
#define REG_R5                   (5
#define REG_R6                   (6)
#define REG_R7                   (7)
#define REG_R8                   (8)
#define REG_R9                   (9)
#define REG_R10                  (10)
#define REG_R11                  (11)
#define REG_R12                  (12)
#define REG_R13                  (13)
#define REG_R14                  (14)
#define REG_R15                  (15)
#define REG_CPSR                 (16)

#define XCPTCONTEST_REGS         (17)
#define XCPTCONTEXT_SIZE         (4 * XCPTCONTEST_REGS)

#define REG_A1                   REG_R0
#define REG_A2                   REG_R1
#define REG_A3                   REG_R2
#define REG_A4                   REG_R3
#define REG_V1                   REG_R4
#define REG_V2                   REG_R5
#define REG_V3                   REG_R6
#define REG_V4                   REG_R7
#define REG_V5                   REG_R8
#define REG_V6                   REG_R9
#define REG_V7                   REG_R10
#define REG_SB                   REG_R9
#define REG_SL                   REG_R10
#define REG_FP                   REG_R11
#define REG_IP                   REG_R12
#define REG_SP                   REG_R13
#define REG_LR                   REG_R14
#define REG_PC                   REG_R15

/* C5471 Interrupts */

#define C5471_IRQ_TIMER0         0
#define C5471_IRQ_TIMER1         1
#define C5471_IRQ_TIMER2         2
#define C5471_IRQ_GPIO0          3
#define C5471_IRQ_ETHER          4
#define C5471_IRQ_KBGPIO_0_7     5
#define C5471_IRQ_UART           6
#define C5471_IRQ_UART_IRDA      7
#define C5471_IRQ_KBGPIO_8_15    8
#define C5471_IRQ_GPIO3          9
#define C5471_IRQ_GPIO2         10
#define C5471_IRQ_I2C           11
#define C5471_IRQ_GPIO1         12
#define C5471_IRQ_SPI           13
#define C5471_IRQ_GPIO_4_19     14
#define C5471_IRQ_API           15

#define C5471_IRQ_WATCHDOG      C5471_IRQ_TIMER0
#define C5471_IRQ_SYSTIMER      C5471_IRQ_TIMER2
#define NR_IRQS                 (C5471_IRQ_API+1)

/************************************************************
 * Public Types
 ************************************************************/

/* This struct defines the way the registers are stored.  We
 * need to save:
 *
 *  1	CPSR
 *  7	Static registers, v1-v7 (aka r4-r10)
 *  1	Frame pointer, fp (aka r11)
 *  1	Stack pointer, sp (aka r13)
 *  1	Return address, lr (aka r14)
 * ---
 * 11	(XCPTCONTEXT_USER_REG)
 *
 * On interrupts, we also need to save:
 *  4	Volatile registers, a1-a4 (aka r0-r3)
 *  1	Scratch Register, ip (aka r12)
 *---
 *  5	(XCPTCONTEXT_IRQ_REGS)
 *
 * For a total of 17 (XCPTCONTEST_REGS)
 */

#ifndef __ASSEMBLY__
struct xcptcontext
{
  /* The following function pointer is non-zero if there
   * are pending signals to be processed.
   */

  void *sigdeliver; /* Actual type is sig_deliver_t */

  /* These are saved copies of LR and CPSR used during
   * signal processing.
   */

  uint32 saved_pc;
  uint32 saved_cpsr;

  /* Register save area */

  uint32 regs[XCPTCONTEST_REGS];
};
#endif

/************************************************************
 * Inline functions
 ************************************************************/

#ifndef __ASSEMBLY__

/* Save the current interrupt enable state & disable IRQs */

static inline irqstate_t irqsave(void)
{
  unsigned int flags;
  unsigned int temp;
  __asm__ __volatile__
    (
     "\tmrs    %0, cpsr\n"
     "\torr    %1, %0, #128\n"
     "\tmsr    cpsr_c, %1"
     : "=r" (flags), "=r" (temp)
     :
     : "memory");
  return flags;
}

/* Restore saved IRQ & FIQ state */

static inline void irqrestore(irqstate_t flags)
{
  __asm__ __volatile__
    (
     "msr    cpsr_c, %0"
     :
     : "r" (flags)
     : "memory");
}

static inline void system_call(swint_t func, int parm1,
			       int parm2, int parm3)
{
  __asm__ __volatile__
    (
     "mov\tr0,%0\n\t"
     "mov\tr1,%1\n\t"
     "mov\tr2,%2\n\t"
     "mov\tr2,%3\n\t"
     "swi\t0x900001\n\t"
     :
     : "r" ((long)(func)),  "r" ((long)(parm1)),
       "r" ((long)(parm2)), "r" ((long)(parm3))
     : "r0", "r1", "r2", "r3", "lr");
}
#endif

/************************************************************
 * Public Variables
 ************************************************************/

/************************************************************
 * Public Function Prototypes
 ************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_C5471_IRQ_H */

