/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"
#include "DrvDPWM.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_Open                                                                                  */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Open DPWM IP and config the parameter.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_Open(void)
{
	/* enable DPWM clock */
	SYSCLK->APBCLK.DPWM_EN        =1;

	/* reset DPWM */
	SYS->IPRSTC2.DPWM_RST         =1;
	SYS->IPRSTC2.DPWM_RST         =0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_Close                                                                                 */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Close DPWM IP                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_Close(void)
{
	int32_t i=16;
        // Flush FIFO
	while(i--)
		DPWM->FIFO = 0;	
	/* reset DPWM */
	SYS->IPRSTC2.DPWM_RST         =1;
	SYS->IPRSTC2.DPWM_RST         =0;

	/* disable DPWM clock */
	SYSCLK->APBCLK.DPWM_EN        =0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_SetFrequency                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               Frequency [in]:  Specify the DPWM Frequency.                                              */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Select DPWM Frequnecy.                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_SetFrequency(E_DRVDPWM_FREQ eFrequency)
{
    DPWM->CTRL.Freq = eFrequency;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_GetFrequency                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               return the DPWM Frequency in Hz.                                                          */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get current DPWM Frequnecy.                                                               */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvDPWM_GetFrequency(void)
{
	return (DPWM->CTRL.Freq);	  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_SetDeadTime                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               eDeadTime [in]:  Specify the Dead time.                                                   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Select DPWM Dead time.                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_SetDeadTime(E_DRVDPWM_DEADTIME eDeadTime)
{
    DPWM->CTRL.Deadtime = eDeadTime;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_GetDeadTime                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Deadtime :  return the DPWM Dead Time.                                                    */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get current DPWM Dead time.                                                               */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvDPWM_GetDeadTime(void)
{
    return (DPWM->CTRL.Deadtime);	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_EnableDither                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               eDitherType [in]: Specify the dither type.                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable dither function.                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_Dither(E_DRVDPWM_DITHERTYPE eDitherType)
{
    DPWM->CTRL.Dither = eDitherType;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_Enable                                                                                */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable DPWM.                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_Enable(void)
{
    DPWM->CTRL.Enable = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_Disable                                                                               */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable DPWM.                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_Disable(void)
{
    DPWM->CTRL.Enable = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_IsFIFOFull                                                                            */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*                None.                                                                                    */
/*                                                                                                         */
/* Returns:                                                                                                */
/*                TRUE : FIFO is full.                                                                     */
/*                FALSE : FIFO is not full.                                                                */
/* Description:                                                                                            */
/*                Check if DPWM FIFO is full or not.                                                       */
/*---------------------------------------------------------------------------------------------------------*/
BOOL DrvDPWM_IsFIFOFull(void)
{
    return (DPWM->STAT.Full ? TRUE: FALSE);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_IsFIFOEmpty                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               TRUE : FIFO is empty.                                                                     */
/*               FALSE : FIFO is not empty.                                                                */
/* Description:                                                                                            */
/*               Check if DPWM FIFO is empty or not.                                                       */
/*---------------------------------------------------------------------------------------------------------*/
BOOL DrvDPWM_IsFIFOEmpty(void)
{
	 return (DPWM->STAT.Empty ? TRUE: FALSE);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_EnablePDMA                                                                            */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Enable DPWM for PDMA transfer.                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_EnablePDMA(void)
{
	 DPWM->DMA.EnablePDMA = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_DisablePDMA                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Disable DPWM for PDMA transfer.                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_DisablePDMA(void)
{
	 DPWM->DMA.EnablePDMA = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_WriteFIFO                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               pi16Stream : pointer of input data stream for transmit.                                   */
/*               i32count   : transmit sample count.                                                       */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Write to DPWM FIFO for transmit.                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_WriteFIFO(int16_t *pi16Stream, uint32_t u32count)
{
    uint32_t u32countRemain;
	
    u32countRemain = u32count;

	while(u32countRemain > 0)	
    {
	    while(DPWM->STAT.Full);

        DPWM->FIFO = *pi16Stream++;
        u32countRemain --;	
	
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_SetSampleRate                                                                         */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               Sample Rate in Hz.                                                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Actual sample rate set.                                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set the Sample Rate.                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvDPWM_SetSampleRate(uint32_t u32SampleRate)
{
    uint32_t u32clk;
	
    u32clk = DrvSYS_GetHCLK() * 1000;

    DPWM->ZOH_DIV = (uint8_t)((u32clk/64)/u32SampleRate);
    return (u32clk /(DPWM->ZOH_DIV*64));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_GetUpSampleRate                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Current Sample Rate DPWM is set to in Hz                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Get the Sample Rate of DPWM.                                                              */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvDPWM_GetSampleRate(void)
{
    uint32_t u32clk;
	
    u32clk = DrvSYS_GetHCLK() * 1000;
    return (u32clk / (DPWM->ZOH_DIV*64) );
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_SetPWMClk                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None.                                                                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               Set the clock source of DPWM.                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_SetDPWMClk(E_DRVDPWM_DPWMCLK eDpwmClk)
{
    SYSCLK->CLKSEL1.DPWM_S = eDpwmClk;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvDPWM_GetVersion                                                                              */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*               None.                                                                                       */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*               Version number.                                                                             */
/*                                                                                                           */
/* Description:                                                                                              */
/*               This function is used to return the version number of DPWM driver.    	                     */
/*-----------------------------------------------------------------------------------------------------------*/
uint32_t DrvDPWM_GetVersion(void)
{
    return DRVDPWM_VERSION_NUM;
}
