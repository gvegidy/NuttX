/********************************************************************************************
 * arch/mips/src/pic32mx/pic32mx-ethernet.h
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
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
 ********************************************************************************************/

#ifndef __ARCH_MIPS_SRC_PIC32MX_PIC32MX_ETHERNET_H
#define __ARCH_MIPS_SRC_PIC32MX_PIC32MX_ETHERNET_H

/********************************************************************************************
 * Included Files
 ********************************************************************************************/

#include <nuttx/config.h>

#include "pic32mx-memorymap.h"

/********************************************************************************************
 * Pre-Processor Definitions
 ********************************************************************************************/
/* Register Offsets *************************************************************************/

/* Controller and DMA Engine Configuration/Status Registers */

#define PIC32MX_ETH_CON1_OFFSET        0x0000  /* Ethernet Controller Control 1 Register */
#define PIC32MX_ETH_CON1SET_OFFSET     0x0004
#define PIC32MX_ETH_CON1CLR_OFFSET     0x0008
#define PIC32MX_ETH_CON1INV_OFFSET     0x000c
#define PIC32MX_ETH_CON2_OFFSET        0x0010  /* Ethernet Controller Control 2 Register */
#define PIC32MX_ETH_CON2SET_OFFSET     0x0014
#define PIC32MX_ETH_CON2CLR_OFFSET     0x0018
#define PIC32MX_ETH_CON2INV_OFFSET     0x001c
#define PIC32MX_ETH_TXST_OFFSET        0x0020  /* Ethernet Controller TX Packet Descriptor Start Address Register */
#define PIC32MX_ETH_TXSTSET_OFFSET     0x0024
#define PIC32MX_ETH_TXSTCLR_OFFSET     0x0028
#define PIC32MX_ETH_TXSTINV_OFFSET     0x002c
#define PIC32MX_ETH_RXST_OFFSET        0x0030  /* Ethernet Controller RX Packet Descriptor Start Address Register */
#define PIC32MX_ETH_RXSTSET_OFFSET     0x0034
#define PIC32MX_ETH_RXSTCLR_OFFSET     0x0038
#define PIC32MX_ETH_RXSTINV_OFFSET     0x003c

#define PIC32MX_ETH_IEN_OFFSET         0x00c0  /* Ethernet Controller Interrupt Enable Register */
#define PIC32MX_ETH_IENSET_OFFSET      0x00c4
#define PIC32MX_ETH_IENCLR_OFFSET      0x00c8
#define PIC32MX_ETH_IENINV_OFFSET      0x00cc
#define PIC32MX_ETH_IRQ_OFFSET         0x00d0  /* Ethernet Controller Interrupt Request Register */
#define PIC32MX_ETH_IENSET_OFFSET      0x00d4
#define PIC32MX_ETH_IENCLR_OFFSET      0x00d8
#define PIC32MX_ETH_IENINV_OFFSET      0x00dc
#define PIC32MX_ETH_STAT_OFFSET        0x00e0  /* Ethernet Controller Status Register */

/* RX Filtering Configuration Registers */

#define PIC32MX_ETH_RXFC_OFFSET        0x00a0  /* Ethernet Controller Receive Filter Configuration Register */
#define PIC32MX_ETH_RXFCSET_OFFSET     0x00a4
#define PIC32MX_ETH_RXFCCLR_OFFSET     0x00a8
#define PIC32MX_ETH_RXFCINV_OFFSET     0x00ac

#define PIC32MX_ETH_HT0_OFFSET         0x0040  /* Ethernet Controller Hash Table 0 Register */
#define PIC32MX_ETH_HT0SET_OFFSET      0x0044
#define PIC32MX_ETH_HT0CLR_OFFSET      0x0048
#define PIC32MX_ETH_HT0INV_OFFSET      0x004c
#define PIC32MX_ETH_HT1_OFFSET         0x0050  /* Ethernet Controller Hash Table 1 Register */
#define PIC32MX_ETH_HT1SET_OFFSET      0x0054
#define PIC32MX_ETH_HT1CLR_OFFSET      0x0058
#define PIC32MX_ETH_HT1INV_OFFSET      0x005c
#define PIC32MX_ETH_PMM0_OFFSET        0x0060  /* Ethernet Controller Pattern Match Mask 0 Register */
#define PIC32MX_ETH_PMM0SET_OFFSET     0x0064
#define PIC32MX_ETH_PMM0CLR_OFFSET     0x0068
#define PIC32MX_ETH_PMM0INV_OFFSET     0x006c
#define PIC32MX_ETH_PMM1_OFFSET        0x0070  /* Ethernet Controller Pattern Match Mask 1 Register */
#define PIC32MX_ETH_PMM1SET_OFFSET     0x0074
#define PIC32MX_ETH_PMM1CLR_OFFSET     0x0078
#define PIC32MX_ETH_PMM1INV_OFFSET     0x007c
#define PIC32MX_ETH_PMCS_OFFSET        0x0080  /* Ethernet Controller Pattern Match Checksum Register */
#define PIC32MX_ETH_PMCSSET_OFFSET     0x0084
#define PIC32MX_ETH_PMCSCLR_OFFSET     0x0088
#define PIC32MX_ETH_PMCSINV_OFFSET     0x008c
#define PIC32MX_ETH_PMO_OFFSET         0x0090  /* Ethernet Controller Pattern Match Offset Register */
#define PIC32MX_ETH_PMOSET_OFFSET      0x0094
#define PIC32MX_ETH_PMOCLR_OFFSET      0x0098
#define PIC32MX_ETH_PMOINV_OFFSET      0x009c

/* Flow Control Configuring Register */

#define PIC32MX_ETH_RXWM_OFFSET        0x00b0  /* Ethernet Controller Receive Watermarks Register */
#define PIC32MX_ETH_RXWMSET_OFFSET     0x00b4
#define PIC32MX_ETH_RXWMCLR_OFFSET     0x00b8
#define PIC32MX_ETH_RXWMINV_OFFSET     0x00bc

/* Ethernet Statistics Registers */

#define PIC32MX_ETH_RXOVFLOW_OFFSET    0x0100  /* Ethernet Controller Receive Overflow Statistics Register */
#define PIC32MX_ETH_RXOVFLOWSET_OFFSET 0x0104
#define PIC32MX_ETH_RXOVFLOWCLR_OFFSET 0x0108
#define PIC32MX_ETH_RXOVFLOWINV_OFFSET 0x010c
#define PIC32MX_ETH_FRMTXOK_OFFSET     0x0110  /* Ethernet Controller Frames Transmitted OK Statistics Register */
#define PIC32MX_ETH_FRMTXOKSET_OFFSET  0x0114
#define PIC32MX_ETH_FRMTXOKCLR_OFFSET  0x0118
#define PIC32MX_ETH_FRMTXOKINV_OFFSET  0x011c
#define PIC32MX_ETH_SCOLFRM_OFFSET     0x0120  /* Ethernet Controller Single Collision Frames Statistics Register */
#define PIC32MX_ETH_SCOLFRMSET_OFFSET  0x0124
#define PIC32MX_ETH_SCOLFRMCLR_OFFSET  0x0128
#define PIC32MX_ETH_SCOLFRMINV_OFFSET  0x012c
#define PIC32MX_ETH_MCOLFRM_OFFSET     0x0130  /* Ethernet Controller Multiple Collision Frames Statistics Register */
#define PIC32MX_ETH_MCOLFRMSET_OFFSET  0x0134
#define PIC32MX_ETH_MCOLFRMCLR_OFFSET  0x0138
#define PIC32MX_ETH_MCOLFRMINV_OFFSET  0x013c
#define PIC32MX_ETH_FRMRXOK_OFFSET     0x0140  /* Ethernet Controller Frames Received OK Statistics Register */
#define PIC32MX_ETH_FRMRXOKSET_OFFSET  0x0144
#define PIC32MX_ETH_FRMRXOKCLR_OFFSET  0x0148
#define PIC32MX_ETH_FRMRXOKINV_OFFSET  0x014c
#define PIC32MX_ETH_FCSERR_OFFSET      0x0150  /* Ethernet Controller Frame Check Sequence Error Statistics Register */
#define PIC32MX_ETH_FCSERRSET_OFFSET   0x0154
#define PIC32MX_ETH_FCSERRCLR_OFFSET   0x0158
#define PIC32MX_ETH_FCSERRINV_OFFSET   0x015c
#define PIC32MX_ETH_ALGNERR_OFFSET     0x0160  /* Ethernet Controller Alignment Errors Statistics Register */
#define PIC32MX_ETH_ALGNERRSET_OFFSET  0x0164
#define PIC32MX_ETH_ALGNERRCLR_OFFSET  0x0168
#define PIC32MX_ETH_ALGNERRINV_OFFSET  0x016c

/* MAC Configuration Registers */

#define PIC32MX_EMAC1_CFG1_OFFSET      0x0200  /* Ethernet Controller MAC Configuration 1 Register */
#define PIC32MX_EMAC1_CFG1SET_OFFSET   0x0204
#define PIC32MX_EMAC1_CFG1CLR_OFFSET   0x0208
#define PIC32MX_EMAC1_CFG1INV_OFFSET   0x020c
#define PIC32MX_EMAC1_CFG2_OFFSET      0x0210  /* Ethernet Controller MAC Configuration 2 Register */
#define PIC32MX_EMAC1_CFG2SET_OFFSET   0x0214
#define PIC32MX_EMAC1_CFG2CLR_OFFSET   0x0218
#define PIC32MX_EMAC1_CFG2INV_OFFSET   0x021c
#define PIC32MX_EMAC1_IPGT_OFFSET      0x0220  /* Ethernet Controller MAC Back-to-Back Interpacket Gap Register */
#define PIC32MX_EMAC1_IPGTSET_OFFSET   0x0224
#define PIC32MX_EMAC1_IPGTCLR_OFFSET   0x0228
#define PIC32MX_EMAC1_IPGTINV_OFFSET   0x022c
#define PIC32MX_EMAC1_IPGR_OFFSET      0x0230  /* Ethernet Controller MAC Non-Back-to-Back Interpacket Gap Register */
#define PIC32MX_EMAC1_IPGRSET_OFFSET   0x0234
#define PIC32MX_EMAC1_IPGRCLR_OFFSET   0x0238
#define PIC32MX_EMAC1_IPGRINV_OFFSET   0x023c
#define PIC32MX_EMAC1_CLRT_OFFSET      0x0240  /* Ethernet Controller MAC Collision Window/Retry Limit Register */
#define PIC32MX_EMAC1_CLRTSET_OFFSET   0x0244
#define PIC32MX_EMAC1_CLRTCLR_OFFSET   0x0248
#define PIC32MX_EMAC1_CLRTINV_OFFSET   0x024c
#define PIC32MX_EMAC1_MAXF_OFFSET      0x0250  /* Ethernet Controller MAC Maximum Frame Length Register */
#define PIC32MX_EMAC1_MAXFSET_OFFSET   0x0254
#define PIC32MX_EMAC1_MAXFCLR_OFFSET   0x0258
#define PIC32MX_EMAC1_MAXFINV_OFFSET   0x025c
#define PIC32MX_EMAC1_SUPP_OFFSET      0x0260  /* Ethernet Controller MAC PHY Support Register */
#define PIC32MX_EMAC1_SUPPSET_OFFSET   0x0264
#define PIC32MX_EMAC1_SUPPCLR_OFFSET   0x0268
#define PIC32MX_EMAC1_SUPPINV_OFFSET   0x026c
#define PIC32MX_EMAC1_TEST_OFFSET      0x0270  /* Ethernet Controller MAC Test Register */
#define PIC32MX_EMAC1_TESTSET_OFFSET   0x0274
#define PIC32MX_EMAC1_TESTCLR_OFFSET   0x0278
#define PIC32MX_EMAC1_TESTINV_OFFSET   0x027c

#define PIC32MX_EMAC1_SA0_OFFSET       0x0300  /* Ethernet Controller MAC Station Address 0 Register */
#define PIC32MX_EMAC1_SA0SET_OFFSET    0x0304
#define PIC32MX_EMAC1_SA0CLR_OFFSET    0x0308
#define PIC32MX_EMAC1_SA0INV_OFFSET    0x030c
#define PIC32MX_EMAC1_SA1_OFFSET       0x0310  /* Ethernet Controller MAC Station Address 1 Register */
#define PIC32MX_EMAC1_SA1SET_OFFSET    0x0314
#define PIC32MX_EMAC1_SA1CLR_OFFSET    0x0318
#define PIC32MX_EMAC1_SA1INV_OFFSET    0x031c
#define PIC32MX_EMAC1_SA2_OFFSET       0x0320  /* Ethernet Controller MAC Station Address 2 Register */
#define PIC32MX_EMAC1_SA2SET_OFFSET    0x0324
#define PIC32MX_EMAC1_SA2CLR_OFFSET    0x0328
#define PIC32MX_EMAC1_SA2INV_OFFSET    0x032c

/* MII Management Registers */

#define PIC32MX_EMAC1_MCFG_OFFSET      0x0280  /* Ethernet Controller MAC MII Management Configuration Register */
#define PIC32MX_EMAC1_MCFGSET_OFFSET   0x0284
#define PIC32MX_EMAC1_MCFGCLR_OFFSET   0x0288
#define PIC32MX_EMAC1_MCFGINV_OFFSET   0x028c
#define PIC32MX_EMAC1_MCMD_OFFSET      0x0290  /* Ethernet Controller MAC MII Management Command Register */
#define PIC32MX_EMAC1_MCMDSET_OFFSET   0x0294
#define PIC32MX_EMAC1_MCMDCLR_OFFSET   0x0298
#define PIC32MX_EMAC1_MCMDINV_OFFSET   0x029c
#define PIC32MX_EMAC1_MADR_OFFSET      0x02a0  /* Ethernet Controller MAC MII Management Address Register */
#define PIC32MX_EMAC1_MADRSET_OFFSET   0x02a4
#define PIC32MX_EMAC1_MADRCLR_OFFSET   0x02a8
#define PIC32MX_EMAC1_MADRINV_OFFSET   0x02ac
#define PIC32MX_EMAC1_MWTD_OFFSET      0x02b0  /* Ethernet Controller MAC MII Management Write Data Register */
#define PIC32MX_EMAC1_MWTDSET_OFFSET   0x02b4
#define PIC32MX_EMAC1_MWTDCLR_OFFSET   0x02b8
#define PIC32MX_EMAC1_MWTDINV_OFFSET   0x02bc
#define PIC32MX_EMAC1_MRDD_OFFSET      0x02c0  /* Ethernet Controller MAC MII Management Read Data Register */
#define PIC32MX_EMAC1_MRDDSET_OFFSET   0x02c4
#define PIC32MX_EMAC1_MRDDCLR_OFFSET   0x02c8
#define PIC32MX_EMAC1_MRDDINV_OFFSET   0x02cc
#define PIC32MX_EMAC1_MIND_OFFSET      0x02d0  /* Ethernet Controller MAC MII Management Indicators Register */
#define PIC32MX_EMAC1_MINDSET_OFFSET   0x02d4
#define PIC32MX_EMAC1_MINDCLR_OFFSET   0x02d8
#define PIC32MX_EMAC1_MINDINV_OFFSET   0x02dc

/* Register Addresses ***********************************************************************/

/* Controller and DMA Engine Configuration/Status Registers */

#define PIC32MX_ETH_CON1               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON1_OFFSET)
#define PIC32MX_ETH_CON1SET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON1SET_OFFSET)
#define PIC32MX_ETH_CON1CLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON1CLR_OFFSET)
#define PIC32MX_ETH_CON1INV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON1INV_OFFSET)
#define PIC32MX_ETH_CON2               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON2_OFFSET)
#define PIC32MX_ETH_CON2SET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON2SET_OFFSET)
#define PIC32MX_ETH_CON2CLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON2CLR_OFFSET)
#define PIC32MX_ETH_CON2INV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_CON2INV_OFFSET)
#define PIC32MX_ETH_TXST               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_TXST_OFFSET)
#define PIC32MX_ETH_TXSTSET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_TXSTSET_OFFSET)
#define PIC32MX_ETH_TXSTCLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_TXSTCLR_OFFSET)
#define PIC32MX_ETH_TXSTINV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_TXSTINV_OFFSET)
#define PIC32MX_ETH_RXST               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXST_OFFSET)
#define PIC32MX_ETH_RXSTSET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXSTSET_OFFSET)
#define PIC32MX_ETH_RXSTCLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXSTCLR_OFFSET)
#define PIC32MX_ETH_RXSTINV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXSTINV_OFFSET)
#define PIC32MX_ETH_IEN                (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IEN_OFFSET)
#define PIC32MX_ETH_IENSET             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IENSET_OFFSET)
#define PIC32MX_ETH_IENCLR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IENCLR_OFFSET)
#define PIC32MX_ETH_IENINV             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IENINV_OFFSET)
#define PIC32MX_ETH_IRQ                (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IRQ_OFFSET)
#define PIC32MX_ETH_IENSET             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IENSET_OFFSET)
#define PIC32MX_ETH_IENCLR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IENCLR_OFFSET)
#define PIC32MX_ETH_IENINV             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_IENINV_OFFSET)
#define PIC32MX_ETH_STAT               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_STAT_OFFSET)

/* RX Filtering Configuration Registers */

#define PIC32MX_ETH_RXFC               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXFC_OFFSET)
#define PIC32MX_ETH_RXFCSET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXFCSET_OFFSET)
#define PIC32MX_ETH_RXFCCLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXFCCLR_OFFSET)
#define PIC32MX_ETH_RXFCINV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXFCINV_OFFSET)
#define PIC32MX_ETH_HT0                (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT0_OFFSET)
#define PIC32MX_ETH_HT0SET             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT0SET_OFFSET)
#define PIC32MX_ETH_HT0CLR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT0CLR_OFFSET)
#define PIC32MX_ETH_HT0INV             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT0INV_OFFSET)
#define PIC32MX_ETH_HT1                (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT1_OFFSET)
#define PIC32MX_ETH_HT1SET             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT1SET_OFFSET)
#define PIC32MX_ETH_HT1CLR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT1CLR_OFFSET)
#define PIC32MX_ETH_HT1INV             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_HT1INV_OFFSET)
#define PIC32MX_ETH_PMM0               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM0_OFFSET)
#define PIC32MX_ETH_PMM0SET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM0SET_OFFSET)
#define PIC32MX_ETH_PMM0CLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM0CLR_OFFSET)
#define PIC32MX_ETH_PMM0INV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM0INV_OFFSET)
#define PIC32MX_ETH_PMM1               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM1_OFFSET)
#define PIC32MX_ETH_PMM1SET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM1SET_OFFSET)
#define PIC32MX_ETH_PMM1CLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM1CLR_OFFSET)
#define PIC32MX_ETH_PMM1INV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMM1INV_OFFSET)
#define PIC32MX_ETH_PMCS               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMCS_OFFSET)
#define PIC32MX_ETH_PMCSSET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMCSSET_OFFSET)
#define PIC32MX_ETH_PMCSCLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMCSCLR_OFFSET)
#define PIC32MX_ETH_PMCSINV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMCSINV_OFFSET)
#define PIC32MX_ETH_PMO                (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMO_OFFSET)
#define PIC32MX_ETH_PMOSET             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMOSET_OFFSET)
#define PIC32MX_ETH_PMOCLR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMOCLR_OFFSET)
#define PIC32MX_ETH_PMOINV             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_PMOINV_OFFSET)

/* Flow Control Configuring Register */

#define PIC32MX_ETH_RXWM               (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXWM_OFFSET)
#define PIC32MX_ETH_RXWMSET            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXWMSET_OFFSET)
#define PIC32MX_ETH_RXWMCLR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXWMCLR_OFFSET)
#define PIC32MX_ETH_RXWMINV            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXWMINV_OFFSET)

/* Ethernet Statistics Registers */

#define PIC32MX_ETH_RXOVFLOW           (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXOVFLOW_OFFSET)
#define PIC32MX_ETH_RXOVFLOWSET        (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXOVFLOWSET_OFFSET)
#define PIC32MX_ETH_RXOVFLOWCLR        (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXOVFLOWCLR_OFFSET)
#define PIC32MX_ETH_RXOVFLOWINV        (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_RXOVFLOWINV_OFFSET)
#define PIC32MX_ETH_FRMTXOK            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMTXOK_OFFSET)
#define PIC32MX_ETH_FRMTXOKSET         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMTXOKSET_OFFSET)
#define PIC32MX_ETH_FRMTXOKCLR         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMTXOKCLR_OFFSET)
#define PIC32MX_ETH_FRMTXOKINV         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMTXOKINV_OFFSET)
#define PIC32MX_ETH_SCOLFRM            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_SCOLFRM_OFFSET)
#define PIC32MX_ETH_SCOLFRMSET         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_SCOLFRMSET_OFFSET)
#define PIC32MX_ETH_SCOLFRMCLR         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_SCOLFRMCLR_OFFSET)
#define PIC32MX_ETH_SCOLFRMINV         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_SCOLFRMINV_OFFSET)
#define PIC32MX_ETH_MCOLFRM            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_MCOLFRM_OFFSET)
#define PIC32MX_ETH_MCOLFRMSET         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_MCOLFRMSET_OFFSET)
#define PIC32MX_ETH_MCOLFRMCLR         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_MCOLFRMCLR_OFFSET)
#define PIC32MX_ETH_MCOLFRMINV         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_MCOLFRMINV_OFFSET)
#define PIC32MX_ETH_FRMRXOK            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMRXOK_OFFSET)
#define PIC32MX_ETH_FRMRXOKSET         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMRXOKSET_OFFSET)
#define PIC32MX_ETH_FRMRXOKCLR         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMRXOKCLR_OFFSET)
#define PIC32MX_ETH_FRMRXOKINV         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FRMRXOKINV_OFFSET)
#define PIC32MX_ETH_FCSERR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FCSERR_OFFSET)
#define PIC32MX_ETH_FCSERRSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FCSERRSET_OFFSET)
#define PIC32MX_ETH_FCSERRCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FCSERRCLR_OFFSET)
#define PIC32MX_ETH_FCSERRINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_FCSERRINV_OFFSET)
#define PIC32MX_ETH_ALGNERR            (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_ALGNERR_OFFSET)
#define PIC32MX_ETH_ALGNERRSET         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_ALGNERRSET_OFFSET)
#define PIC32MX_ETH_ALGNERRCLR         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_ALGNERRCLR_OFFSET)
#define PIC32MX_ETH_ALGNERRINV         (PIC32MX_ETHERNET_K1BASE+PIC32MX_ETH_ALGNERRINV_OFFSET)

/* MAC Configuration Registers */

#define PIC32MX_EMAC1_CFG1             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG1_OFFSET)
#define PIC32MX_EMAC1_CFG1SET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG1SET_OFFSET)
#define PIC32MX_EMAC1_CFG1CLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG1CLR_OFFSET)
#define PIC32MX_EMAC1_CFG1INV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG1INV_OFFSET)
#define PIC32MX_EMAC1_CFG2             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG2_OFFSET)
#define PIC32MX_EMAC1_CFG2SET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG2SET_OFFSET)
#define PIC32MX_EMAC1_CFG2CLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG2CLR_OFFSET)
#define PIC32MX_EMAC1_CFG2INV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CFG2INV_OFFSET)
#define PIC32MX_EMAC1_IPGT             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGT_OFFSET)
#define PIC32MX_EMAC1_IPGTSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGTSET_OFFSET)
#define PIC32MX_EMAC1_IPGTCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGTCLR_OFFSET)
#define PIC32MX_EMAC1_IPGTINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGTINV_OFFSET)
#define PIC32MX_EMAC1_IPGR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGR_OFFSET)
#define PIC32MX_EMAC1_IPGRSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGRSET_OFFSET)
#define PIC32MX_EMAC1_IPGRCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGRCLR_OFFSET)
#define PIC32MX_EMAC1_IPGRINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_IPGRINV_OFFSET)
#define PIC32MX_EMAC1_CLRT             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CLRT_OFFSET)
#define PIC32MX_EMAC1_CLRTSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CLRTSET_OFFSET)
#define PIC32MX_EMAC1_CLRTCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CLRTCLR_OFFSET)
#define PIC32MX_EMAC1_CLRTINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_CLRTINV_OFFSET)
#define PIC32MX_EMAC1_MAXF             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MAXF_OFFSET)
#define PIC32MX_EMAC1_MAXFSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MAXFSET_OFFSET)
#define PIC32MX_EMAC1_MAXFCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MAXFCLR_OFFSET)
#define PIC32MX_EMAC1_MAXFINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MAXFINV_OFFSET)
#define PIC32MX_EMAC1_SUPP             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SUPP_OFFSET)
#define PIC32MX_EMAC1_SUPPSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SUPPSET_OFFSET)
#define PIC32MX_EMAC1_SUPPCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SUPPCLR_OFFSET)
#define PIC32MX_EMAC1_SUPPINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SUPPINV_OFFSET)
#define PIC32MX_EMAC1_TEST             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_TEST_OFFSET)
#define PIC32MX_EMAC1_TESTSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_TESTSET_OFFSET)
#define PIC32MX_EMAC1_TESTCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_TESTCLR_OFFSET)
#define PIC32MX_EMAC1_TESTINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_TESTINV_OFFSET)
#define PIC32MX_EMAC1_SA0              (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA0_OFFSET)
#define PIC32MX_EMAC1_SA0SET           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA0SET_OFFSET)
#define PIC32MX_EMAC1_SA0CLR           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA0CLR_OFFSET)
#define PIC32MX_EMAC1_SA0INV           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA0INV_OFFSET)
#define PIC32MX_EMAC1_SA1              (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA1_OFFSET)
#define PIC32MX_EMAC1_SA1SET           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA1SET_OFFSET)
#define PIC32MX_EMAC1_SA1CLR           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA1CLR_OFFSET)
#define PIC32MX_EMAC1_SA1INV           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA1INV_OFFSET)
#define PIC32MX_EMAC1_SA2              (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA2_OFFSET)
#define PIC32MX_EMAC1_SA2SET           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA2SET_OFFSET)
#define PIC32MX_EMAC1_SA2CLR           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA2CLR_OFFSET)
#define PIC32MX_EMAC1_SA2INV           (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_SA2INV_OFFSET)

/* MII Management Registers */

#define PIC32MX_EMAC1_MCFG             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCFG_OFFSET)
#define PIC32MX_EMAC1_MCFGSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCFGSET_OFFSET)
#define PIC32MX_EMAC1_MCFGCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCFGCLR_OFFSET)
#define PIC32MX_EMAC1_MCFGINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCFGINV_OFFSET)
#define PIC32MX_EMAC1_MCMD             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCMD_OFFSET)
#define PIC32MX_EMAC1_MCMDSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCMDSET_OFFSET)
#define PIC32MX_EMAC1_MCMDCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCMDCLR_OFFSET)
#define PIC32MX_EMAC1_MCMDINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MCMDINV_OFFSET)
#define PIC32MX_EMAC1_MADR             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MADR_OFFSET)
#define PIC32MX_EMAC1_MADRSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MADRSET_OFFSET)
#define PIC32MX_EMAC1_MADRCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MADRCLR_OFFSET)
#define PIC32MX_EMAC1_MADRINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MADRINV_OFFSET)
#define PIC32MX_EMAC1_MWTD             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MWTD_OFFSET)
#define PIC32MX_EMAC1_MWTDSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MWTDSET_OFFSET)
#define PIC32MX_EMAC1_MWTDCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MWTDCLR_OFFSET)
#define PIC32MX_EMAC1_MWTDINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MWTDINV_OFFSET)
#define PIC32MX_EMAC1_MRDD             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MRDD_OFFSET)
#define PIC32MX_EMAC1_MRDDSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MRDDSET_OFFSET)
#define PIC32MX_EMAC1_MRDDCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MRDDCLR_OFFSET)
#define PIC32MX_EMAC1_MRDDINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MRDDINV_OFFSET )
#define PIC32MX_EMAC1_MIND             (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MIND_OFFSET)
#define PIC32MX_EMAC1_MINDSET          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MINDSET_OFFSET)
#define PIC32MX_EMAC1_MINDCLR          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MINDCLR_OFFSET)
#define PIC32MX_EMAC1_MINDINV          (PIC32MX_ETHERNET_K1BASE+PIC32MX_EMAC1_MINDINV_OFFSET)

/* Register Bit-Field Definitions ***********************************************************/

/* Controller and DMA Engine Configuration/Status Registers */

/* Ethernet Controller Control 1 Register */
#define ETH_CON1_
/* Ethernet Controller Control 2 Register */
#define ETH_CON2_
/* Ethernet Controller TX Packet Descriptor Start Address Register */
#define ETH_TXST_
/* Ethernet Controller RX Packet Descriptor Start Address Register */
#define ETH_RXST_
/* Ethernet Controller Interrupt Enable Register */
#define ETH_IEN_
/* Ethernet Controller Interrupt Request Register */
#define ETH_IRQ_
/* Ethernet Controller Status Register */
#define ETH_STAT_

/* RX Filtering Configuration Registers */

/* Ethernet Controller Receive Filter Configuration Register */
#define ETH_RXFC_
/* Ethernet Controller Hash Table 0 Register */
#define ETH_HT0_
/* Ethernet Controller Hash Table 1 Register */
#define ETH_HT1_
/* Ethernet Controller Pattern Match Mask 0 Register */
#define ETH_PMM0_
/* Ethernet Controller Pattern Match Mask 1 Register */
#define ETH_PMM1_
/* Ethernet Controller Pattern Match Checksum Register */
#define ETH_PMCS_
/* Ethernet Controller Pattern Match Offset Register */
#define ETH_PMO_

/* Flow Control Configuring Register */

/* Ethernet Controller Receive Watermarks Register */
#define ETH_RXWM_

/* Ethernet Statistics Registers */

/* Ethernet Controller Receive Overflow Statistics Register */
#define ETH_RXOVFLOW_
/* Ethernet Controller Frames Transmitted OK Statistics Register */
#define ETH_FRMTXOK_
/* Ethernet Controller Single Collision Frames Statistics Register */
#define ETH_SCOLFRM_
/* Ethernet Controller Multiple Collision Frames Statistics Register */
#define ETH_MCOLFRM_
/* Ethernet Controller Frames Received OK Statistics Register */
#define ETH_FRMRXOK_
/* Ethernet Controller Frame Check Sequence Error Statistics Register */
#define ETH_FCSERR_
/* Ethernet Controller Alignment Errors Statistics Register */
#define ETH_ALGNERR_

/* MAC Configuration Registers */

/* Ethernet Controller MAC Configuration 1 Register */

#define EMAC1_CFG1_RXEN                (1 << 0)  /* Bit 0:  MAC Receive enable */
#define EMAC1_CFG1_PASSALL             (1 << 1)  /* Bit 1:  MAC Pass all all receive frames */
#define EMAC1_CFG1_PXPAUSE             (1 << 2)  /* Bit 2:  MAC RX flow control bit */
#define EMAC1_CFG1_TXPAUSE             (1 << 3)  /* Bit 3:  MAC TX flow control */
#define EMAC1_CFG1_LOOPBACK            (1 << 4)  /* Bit 4:  MAC loopback mode */
                                                 /* Bits 5-7: Reserved */
#define EMAC1_CFG1_TXRST               (1 << 8)  /* Bit 8:  Reset TX function */
#define EMAC1_CFG1_MCSTXRST            (1 << 9)  /* Bit 9:  Reset MCS/TX */
#define EMAC1_CFG1_RXRST               (1 << 10) /* Bit 10: Reset RX */
#define EMAC1_CFG1_MCSRXRST            (1 << 11) /* Bit 11: Reset MCS/RX */
                                                 /* Bits 12-13: Reserved */
#define EMAC1_CFG1_SIMRST              (1 << 14) /* Bit 14: Simulation reset */
#define EMAC1_CFG1_SOFTRST             (1 << 15) /* Bit 15: Soft reset */
                                                 /* Bits 16-31: Reserved */

/* Ethernet Controller MAC Configuration 2 Register */

#define EMAC1_CFG2_FULLDPLX:           (1 << 0)  /* Bit 0:  Full duplex operation */
#define EMAC1_CFG2_LENGTHCK            (1 << 1)  /* Bit 1:  Frame length checking */
#define EMAC1_CFG2_HUGEFRM             (1 << 2)  /* Bit 2:  Huge frame enable */
#define EMAC1_CFG2_DELAYCRC            (1 << 3)  /* Bit 3:  Delayed CRC */
#define EMAC1_CFG2_CRCEN               (1 << 4)  /* Bit 4:  CRC enable */
#define EMAC1_CFG2_PADCRCEN            (1 << 5)  /* Bit 5:  Pad/CRC enable */
#define EMAC1_CFG2_VLANPADEN           (1 << 6)  /* Bit 6:  VLAN pad enable */
#define EMAC1_CFG2_AUTOPADEN           (1 << 7)  /* Bit 7:  Auto detect pad enable */
#define EMAC1_CFG1_PUREPRE             (1 << 8)  /* Bit 8:  Pure preamble enforcement */
#define EMAC1_CFG1_LONGPRE             (1 << 9)  /* Bit 9:  Long preamble enforcement */
                                                 /* Bits 10-11: Reserved */
#define EMAC1_CFG2_NOBKOFF             (1 << 12) /* Bit 12: No backoff */
#define EMAC1_CFG2_BPNOBKOFF           (1 << 13) /* Bit 13: Back pressure/no backoff */
#define EMAC1_CFG2_EXCESSDFR           (1 << 14) /* Bit 14: Excess defer */
                                                 /* Bits 15-31: Reserved */

/* Ethernet Controller MAC Back-to-Back Interpacket Gap Register */

#define EMAC1_IPGT_SHIFT               (0)       /* Bits 0-6 */
#define EMAC1_IPGT_MASK                (0x7f << EMAC1_IPGT_SHIFT)
                                                 /* Bits 7-31: Reserved */
/* Ethernet Controller MAC Non-Back-to-Back Interpacket Gap Register */

#define EMAC1_IPGR_GAP2_SHIFT          (0)       /* Bits 0-6: Gap part 2 */
#define EMAC1_IPGR_GAP2_MASK           (0x7f << EMAC1_IPGR_GAP2_SHIFT)
                                                 /* Bit 7: Reserved */
#define EMAC1_IPGR_GAP1_SHIFT          (8)       /* Bits 8-18: Gap part 1 */
#define EMAC1_IPGR_GAP1_MASK           (0x7f << EMAC1_IPGR_GAP2_SHIFT)
                                                 /* Bits 15-31: Reserved */
/* Ethernet Controller MAC Collision Window/Retry Limit Register */

#define EMAC1_CLRT_RETX_SHIFT          (0)       /* Bits 0-3: Retransmission maximum */
#define EMAC1_CLRT_RETX_MASK           (15 << EMAC1_CLRT_RETX_SHIFT)
                                                 /* Bits 4-7: Reserved */
#define EMAC1_CLRT_CWINDOW_SHIFT       (8)       /* Bits 8-13: Collision window */
#define EMAC1_CLRT_CWINDOW_MASK        (0x3f << EMAC1_CLRT_CWINDOW_SHIFT)
                                                 /* Bits 14-31: Reserved */
/* Ethernet Controller MAC Maximum Frame Length Register */

#define EMAC1_MAXF_SHIFT               (0)       /* Bits 0-15 */
#define EMAC1_MAXF_MASK                (0xffff << EMAC1_MAXF_SHIFT)
                                                 /* Bits 16-31: Reserved */
/* Ethernet Controller MAC PHY Support Register */
#define EMAC1_SUPP_
                                                 /* Bits 0-7: Reserved */
#define EMAC1_SUPP_SPEEDRMII           (1 << 8)  /* Bit 8:  RMII Speed0=10Bps 1=100Bps */
                                                 /* Bits 9-10: Reserved */
#define EMAC1_SUPP_RESETRMII           (1 << 11) /* Bit 11: Reset RMII Logic */
                                                 /* Bits 12-31: Reserved */
/* Ethernet Controller MAC Test Register */

#define EMAC1_TEST_SHRTQNTA            (1 << 0)  /* Bit 0:  Shortcut pause quanta */
#define EMAC1_TEST_TESTPAUSE           (1 << 1)  /* Bit 1:  Test pause */
#define EMAC1_TEST_TESTBP              (1 << 2)  /* Bit 2:  Test packpressure */
                                                 /* Bits 3-31: Reserved */
/* Ethernet Controller MAC Station Address 0 Register */

#define EMAC1_SA0_STNADDR6_SHIFT        (0)       /* Bits 0-7: Station address 5th octet */
#define EMAC1_SA0_STNADDR6_MASK         (0xff << EMAC1_SA0_STNADDR6_SHIFT)
#define EMAC1_SA0_STNADDR5_SHIFT        (8)       /* Bits 8-15: Station address 6th octet */
#define EMAC1_SA0_STNADDR5_MASK         (0xff << EMAC1_SA0_STNADDR5_SHIFT)
                                              /* Bits 16-31: Reserved */
/* Ethernet Controller MAC Station Address 1 Register */

#define EMAC1_SA1_STNADDR4_SHIFT        (0)       /* Bits 0-7: Station address 4th octet */
#define EMAC1_SA1_STNADDR4_MASK         (0xff << EMAC1_SA0_STNADDR4_SHIFT)
#define EMAC1_SA1_STNADDR3_SHIFT        (8)       /* Bits 8-15: Station address 3rd octet */
#define EMAC1_SA1_STNADDR3_MASK         (0xff << EMAC1_SA0_STNADDR3_SHIFT)
                                              /* Bits 16-31: Reserved */
/* Ethernet Controller MAC Station Address 2 Register */

#define EMAC1_SA2_STNADDR2_SHIFT        (0)       /* Bits 0-7: Station address 2nd octet */
#define EMAC1_SA2_STNADDR2_MASK         (0xff << EMAC1_SA2_STNADDR2_SHIFT)
#define EMAC1_SA2_STNADDR1_SHIFT        (8)       /* Bits 8-15: Station address 1st octet */
#define EMAC1_SA2_STNADDR1_MASK         (0xff << EMAC1_SA2_STNADDR1_SHIFT)
                                              /* Bits 16-31: Reserved */
/* MII Management Registers */

/* Ethernet Controller MAC MII Management Configuration Register */

#define EMAC1_MCFG_SCANINC              (1 << 0)  /* Bit 0:  Scan increment */
#define EMAC1_MCFG_NOPRE                (1 << 1)  /* Bit 1:  Suppress preamble */
#define EMAC1_MCFG_CLKSEL_SHIFT         (2)       /* Bits 2-5: Clock select */
#define EMAC1_MCFG_CLKSEL_MASK          (15 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV4        (0 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV6        (2 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV8        (3 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV10       (4 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV14       (5 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV20       (6 << EMAC1_MCFG_CLKSEL_SHIFT)
#  define EMAC1_MCFG_CLKSEL_DIV40       (8 << EMAC1_MCFG_CLKSEL_SHIFT)
                                                  /* Bits 6-14: Reserved */
#define EMAC1_MCFG_MGMTRST              (1 << 15) /* Bit 15: Reset MII mgmt */
                                                  /* Bits 16-31: Reserved */

/* Ethernet Controller MAC MII Management Command Register */

#define EMAC1_MCMD_READ                 (1 << 0)  /* Bit 0:  Single read cycle */
#define EMAC1_MCMD_SCAN                 (1 << 1)  /* Bit 1:  Continuous read cycles */
                                                  /* Bits 2-31: Reserved */
#define EMAC1_MCMD_WRITE                (0)

/* Ethernet Controller MAC MII Management Address Register */

#define EMAC1_MADR_REGADDR_SHIFT        (0)       /* Bits 0-4: Register address */
#define EMAC1_MADR_REGADDR_MASK         (31 << EMAC1_MADR_REGADDR_SHIFT)
                                                  /* Bits 7-5: Reserved */
#define EMAC1_MADR_PHYADDR_SHIFT        (8)       /* Bits 8-12: PHY address */
#define EMAC1_MADR_PHYADDR_MASK         (31 << EMAC1_MADR_PHYADDR_SHIFT)
                                                  /* Bits 13-31: Reserved */
/* Ethernet Controller MAC MII Management Write Data Register */

#define EMAC1_MWTD_SHIFT                (0)       /* Bits 0-15 */
#define EMAC1_MWTD_MASK                 (0xffff << EMAC1_MWTD_SHIFT)
                                                  /* Bits 16-31: Reserved */
/* Ethernet Controller MAC MII Management Read Data Register */

#define EMAC1_MRDD_SHIFT                (0)       /* Bits 0-15 */
#define EMAC1_MRDD_MASK                 (0xffff << EMAC1_MRDD_SHIFT)
                                                  /* Bits 16-31: Reserved */
/* Ethernet Controller MAC MII Management Indicators Register */

#define EMAC1_MIND_MIIMBUSY             (1 << 0)  /* Bit 0:  Busy */
#define EMAC1_MIND_SCAN                 (1 << 1)  /* Bit 1:  Scanning */
#define EMAC1_MIND_NOTVALID             (1 << 2)  /* Bit 2:  Not valid */
#define EMAC1_MIND_LINKFAIL             (1 << 3)  /* Bit 3:  MII link fail */
                                                  /* Bits 4-31: Reserved */

/********************************************************************************************
 * Public Types
 ********************************************************************************************/

#ifndef __ASSEMBLY__

/********************************************************************************************
 * Inline Functions
 ********************************************************************************************/

/********************************************************************************************
 * Public Function Prototypes
 ********************************************************************************************/

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

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_MIPS_SRC_PIC32MX_PIC32MX_ETHERNET_H */
