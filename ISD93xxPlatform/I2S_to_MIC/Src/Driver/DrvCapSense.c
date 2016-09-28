/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "isd93xx.h"
#include "DrvCapSense.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_ISRC_En                                                                       */
/* Parameter:                                                                                              */
/*               bits     - [in]      1: Enable   0:Disable 		                                       */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable current source to GPIOB pins, each pin has a separate current source.              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_ISRC_En(uint32_t bits)
{
	ANA->ISRC1.EN = bits;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_ISRC_Val                                                                      */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               val     - [in]     3=5uA, 2=2.5uA, 1=1uA, 0=0.5uA                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Select master current for source generation            		                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_ISRC_Val(uint32_t val)
{
	 ANA->ISRC.VAL = val;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_Ctrl                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               time    - [in]   Number of PCLK cycles to discharge external capacitor.                   */
/*               cnt     - [in]   Peripheral performs 2^(CYCLE_CNT) reladation cycles before Gen. INT      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Setting cycle counts before  generating an interrupt                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_Ctrl(uint32_t time, uint32_t cnt)
{
	ANA->CAPS_CTRL.LOW_TIME = time;
    ANA->CAPS_CTRL.CYCLE_CNT = cnt;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_INT_Enable                                                                    */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable CAPS_IRQ interrupt            		                                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_INT_Enable(void)
{
	ANA->CAPS_CTRL.INT_EN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_INT_Disable                                                                   */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable/Reset CAPS_IRQ interrupt          		                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_INT_Disable(void)
{
	ANA->CAPS_CTRL.INT_EN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_ResetCnt                                                                      */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Reset capsense count then release/activate capsense count           		               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_ResetCnt(void)
{
	ANA->CAPS_CTRL.RST_CNT = 1;
    ANA->CAPS_CTRL.RST_CNT = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvCAPSENSE_CntEnable                                                                     */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable capsense block            		                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvCAPSENSE_CntEnable(void)
{
	ANA->CAPS_CTRL.CNT_EN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvCAPSENSE_GetVersion                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None	                                                         						   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvCAPSENSE version number                                                            */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvCAPSENSE version number                               */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvCAPSENSE_GetVersion(void)
{
	return DRVCAPSENSE_VERSION_NUM;
}
