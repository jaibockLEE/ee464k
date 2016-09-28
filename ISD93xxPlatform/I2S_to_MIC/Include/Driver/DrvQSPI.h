/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVSPI_H__
#define __DRVSPI_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"
#include "System/SysInfra.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVSPI_MAJOR_NUM 1
#define DRVSPI_MINOR_NUM 00
#define DRVSPI_BUILD_NUM 001

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVSPI_VERSION_NUM    _SYSINFRA_VERSION(DRVSPI_MAJOR_NUM,DRVSPI_MINOR_NUM,DRVSPI_BUILD_NUM)  

/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVSPI_ERR_BURST_CNT                 Wrong Burst Number			
// E_DRVSPI_ERR_TRANSMIT_INTERVAL         Wrong Transmit Number
// E_DRVSPI_ERR_BIT_LENGTH                Wrong Bit Length
// E_DRVSPI_ERR_INIT                      Init Fail
// E_DRVSPI_ERR_BUSY                      SPI is busy
// E_DRVSPI_ERR_PORT                      SPI Port does not exist
#define E_DRVSPI_ERR_BURST_CNT            _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 0)
#define E_DRVSPI_ERR_TRANSMIT_INTERVAL    _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 1)
#define E_DRVSPI_ERR_BIT_LENGTH           _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 2)
#define E_DRVSPI_ERR_INIT                 _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 3)
#define E_DRVSPI_ERR_BUSY                 _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 4)
#define E_DRVSPI_ERR_PORT			        	  _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 5)

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Setting                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*PFN_DRVSPI_CALLBACK)(uint32_t u32Status);

typedef int32_t ERRCODE;
typedef unsigned char BOOL;

typedef enum {
	eDRVSPI_PORT0
} E_DRVSPI_PORT;

typedef enum {
	eDRVSPI_MASTER,
	eDRVSPI_SLAVE
} E_DRVSPI_MODE;

typedef enum {
	eDRVSPI_TYPE0,
	eDRVSPI_TYPE1,
	eDRVSPI_TYPE2,
	eDRVSPI_TYPE3,
	eDRVSPI_TYPE4,
	eDRVSPI_TYPE5,
	eDRVSPI_TYPE6,
	eDRVSPI_TYPE7
} E_DRVSPI_TRANS_TYPE;

typedef enum {
	eDRVSPI_LSB_FIRST,
	eDRVSPI_MSB_FIRST
} E_DRVSPI_ENDIAN;

typedef enum {
	eDRVSPI_ACTIVE_LOW_FALLING,
	eDRVSPI_ACTIVE_HIGH_RISING
} E_DRVSPI_SS_ACT_TYPE;

typedef enum {
	eDRVSPI_NONE,
	eDRVSPI_SS0,
	eDRVSPI_SS1,
	eDRVSPI_SS0_SS1
} E_DRVSPI_SLAVE_SEL;

typedef enum {
	eDRVSPI_TX_DMA,
	eDRVSPI_RX_DMA
} E_DRVSPI_DMA_MODE;

typedef enum {
	eDRVSPI_STANDARD,
	eDRVSPI_QUAD,
	eDRVSPI_DUAL
} E_DRVSPI_STANDARD_QUAD_DUAL_MODE;

typedef enum {
	eDRVSPI_INPUT,
	eDRVSPI_OUTPUT
} E_DRVSPI_QD_IO_DIR;

typedef enum
{
    eDRVSPI_UNIT      = BIT0,
    eDRVSPI_SSINA     = BIT1, 
    eDRVSPI_SSACT 	  = BIT2,
    eDRVSPI_SLVER1    = BIT3,
    eDRVSPI_SLVER0    = BIT4,
    eDRVSPI_SLVTO     = BIT5,
    eDRVSPI_TXUDR 	  = BIT6, 
    eDRVSPI_RXOVR     = BIT7,
    eDRVSPI_RXTO      = BIT8,
    eDRVSPI_TXTH      = BIT9,
    eDRVSPI_RXTH      = BIT10,
} E_DRVSPI_CALLBACK_TYPE;

typedef enum
{
    eDRVSPI_UNIT_FLAG_CLR  = BIT0,
    eDRVSPI_SSINA_FLAG_CLR  = BIT1, 
    eDRVSPI_SSACT_FLAG_CLR  = BIT2,
    eDRVSPI_TXUDR_FLAG_CLR  = BIT6, 
    eDRVSPI_RXOVR_FLAG_CLR  = BIT7,
    eDRVSPI_RXTO_FLAG_CLR  = BIT8,
} E_DRVSPI_FLAG_CLEAR_TYPE;

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Port                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define SPI_PORT0 0
//#define SPI_PORT1 1
//#define SPI_PORT2 2
//#define SPI_PORT3 3

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void SPI0_IRQHandler(void);	  
ERRCODE DrvSPI_Open(E_DRVSPI_PORT eSpiPort, E_DRVSPI_MODE eMode, E_DRVSPI_TRANS_TYPE eType, int32_t i32BitLength);	
void DrvSPI_Close(E_DRVSPI_PORT eSpiPort);	 
void DrvSPI_Set2BitSerialDataIOMode(E_DRVSPI_PORT eSpiPort, BOOL bEnable);	
void DrvSPI_SetEndian(E_DRVSPI_PORT eSpiPort, E_DRVSPI_ENDIAN eEndian);	 
ERRCODE DrvSPI_SetBitLength(E_DRVSPI_PORT eSpiPort, int32_t i32BitLength);
ERRCODE DrvSPI_SetByteEndian(E_DRVSPI_PORT eSpiPort, BOOL bEnable);	
ERRCODE DrvSPI_SetSuspendCycle(E_DRVSPI_PORT eSpiPort, int32_t i32Interval);
void DrvSPI_SetSlaveSelectActiveLevel(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SS_ACT_TYPE eSSActType);	 
void DrvSPI_EnableAutoCS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel);	 
void DrvSPI_DisableAutoCS(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_SetCS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel);	
void DrvSPI_ClrCS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel);  
uint32_t DrvSPI_SetClock(E_DRVSPI_PORT eSpiPort, uint32_t u32DesiredClockFreq);	 
uint32_t DrvSPI_GetClock(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_EnableInt(E_DRVSPI_PORT eSpiPort, uint16_t eSpiCallType, PFN_DRVSPI_CALLBACK pfnCallback);	  
void DrvSPI_DisableInt(E_DRVSPI_PORT eSpiPort, uint16_t eSpiCallType);	  
void DrvSPI_ClearIntFlag(E_DRVSPI_PORT eSpiPort, uint16_t eSpiCallType);	
uint8_t DrvSPI_GetIntFlag(E_DRVSPI_PORT eSpiPort, E_DRVSPI_CALLBACK_TYPE eSpiCallType);	 
uint32_t DrvSPI_Status(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_EnableSPITransfer(E_DRVSPI_PORT eSpiPort);	   
void DrvSPI_DisableSPITransfer(E_DRVSPI_PORT eSpiPort);	  
void DrvSPI_StartPDMA(E_DRVSPI_PORT eSpiPort, E_DRVSPI_DMA_MODE eDmaMode, BOOL bEnable);
BOOL DrvSPI_IsRxEmpty(E_DRVSPI_PORT eSpiPort);	  
BOOL DrvSPI_IsRxFull(E_DRVSPI_PORT eSpiPort); 
BOOL DrvSPI_IsTxEmpty(E_DRVSPI_PORT eSpiPort);
BOOL DrvSPI_IsTxFull(E_DRVSPI_PORT eSpiPort); 
void DrvSPI_Enable3Wire(E_DRVSPI_PORT eSpiPort);	  
void DrvSPI_Disable3Wire(E_DRVSPI_PORT eSpiPort);	   
void DrvSPI_EnableRxMode(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_DisableRxMode(E_DRVSPI_PORT eSpiPort);
void DrvSPI_SetRxTransCnt(E_DRVSPI_PORT eSpiPort, BOOL bEnable, uint16_t u16BitLength);	  
void DrvSPI_IoMode(E_DRVSPI_PORT eSpiPort, E_DRVSPI_STANDARD_QUAD_DUAL_MODE eMode);
void DrvSPI_QuadDualIoDir(E_DRVSPI_PORT eSpiPort, E_DRVSPI_QD_IO_DIR eQDIoDir);	 
uint8_t DrvSPI_GetTxFifoCount(E_DRVSPI_PORT eSpiPort);
uint8_t DrvSPI_GetRxFifoCount(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_ClrTxFifo(E_DRVSPI_PORT eSpiPort);	  
void DrvSPI_ClrRxFifo(E_DRVSPI_PORT eSpiPort);	 
void DrvSPI_EnableSlvTimeoutFifoClr(E_DRVSPI_PORT eSpiPort);		 
void DrvSPI_DisableSlvTimeoutFifoClr(E_DRVSPI_PORT eSpiPort);		
void DrvSPI_SlvTimeoutPeriod(E_DRVSPI_PORT eSpiPort, uint16_t u16Slvtoprd);	 
ERRCODE DrvSPI_RxFifoThr(E_DRVSPI_PORT eSpiPort, uint8_t u8RxTh); 
ERRCODE DrvSPI_TxFifoThr(E_DRVSPI_PORT eSpiPort, uint8_t u8TxTh); 
void DrvSPI_SetTxFifoUnderrunDo(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_ClearTxFifoUnderrunDo(E_DRVSPI_PORT eSpiPort);
BOOL DrvSPI_GetBusyStatus(E_DRVSPI_PORT eSpiPort);	 
BOOL DrvSPI_GetSSLineStatus(E_DRVSPI_PORT eSpiPort); 
uint32_t DrvSPI_ReadRx(E_DRVSPI_PORT eSpiPort);	
void DrvSPI_WriteTx(E_DRVSPI_PORT eSpiPort, uint32_t u32Data);	 
uint32_t DrvSPI_GetVersion(void);

#endif
