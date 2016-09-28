/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "Driver\DrvPDMA.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvI2S.h"
#include "ISD93xx.h"
#include "NVTTypes.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void InitialI2S(void);
void PDMA3_Callback(void);

/*-------------------------------------------------------------------------------*/
/* Global Variable                                                               */
/*-------------------------------------------------------------------------------*/

#define I2S_BUFFERSIZE 0x80
#define MIC_BUFFERSIZE 0x100
//extern int16_t I2sBuffer[2][I2S_BUFFERSIZE];
extern uint32_t u32I2sTempAddr0, u32I2sTempAddr1, I2sBufferEmptyAddr;
extern uint32_t u32MicTempAddr0, u32MicTempAddr1, BufferReadyAddr;
volatile uint32_t PDMA3CallBackCount;
BOOL	bI2sBufferEmpty;


/*--------------------------------------------------------------------------------------------------------*/
/* PDMA_I2S                                                                                               */
/*--------------------------------------------------------------------------------------------------------*/
void PDMA3forI2S(void)
{
	STR_PDMA_T sPDMA;  

   
	/* PDMA Init */
    //DrvPDMA_Init();

	/* PDMA Setting */
	PDMA_GCR->PDSSR.I2S_TXSEL = eDRVPDMA_CHANNEL_3;

    
	/* CH3 TX Setting */
	sPDMA.sSrcAddr.u32Addr 			= u32MicTempAddr0;
    sPDMA.sDestAddr.u32Addr 		= (uint32_t)&I2S->TXFIFO;   
    sPDMA.u8TransWidth 				= eDRVPDMA_WIDTH_32BITS;
	sPDMA.u8Mode 					= eDRVPDMA_MODE_MEM2APB;
	sPDMA.sSrcAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_WRAPAROUND; 
	sPDMA.sDestAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_FIXED;   
	sPDMA.u8WrapBcr				 	= eDRVPDMA_WRA_WRAP_HALF_INT; 		//Interrupt condition set fro Half buffer & buffer end
	sPDMA.i32ByteCnt                = MIC_BUFFERSIZE * 4;
	//sPDMA.i32ByteCnt = MAX_FRAMESIZE * sizeof(int16_t);
	DrvPDMA_Open(eDRVPDMA_CHANNEL_3,&sPDMA);


	/* Enable INT */
	//DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_3, eDRVPDMA_BLKD );
	//DrvPDMA_InstallCallBack(eDRVPDMA_CHANNEL_3,eDRVPDMA_BLKD,(PFN_DRVPDMA_CALLBACK) PDMA3_Callback );
	DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_3, eDRVPDMA_WAR );		//Enable INT    
	DrvPDMA_InstallCallBack(eDRVPDMA_CHANNEL_3, eDRVPDMA_WAR, (PFN_DRVPDMA_CALLBACK) PDMA3_Callback );		//Callback registration	 
    DrvI2S_EnableTxDMA (TRUE);		 
    DrvI2S_EnableTx(TRUE);

  	DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_3);
 	

	
}

/*---------------------------------------------------------------------------------------------------------*/
/* DPWM Callback                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA3_Callback()
{
	PDMA3CallBackCount++;
	bI2sBufferEmpty=TRUE;
}





/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                           	   			   */
/*---------------------------------------------------------------------------------------------------------*/

void StartI2SPdma()
{
	bI2sBufferEmpty=FALSE;
	PDMA3CallBackCount=0;
	PDMA3forI2S();
}	


void InitialI2S(void)
{
	S_DRVI2S_DATA_T st;

	/* GPIO initial and select operation mode for I2S*/
    DrvGPIO_I2S_PA4_PA5_PA6_PA7();
	DrvGPIO_I2S_MCLK_PB1(); 			//Set MCLK I/O

	/* Set I2S Parameter */
    st.u32SampleRate 	 = 16000;
    st.u8WordWidth 	 	 = DRVI2S_DATABIT_16;
    st.u8AudioFormat 	 = DRVI2S_STEREO;  		
	//st.u8AudioFormat 	 = DRVI2S_MONO;
	//st.u8DataFormat  	 = DRVI2S_FORMAT_MSB;   
	st.u8DataFormat  	 = DRVI2S_FORMAT_I2S;
    //st.u8Mode 		 	 = DRVI2S_MODE_SLAVE;
	st.u8Mode 		 	 = DRVI2S_MODE_MASTER;
    st.u8RxFIFOThreshold = DRVI2S_FIFO_LEVEL_WORD_4;
    st.u8TxFIFOThreshold = DRVI2S_FIFO_LEVEL_WORD_4;

	DrvI2S_Open(&st);


	/* Disable I2S Tx/Rx function */
	DrvI2S_EnableRx(FALSE);
    DrvI2S_EnableTx(TRUE);

	//I2S->CLKDIV.BCLK_DIV=0x5F;

	DrvI2S_SetMCLK(4096000);  //
	DrvI2S_EnableMCLK(1);	   //enable MCLK
}

