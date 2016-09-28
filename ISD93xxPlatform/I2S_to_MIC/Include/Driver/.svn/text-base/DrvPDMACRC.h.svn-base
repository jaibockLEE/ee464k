/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef DRVPDMACRC_H
#define DRVPDMACRC_H

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"

#ifdef  __cplusplus
extern "C"
{
#endif


/*---------------------------------------------------------------------------------------------------------*/
/* Version number                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPDMACRC_MAJOR_NUM 1
#define DRVPDMACRC_MINOR_NUM 00
#define DRVPDMACRC_BUILD_NUM 1


#define DRVPDMACRC_VERSION_NUM     _SYSINFRA_VERSION(DRVPDMACRC_MAJOR_NUM, DRVPDMACRC_MINOR_NUM, DRVPDMACRC_BUILD_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* Error Code define                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define E_DRVPDMACRC_FALSE_INPUT     	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPDMA, 1)


/******************************************************************************/
/*                CRC                                                         */
/******************************************************************************/
/********************* Bit definition of CTL register **********************/
#define PDMACRC_CTL_CEN	                ((uint32_t)0x00000001)			/*!<CRC Channel Enable */
#define PDMACRC_CTL_RST	                ((uint32_t)0x00000002)			/*!<CRC Engine Reset */
#define PDMACRC_CTL_WDATA_RVS	        ((uint32_t)0x01000000)			/*!<Write Data Order Reverse */
#define PDMACRC_CTL_CHECKSUM_RVS		((uint32_t)0x02000000)			/*!<Checksum Reverse */
#define PDMACRC_CTL_WDATA_COM			((uint32_t)0x04000000)			/*!<Write Data Complement */
#define PDMACRC_CTL_CHECKSUM_COM		((uint32_t)0x08000000)			/*!<Checksum Complement */
#define PDMACRC_CTL_CPU_WDLEN_8BIT		((uint32_t)0x00000000)			/*!<CPU Write Data Length 8-bit */
#define PDMACRC_CTL_CPU_WDLEN_16BIT		((uint32_t)0x10000000)			/*!<CPU Write Data Length 16-bit */
#define PDMACRC_CTL_CPU_WDLEN_32BIT		((uint32_t)0x20000000)			/*!<CPU Write Data Length 32-bit */
#define PDMACRC_CTL_CPU_WDLEN_MASK		((uint32_t)0x30000000)			/*!<CPU Write Data Length Mask */
#define PDMACRC_CTL_MODE_CCITT			((uint32_t)0x00000000)			/*!<CRC Polynomial Mode CCITT */
#define PDMACRC_CTL_MODE_CRC8			((uint32_t)0x40000000)			/*!<CRC Polynomial Mode CRC8 */
#define PDMACRC_CTL_MODE_CRC16			((uint32_t)0x80000000)			/*!<CRC Polynomial Mode CRC16 */
#define PDMACRC_CTL_MODE_CRC32			((uint32_t)0xC0000000)			/*!<CRC Polynomial Mode CRC32 */
#define PDMACRC_CTL_MODE_MASK			((uint32_t)0xC0000000)			/*!<CRC Polynomial Mode Mask */
#define PDMACRC_CTL_TRIG_EN			    ((uint32_t)0x00800000)			/*!<PDMACRC Trigger */

/********************* Bit definition of DMAIER register **********************/
#define PDMACRC_DMAIER_TABORT		((uint32_t)0x00000001)			/*!<PDMACRC Read/Write Target Abort Interrupt Enable */
#define PDMACRC_DMAIER_TD			((uint32_t)0x00000002)			/*!<PDMACRC Transfer Done Interrupt Enable */

/********************* Bit definition of DMAISR register **********************/
#define PDMACRC_DMAISR_TABORT		((uint32_t)0x00000001)			/*!<PDMACRC Read/Write Target Abort Interrupt Flag */
#define PDMACRC_DMAISR_TD			((uint32_t)0x00000002)			/*!<PDMACRC Transfer Done Interrupt Flag */

/*---------------------------------------------------------------------------------------------------------*/
/* For interrupt CallBack Function                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Enumerate Type                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum
{
	eDRVPDMACRC_CCITT = 0,
	eDRVPDMACRC_CRC8  = 1,
	eDRVPDMACRC_CRC16 = 2,
	eDRVPDMACRC_CRC32 = 3
}E_DRVPDMACRC_MODE;

typedef enum
{
	eDRVPDMACRC_8BIT	=	0,
	eDRVPDMACRC_16BIT 	=	1,
	eDRVPDMACRC_32BIT	=	2
}E_DRVPDMACRC_CPU_WDLEN;

typedef enum
{
	eDRVPDMACRC_NO_1sCOMPLEMENT = 0,
	eDRVPDMACRC_1sCOMPLEMENT    = 1
}E_DRVPDMACRC_COMPLEMENT;

typedef enum
{
	eDRVPDMACRC_NO_BITORDER  = 0,
	eDRVPDMACRC_BITORDER     = 1
}E_DRVPDMACRC_REVERSE;

typedef enum
{
	eDRVPDMACRC_DISABLE = 0,
	eDRVPDMACRC_ENABLE  = 1
}E_DRVPDMACRC_ENABLE;


typedef struct DRVPDMACRC_CTLSTRUCT
{
    E_DRVPDMACRC_ENABLE     u8ChannelEn;    		/* CRC Channel Enable */
	E_DRVPDMACRC_ENABLE     u8Reset;				/* CRC Engine Reset */
    E_DRVPDMACRC_ENABLE     u8TrigEn;				/* Trig Enable */
    E_DRVPDMACRC_REVERSE  	u8WrDataReverse;		/* Write Data Reverse */
	E_DRVPDMACRC_REVERSE	u8ChecksumReverse;		/* Checksum Reverse */
	E_DRVPDMACRC_COMPLEMENT u8WrDataComplement;     /* Write Data Complement */
	E_DRVPDMACRC_COMPLEMENT u8ChecksumComplement;   /* Checksum Complement */
	E_DRVPDMACRC_CPU_WDLEN  u8CpuWrDataLen;         /* CPU Write Data Length */
	E_DRVPDMACRC_MODE       u8CRCpolynomialMode;    /* CRC Polynomial Mode */
}STR_PDMACRC_CTL_T;



/*---------------------------------------------------------------------------------------------------------*/
/* APIs declaration                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

void DrvPDMACRC_Init(void);
void DrvPDMACRC_Close(uint32_t u32PDMAAHB);
int32_t DrvPDMACRC_SetCTL(STR_PDMACRC_CTL_T *sCTLParam);
int32_t DrvPDMACRC_SetDMAsrcAddr(uint32_t u32SrcAddr);
int32_t DrvPDMACRC_SetTransferByteCount(uint32_t u32TrnBC);
uint32_t DrvPDMACRC_GetDMAcurrentSAR(void);
uint32_t DrvPDMACRC_GetDMAcurrentBCR(void);
int32_t DrvPDMACRC_SetInt(uint32_t u32BLKDINT, uint32_t u32TABORTINT);
uint32_t DrvPDMACRC_GetBLKD_INTflag(void);
uint32_t DrvPDMACRC_GetTABORT_INTflag(void);
int32_t DrvPDMACRC_ClearIF(uint32_t u32ClrBLKD_IF, uint32_t u32ClrTABORT_IF);
int32_t DrvPDMACRC_SetWDATA(uint32_t u32Wdata);
uint32_t DrvPDMACRC_GetWDATA(void);
int32_t DrvPDMACRC_SetSEED(uint32_t u32Seed);
uint32_t DrvPDMACRC_GetSEED(void);
uint32_t DrvPDMACRC_GetCheckSum(void);
int32_t DrvPDMACRC_GetVersion(void);


#ifdef  __cplusplus
}
#endif

#endif	// DRVPDMACRC_H

