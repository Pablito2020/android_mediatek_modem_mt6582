/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    pdn_def_mt6571_series.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file defines PDN X-MACRO for MT6571 series chips.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

/** there 2 kind of PDN definition macro:
 *  X_PDN_USER: Define pre-register PDN users, that need to sync with lpwr.c and register to L1SM
 *  X_PDN_CONST: remaining PDN users, and pure macro definition.
 *               format: X_PDN_CONST(DEVICE_NAME, PDN_CON_REG, PDN_SET_REG, PDN_CLR_REG, PDN_REG_POS, PDN_OPTIONS)
 **/

X_PDN_CONST(UART1,      DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_UART,        PDN_OPT_32BIT_REG | PDN_OPT_NEVER_OFF)
X_PDN_CONST(DMA,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_DMA,         PDN_OPT_32BIT_REG)
X_PDN_CONST(LPM,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_DCM_MON,     PDN_OPT_32BIT_REG)
X_PDN_CONST(SIM,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_SIM1,        PDN_OPT_32BIT_REG)
X_PDN_CONST(SIM2,       DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_SIM2,        PDN_OPT_32BIT_REG)
X_PDN_CONST(GPT,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_GPT,         PDN_OPT_32BIT_REG)
X_PDN_CONST(TOPSM,      DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_TOPSM,       PDN_OPT_32BIT_REG)
X_PDN_CONST(ELM,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_ELM,         PDN_OPT_32BIT_REG)
X_PDN_CONST(OSTIMERARM, DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_OSTIMER,     PDN_OPT_32BIT_REG)
X_PDN_CONST(ABM,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_ABM,         PDN_OPT_32BIT_REG)
X_PDN_CONST(ALC,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_ALC,         PDN_OPT_32BIT_REG)
X_PDN_CONST(ADOE,       DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_ADOE,        PDN_OPT_32BIT_REG)
X_PDN_CONST(PF_BUFF,    DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_PF_BUFF,     PDN_OPT_32BIT_REG)
X_PDN_CONST(BOOT_SLV,   DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_BOOT_SLV,    PDN_OPT_32BIT_REG)
X_PDN_CONST(DBG,        DRVPDN_MD_GLOBAL_CON0,  DRVPDN_MD_GLOBAL_SET0,  DRVPDN_MD_GLOBAL_CLR0,  MD_GLOBAL_CON0_DBG,         PDN_OPT_32BIT_REG)

X_PDN_CONST(BSI3G,      DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_BSI_3G,       PDN_OPT_32BIT_REG)
X_PDN_CONST(BPI3G,      DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_BPI_3G,       PDN_OPT_32BIT_REG)
X_PDN_CONST(WCDMATIMER, DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_WTIMER,       PDN_OPT_32BIT_REG)
X_PDN_CONST(AFC3G,      DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_AFC_3G,       PDN_OPT_32BIT_REG)
X_PDN_CONST(BSIPMIC,    DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_BSI_PMIC,     PDN_OPT_32BIT_REG)
X_PDN_CONST(BPIPMIC,    DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_BPI_PMIC,     PDN_OPT_32BIT_REG)
X_PDN_CONST(PFCDEC,     DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_PFC_DEC,      PDN_OPT_32BIT_REG)
X_PDN_CONST(BYCACC,     DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_BYC_ACC,      PDN_OPT_32BIT_REG)
X_PDN_CONST(ATB,        DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_ATB,          PDN_OPT_32BIT_REG)
X_PDN_CONST(PFCENC,     DRVPDN_MODEM_CG_CON0,   DRVPDN_MODEM_CG_SET0,   DRVPDN_MODEM_CG_CLR0,   MODEM_CG_CON0_PFC_ENC,      PDN_OPT_32BIT_REG)

X_PDN_CONST(GCU,        DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_GCU,          PDN_OPT_32BIT_REG)
X_PDN_CONST(TDMA,       DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_TDMA,         PDN_OPT_32BIT_REG)
X_PDN_CONST(DIV,        DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_DIVIDER,      PDN_OPT_32BIT_REG)
X_PDN_CONST(FCS,        DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_FCS,          PDN_OPT_32BIT_REG)
X_PDN_CONST(BSI2G_T,    DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_BSI_2G_T,     PDN_OPT_32BIT_REG)
X_PDN_CONST(BPI2G_T,    DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_BPI_2G_T,     PDN_OPT_32BIT_REG)
X_PDN_CONST(AFC2G_T,    DRVPDN_MODEM_CG_CON2,   DRVPDN_MODEM_CG_SET2,   DRVPDN_MODEM_CG_CLR2,   MODEM_CG_CON2_AFC_2G_T,     PDN_OPT_32BIT_REG)

X_PDN_CONST(BSI2G,      DRVPDN_MODEM_CG_CON4,   DRVPDN_MODEM_CG_SET4,   DRVPDN_MODEM_CG_CLR4,   MODEM_CG_CON4_BSI_2G,       PDN_OPT_32BIT_REG)
X_PDN_CONST(BPI2G,      DRVPDN_MODEM_CG_CON4,   DRVPDN_MODEM_CG_SET4,   DRVPDN_MODEM_CG_CLR4,   MODEM_CG_CON4_BPI_2G,       PDN_OPT_32BIT_REG)
X_PDN_CONST(AFC2G,      DRVPDN_MODEM_CG_CON4,   DRVPDN_MODEM_CG_SET4,   DRVPDN_MODEM_CG_CLR4,   MODEM_CG_CON4_AFC_2G,       PDN_OPT_32BIT_REG)

X_PDN_CONST(IRDMA,      DRVPDN_MD2GSYS_CG_CON0, DRVPDN_MD2GSYS_CG_SET0, DRVPDN_MD2GSYS_CG_CLR0, MD2GSYS_CG_CON0_IRDMA,      PDN_OPT_16BIT_REG)
X_PDN_CONST(GCC,        DRVPDN_MD2GSYS_CG_CON0, DRVPDN_MD2GSYS_CG_SET0, DRVPDN_MD2GSYS_CG_CLR0, MD2GSYS_CG_CON0_GCC,        PDN_OPT_16BIT_REG)
X_PDN_CONST(TXBUF,      DRVPDN_MD2GSYS_CG_CON0, DRVPDN_MD2GSYS_CG_SET0, DRVPDN_MD2GSYS_CG_CLR0, MD2GSYS_CG_CON0_TXBUF,      PDN_OPT_16BIT_REG)
X_PDN_CONST(RXBUF,      DRVPDN_MD2GSYS_CG_CON0, DRVPDN_MD2GSYS_CG_SET0, DRVPDN_MD2GSYS_CG_CLR0, MD2GSYS_CG_CON0_RXBUF,      PDN_OPT_16BIT_REG)
X_PDN_CONST(IRDBG,      DRVPDN_MD2GSYS_CG_CON0, DRVPDN_MD2GSYS_CG_SET0, DRVPDN_MD2GSYS_CG_CLR0, MD2GSYS_CG_CON0_IRDBG,      PDN_OPT_16BIT_REG)
X_PDN_CONST(AHB2DSPIO,  DRVPDN_MD2GSYS_CG_CON0, DRVPDN_MD2GSYS_CG_SET0, DRVPDN_MD2GSYS_CG_CLR0, MD2GSYS_CG_CON0_AHB2DSPIO,  PDN_OPT_16BIT_REG)

X_PDN_CONST(APC_PRE,    DRVPDN_MD2GSYS_CG_CON2, DRVPDN_MD2GSYS_CG_SET2, DRVPDN_MD2GSYS_CG_CLR2, MD2GSYS_CG_CON2_APC_PRE,    PDN_OPT_16BIT_REG)
X_PDN_CONST(BFE,        DRVPDN_MD2GSYS_CG_CON2, DRVPDN_MD2GSYS_CG_SET2, DRVPDN_MD2GSYS_CG_CLR2, MD2GSYS_CG_CON2_BFE,        PDN_OPT_16BIT_REG)
X_PDN_CONST(VAFE,       DRVPDN_MD2GSYS_CG_CON2, DRVPDN_MD2GSYS_CG_SET2, DRVPDN_MD2GSYS_CG_CLR2, MD2GSYS_CG_CON2_VAFE,       PDN_OPT_16BIT_REG)

X_PDN_CONST(USB,        DRVPDN_AP_CLK_GATING_CTRL1, DRVPDN_AP_SET_CLK_GATING_CTRL1, DRVPDN_AP_CLR_CLK_GATING_CTRL1, DRVPDN_AP_CLK_GATING_USB_SW_CG, PDN_OPT_32BIT_REG)
