/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2011 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "ISD93xx.h"
#include "Driver\DrvTALARM.h"
#include "Driver\DrvSYS.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static PFN_DRVTALARM_CALLBACK  *g_pfnTALARMCallBack_TALARM   = NULL;

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <TALARM_IRQHandler>                                                                       */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle TALARM interrupt event            		                                   */
/*---------------------------------------------------------------------------------------------------------*/
void TALARM_IRQHandler(void)
{
	TALARM->TALARM_EN.INT = 1; //Clear INT
    if (g_pfnTALARMCallBack_TALARM != NULL)    /* execute callback function */
    {
        g_pfnTALARMCallBack_TALARM();
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTALARM_SetTALARMselect				                                               	       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32LVL		- [in] 	  1000: 145C    0100: 135C    0010: 125C   							   */
/*                      		      0001: 115C    0000: 105C                     	   					   */
/* Returns:                                                                                                */
/*               None 										 											   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Set Tempature Alarm Sensor Level                           	           		   	   	   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTALARM_SetTALARMselect(uint32_t u32LVL)
{
	TALARM->TALARM_SEL.LVL = u32LVL;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTALARM_EnableTALARM				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Enable		- [in] 	  1 : Enable TALARM detection  						               */
/*                     		              0 : Disable TALARM detection                                     */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable TALARM function                           	           		   	   		   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTALARM_EnableTALARM(uint32_t u32Enable)
{
	TALARM->TALARM_EN.EN = u32Enable & 0x01;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTALARM_GetTALARMstatus				                                               	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None			  						   									   			   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               Status of TALARM block			                                                           */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Get status of TALARM                           	           		   	   			   	   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvTALARM_GetTALARMstatus(void)
{
	return TALARM->TALARM_EN.TEPALARM;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTALARM_SetTALARMIE				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32TALARMIE	- [in] 	  1 : Enable TALARM interrupt  						               */
/*                     		              0 : Disable TALARM interrupt                                     */
/* Returns:                                                                                                */
/*               None																		   	   		   */
/*              					                                                           			   */
/* Description:                                                                                            */
/*               Enable TALARM Interrupt                           	           		   	   		   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTALARM_SetTALARMIE(uint32_t u32TALARMIE)
{
	TALARM->TALARM_EN.IE = u32TALARMIE & 0x01;
	if( u32TALARMIE )
		NVIC_EnableIRQ(TALARM_IRQn);
    else
		NVIC_DisableIRQ(TALARM_IRQn);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTALARM_InstallISR                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               callback : call back function	                                                           */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to install TALARM callback function                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTALARM_InstallISR(PFN_DRVTALARM_CALLBACK callback)
{
 	g_pfnTALARMCallBack_TALARM = callback;

}



/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTALARM_GetVersion                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*              None                                                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*              None                                                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*              Get the version number of TALARM driver.                                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTALARM_GetVersion(void)
{
	return DRVTALARM_VERSION_NUM;
}

