/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "ISD93xx.h"
#include "Driver\DrvPDMA.h"
#include "Driver\DrvDPWM.h"
#include "Driver\DrvFMC.h"


volatile uint32_t PDMA1Counter;
extern 	uint32_t u32TempAddr0,u32TempAddr1,BufferReadyAddr,BufferSampleCount;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA1_Callback(void);
//void PDMA1forDPWM(void);




/*---------------------------------------------------------------------------------------------------------*/
/* InitialDPWM                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void InitialDPWM(uint32_t u32SampleRate)
{
	DrvDPWM_Open();
	DrvDPWM_SetDPWMClk(E_DRVDPWM_DPWMCLK_HCLKX2);
	//DrvDPWM_SetDPWMClk(E_DRVDPWM_DPWMCLK_HCLK);
	DrvDPWM_SetSampleRate(u32SampleRate);
	DrvDPWM_Enable();
}





/*---------------------------------------------------------------------------------------------------------*/
/* Set PDMA0 to move ADC FIFO to MIC buffer with wrapped-around mode                                       */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA1forDPWM(void)
{
	STR_PDMA_T sPDMA;  

	sPDMA.sSrcAddr.u32Addr 			= BufferReadyAddr; 
	sPDMA.sDestAddr.u32Addr 		= (uint32_t)&DPWM->FIFO;
	sPDMA.u8Mode 					= eDRVPDMA_MODE_MEM2APB;;
	sPDMA.u8TransWidth 				= eDRVPDMA_WIDTH_16BITS;
	sPDMA.sSrcAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_WRAPAROUND;
	//sPDMA.sSrcAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_INCREMENTED; 
	sPDMA.sDestAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_FIXED;  
	sPDMA.u8WrapBcr				 	= eDRVPDMA_WRA_WRAP_HALF_INT; 		//Interrupt condition set fro Half buffer & buffer end	 //For WARPROUND
    //sPDMA.i32ByteCnt = BufferSampleCount * 2;	   	//Full MIC buffer length (byte)		//For INCREMENTED
	sPDMA.i32ByteCnt = BufferSampleCount * 4;	   	//Full MIC buffer length (byte)		//For WARPROUND
    DrvPDMA_Open(eDRVPDMA_CHANNEL_1, &sPDMA);

	// PDMA Setting 
    //PDMA_GCR->PDSSR.ADC_RXSEL = eDRVPDMA_CHANNEL_2;

	DrvPDMA_SetCHForAPBDevice(
    	eDRVPDMA_CHANNEL_1, 
    	eDRVPDMA_DPWM,
    	eDRVPDMA_WRITE_APB    
	);


	// Enable INT 
	//DrvPDMA_DisableInt(eDRVPDMA_CHANNEL_1, eDRVPDMA_BLKD );
	//DrvPDMA_DisableInt(eDRVPDMA_CHANNEL_1, eDRVPDMA_WAR );
	DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_1, eDRVPDMA_WAR); 	  		//For WARPROUND
	//DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_1, eDRVPDMA_BLKD ); 		//For INCREMENTED

	// Install Callback function    
	DrvPDMA_InstallCallBack(eDRVPDMA_CHANNEL_1, eDRVPDMA_WAR, (PFN_DRVPDMA_CALLBACK) PDMA1_Callback ); 
	//DrvPDMA_InstallCallBack(eDRVPDMA_CHANNEL_1, eDRVPDMA_BLKD, (PFN_DRVPDMA_CALLBACK) PDMA1_Callback ); 

	// Enable DPWM DMA
	DrvDPWM_EnablePDMA();	
		
	DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_1);

}


/*---------------------------------------------------------------------------------------------------------*/
/* DPWM Callback                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA1_Callback()
{
	
	//PDMA1forDPWM();
	PDMA1Counter++;
	//DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_1, eDRVPDMA_WAR); 	  		//For WARPROUND

}






/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
void PlayStart(void)
{
//	uint8_t u8Option;
//	int32_t	i32Err;
//	uint32_t	u32temp;

	InitialDPWM(16000); 
	PDMA1forDPWM();

	
}



