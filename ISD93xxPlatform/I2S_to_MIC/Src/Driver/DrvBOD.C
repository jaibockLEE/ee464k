/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "ISD93xx.h"
#include "DrvBOD.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static void (*_pfnBODCallBack)(void);

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     <BOD_IRQHandler>                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               ISR to handle BOD interrupt event                                                         */	    
/*---------------------------------------------------------------------------------------------------------*/
void BOD_IRQHandler(void)
{
    BOD->BOD_EN.INT = 1; //Clear INT

    if (_pfnBODCallBack != NULL)    /* execute callback function */
    {
        _pfnBODCallBack();
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_SelectBODVolt		                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               eLevel		- [in]  Specify the BOD level                                                  */
/*                               	eDRVBOD_46V, eDRVBOD_42V, eDRVBOD_39V, eDRVBOD_38V,                     */
/*                     		        eDRVBOD_37V, eDRVBOD_36V, eDRVBOD_34V, eDRVBOD_32V,              	     */
/*                                eDRVBOD_31V, eDRVBOD_30V, eDRVBOD_28V, eDRVBOD_2625V,                    */
/*                                eDRVBOD_25V, eDRVBOD_24V, eDRVBOD_22V, eDRVBOD_21V                       */
/* Returns:                                                                                                */
/*               None                                                                                     */  
/*              					                                                                           */
/* Description:                                                                                            */
/*               Select BOD voltage Level                           	           		   	   		         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvBOD_SelectBODVolt(E_DRVBOD_LVL eLevel)
{ 
    BOD->BOD_SEL.BOD_LVL = eLevel&0x07;
    BOD->BOD_SEL.BOD_RANGE = (eLevel>>3)&0x1;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_SelectBODHyst				                                                             */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               eHYS	- [in] 	  eDRVBOD_ENABLE: Enable Hysteresis of BOD detect                       */
/*                     	     	      eDRVBOD_DISABLE: Hysteresis Disabled          	   	                   */
/* Returns:                                                                                                */
/*               None				                                                                   	   */
/*              					                                                                           */
/* Description:                                                                                            */
/*               Select BOD Hysteresis                           	           	                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvBOD_SelectBODHyst(E_DRVBOD_HYS eHYS)
{
    if(eHYS == eDRVBOD_DISABLE)
        BOD->BOD_SEL.BOD_HYS = 0;
    else
        BOD->BOD_SEL.BOD_HYS = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_EnableBOD				                                               	   		   	     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32Enable    	- [in] 	  eDRVBOD_TIME_MULTIPLEXED : Enable continuous BOD detection  	   */
/*                     		                eDRVBOD_CONTINUOUS : Enable time multiplexed BOD detection     */
/*                                        eDRVBOD_DISABLE : Disable BOD detection             	   		     */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */		 
/*               Enable BOD function                           	           		   	   		   			  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvBOD_EnableBOD(E_DRVBOD_EN eEnable)
{
    SYSCLK->PWRCON.OSC10K_ENB = 0;
    switch(eEnable)
    {
        case eDRVBOD_DISABLE:
            BOD->BOD_EN.EN = 0;
        break;	
		case eDRVBOD_TIME_MULTIPLEXED:  
            BOD->BOD_EN.EN = 1;
        break;
        case eDRVBOD_CONTINUOUS:
            BOD->BOD_EN.EN = 2;
        break;
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_EnableInt				                                                            	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*				   pfnBODCallBack  -[in] The callback function of BOD interrupts.    		       			   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None	                                                                                  */
/*              			                                                                             	   */
/* Description:                                                                                            */
/*               Enable BOD Interrupt and install BOD interrrupt callback functions       	   		   		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvBOD_EnableInt(PFN_DRVBOD_CALLBACK pfnBODCallBack)
{
    _pfnBODCallBack = pfnBODCallBack;
    
    BOD->BOD_EN.IE = 1;
    
    NVIC_EnableIRQ(BOD_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_DisableInt	                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*				   None                                                                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*              					                                                                            */
/* Description:                                                                                            */
/*               Disable BOD Interrupt and un-install BOD interrrupt callback functions               	   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvBOD_DisableInt(void)
{
    _pfnBODCallBack = NULL;
    
    BOD->BOD_EN.IE = 0;
    
    NVIC_DisableIRQ(BOD_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_GetBODout				                                               	   			     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None			  						   									           			   */
/*                     		                           	   											        	   */
/* Returns:                                                                                                */
/*               Output of BOD detection block			                                                   */
/*              					                                                               			   */     
/* Description:                                                                                            */
/*               Get BOD output block                           	           		   	   	    		   	   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvBOD_GetBODout(void)
{
	return BOD->BOD_EN.BOD_OUT;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_SetDetectionTime				                                                      	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u8OnDUR	  - [in] 	  Time BOD detector is active (100us)	                                 */
/*               u16OffDUR  - [in]    Time BOD detector if off    (100us)                                  */
/*              					                                                                            */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set up BOD detector to take periodic samples of the                                       */
/*               supply voltage to minimize power consumption                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvBOD_SetDetectionTime(uint8_t u8OnDUR, uint16_t u16OffDUR)
{
	BOD->DET_TIMER.ON_DUR  = u8OnDUR;
	BOD->DET_TIMER.OFF_DUR = u16OffDUR;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvBOD_GetVersion                                                                           */		   
/*                                                                                                         */
/* Parameters:                                                                                             */
/*              None                                                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*              None                                                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*              Get the version number of BOD driver.                                                      */			     
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvBOD_GetVersion(void)
{
	return DRVBOD_VERSION_NUM;
}

