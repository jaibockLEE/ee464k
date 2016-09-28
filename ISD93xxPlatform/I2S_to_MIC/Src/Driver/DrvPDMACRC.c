/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "isd93xx.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvPDMA.h"
#include "DrvPDMACRC.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

#define MAX_TRANSFER_BYTE_COUNT     0xFF

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_Init				                                                           */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to initialize PDMACRC   	                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMACRC_Init(void)
{
    UNLOCKREG();
	/* Enable PDMA Clock */
	SYSCLK->AHBCLK.PDMA_EN  =1;
	LOCKREG();
	/* Enable PDMACRC Clock */
	PDMA->GCR.CRC_CLK_EN =1;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_Close	                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function disables PDMACRC clock and AHB PDMA clock                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMACRC_Close(uint32_t u32PDMAAHB)
{
   	/* Disable PDMACRC clock */
	PDMA->GCR.CRC_CLK_EN =0;
	/* Disable PDMA clock */
	if(u32PDMAAHB){
	    UNLOCKREG();
	    SYSCLK->AHBCLK.PDMA_EN  =0;
	    LOCKREG();
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_SetCTL                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 sCTLParam			-[in]   PDMACRC Control parameters                                     */
/*                                          It include of                                                  */
/*                                  u8ChannelEn     		- CRC Channel Enable                    	   */
/*                                  u8Reset 				- CRC Engine Reset							   */
/*                                  u8TrigEn				- Trig Enable            					   */
/*                                  u8WrDataReverse		    - Write Data Reverse                           */
/*                                  u8ChecksumReverse		- Checksum Reverse                             */
/*                                  u8WrDataComplement      - Write Data Complement                        */
/*                                  u8ChecksumComplement    - Checksum Complement                          */
/* Returns:                                                                                                */
/*               E_SUCCESS               Success                                                           */
/*               E_DRVPDMA_FALSE_INPUT   invalid Argument                                                  */
/* Description:                                                                                            */
/*               The function is used to set CRC Control parameters                                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_SetCTL(STR_PDMACRC_CTL_T *sCTLParam)
{
	if( (sCTLParam->u8ChannelEn > eDRVPDMACRC_ENABLE) ||
	    (sCTLParam->u8Reset > eDRVPDMACRC_ENABLE) ||
	    (sCTLParam->u8TrigEn > eDRVPDMACRC_ENABLE) ||
	    (sCTLParam->u8WrDataReverse > eDRVPDMACRC_BITORDER) ||
	    (sCTLParam->u8ChecksumReverse > eDRVPDMACRC_BITORDER) ||
	    (sCTLParam->u8WrDataComplement > eDRVPDMACRC_1sCOMPLEMENT) ||
	    (sCTLParam->u8ChecksumComplement > eDRVPDMACRC_1sCOMPLEMENT) ||
	    (sCTLParam->u8CpuWrDataLen > eDRVPDMACRC_32BIT) ||
	    (sCTLParam->u8CRCpolynomialMode > eDRVPDMACRC_CRC32) )

	    return E_DRVPDMA_FALSE_INPUT;

	else
	{
        PDMACRC->CTL.CRCCEN  = sCTLParam->u8ChannelEn;
        PDMACRC->CTL.CRC_RST = sCTLParam->u8Reset;
        PDMACRC->CTL.WDATA_RVS    = sCTLParam->u8WrDataReverse;
        PDMACRC->CTL.CHECKSUM_RVS = sCTLParam->u8ChecksumReverse;
        PDMACRC->CTL.WDATA_COM    = sCTLParam->u8WrDataComplement;
        PDMACRC->CTL.CHECKSUM_COM = sCTLParam->u8ChecksumComplement;
        PDMACRC->CTL.CPU_WDLEN    = sCTLParam->u8CpuWrDataLen;
        PDMACRC->CTL.CRC_MODE     = sCTLParam->u8CRCpolynomialMode;
        PDMACRC->CTL.TRIG_EN      = sCTLParam->u8TrigEn;
        return E_SUCCESS;
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_SetDMAsrcAddr         				                                           */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 u32SrcAddr				-[in]     PDMACRC DMA Transfer Source Address                      */
/* Returns:                                                                                                */
/*               E_SUCCESS               Success                                                           */
/* Description:                                                                                            */
/*               The function is used to set CRC DMA transfer source address                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_SetDMAsrcAddr(uint32_t u32SrcAddr)
{
	PDMACRC->DMASAR = u32SrcAddr;
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_SetTransferByteCount         				                                   */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 u32TrnBC				-[in]     PDMACRC DMA Transfer Byte Count                          */
/* Returns:                                                                                                */
/*               E_DRVPDMA_FALSE_INPUT   invalid Argument                                                  */
/*               E_SUCCESS               Success                                                           */
/* Description:                                                                                            */
/*               The function is used to set CRC DMA transfer byte count register                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_SetTransferByteCount(uint32_t u32TrnBC)
{
	if(u32TrnBC & 0xFF00)
	    return E_DRVPDMA_FALSE_INPUT;
	else
	{
		PDMACRC->DMABCR.CRC_DMABCR = u32TrnBC;
        return E_SUCCESS;
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetDMAcurrentSAR         				                                       */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 DMACSAR			      -[out]     PDMACRC DMA Current Source Address                    */
/* Description:                                                                                            */
/*               The function is used to get CRC DMA current source address                                */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetDMAcurrentSAR(void)
{
	return PDMACRC->DMACSAR;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetDMAcurrentBCR         				                                       */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 DMACBCR	      		  -[out]     PDMACRC DMA Current Byte Count                        */
/* Description:                                                                                            */
/*               The function is used to get CRC DMA current byte count                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetDMAcurrentBCR(void)
{
	return PDMACRC->DMACBCR.CRC_DMACBCR;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_SetINT			                                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*               u32BLKDINT	   			-[in] CRC DMA Transfer Done INT                                    */
/*               u32TABORTINT	   		-[in] CRC DMA Read/Write Target Abort INT                          */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVPDMA_FALSE_INPUT    invalid Argument                                                 */
/* Description:                                                                                            */
/*               The function is used to set interrupt register for CRC DMA                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_SetInt(uint32_t u32BLKDINT, uint32_t u32TABORTINT)
{
	if( (u32BLKDINT > eDRVPDMACRC_ENABLE) ||
	    (u32TABORTINT > eDRVPDMACRC_ENABLE) )

        return E_DRVPDMA_FALSE_INPUT;

    else
    {
    	PDMACRC->DMAIER.TABORT_IE = u32TABORTINT;
    	PDMACRC->DMAIER.BLKD_IE   = u32BLKDINT;
    }

	if( (u32BLKDINT == eDRVPDMACRC_DISABLE) &&
	    (u32TABORTINT == eDRVPDMACRC_DISABLE) )
    	NVIC_DisableIRQ(PDMA_IRQn);  	/* Disable Specified Interrupt */
    else
	    NVIC_EnableIRQ(PDMA_IRQn);  	/* Enable Specified Interrupt */

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetBLKD_INTflag         				                                       */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 BLKD_IF	    		 -[out]     PDMACRC DMA Block Transfer Done INT Flag               */
/* Description:                                                                                            */
/*               The function is used to get CRC DMA block transfer done interrupt flag                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetBLKD_INTflag(void)
{
	return PDMACRC->DMAISR.BLKD_IF;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetTABORT_INTflag         				                                       */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 TABORT_IF	    		 -[out]     PDMACRC DMA Read/Write Target Abort INT Flag           */
/* Description:                                                                                            */
/*               The function is used to get CRC DMA Read/Write target abort interrupt flag                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetTABORT_INTflag(void)
{
	return PDMACRC->DMAISR.TABORT_IF;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_ClearIF			                                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*               u32ClrBLKD_IF	   			-[in] CRC DMA Transfer Done INT Flag                           */
/*               u32ClrTABORT_IF	   		-[in] CRC DMA Read/Write Target Abort INT Flag                 */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/* Description:                                                                                            */
/*               The function is used to clear interrupt flag register for CRC DMA                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_ClearIF(uint32_t u32ClrBLKD_IF, uint32_t u32ClrTABORT_IF)
{
   	PDMACRC->DMAISR.TABORT_IF = u32ClrTABORT_IF;
   	PDMACRC->DMAISR.BLKD_IF   = u32ClrBLKD_IF;

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_SetWDATA         				                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 u32Wdata				  -[in]     PDMACRC Write Data                                     */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVPDMA_FALSE_INPUT    invalid Argument                                                 */
/* Description:                                                                                            */
/*               The function is used to set CRC write data when poerated in CPU PIO mode                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_SetWDATA(uint32_t u32Wdata)
{
	if (PDMACRC->CTL.TRIG_EN) //In DMA mode
	    return E_DRVPDMA_FALSE_INPUT;
	else
	{
	    PDMACRC->WDATA = u32Wdata;
        return E_SUCCESS;
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetWDATA         				                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 WDATA	 			  -[out]     PDMACRC Write Data                                        */
/* Description:                                                                                            */
/*               The function is used to Get CRC write data, it will be affected by WDATA_COM & WDATA_REV  */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetWDATA(void)
{
    return PDMACRC->WDATA;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_SetSEED         				                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 u32Seed				  -[in]     PDMACRC Seed Value                                     */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/* Description:                                                                                            */
/*               The function is used to set CRC seed value                                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_SetSEED(uint32_t u32Seed)
{
    PDMACRC->SEED = u32Seed;
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetSeed         				                                               */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 SEED	 			  -[out]     PDMACRC SEED                                              */
/* Description:                                                                                            */
/*               The function is used to Get CRC SEED                                                      */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetSEED(void)
{
    return PDMACRC->SEED;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetCheckSum         				                                           */
/*                                                                                                         */
/* Parameter:        																					   */
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*				 CHECKSUM	 			  -[out]     PDMACRC Checksum                                      */
/* Description:                                                                                            */
/*               The function is used to Get CRC Checksum                                                  */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMACRC_GetCheckSum(void)
{
    return PDMACRC->CHECKSUM;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMACRC_GetVersion                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */
/*	             None                                                                                 	   */
/* Returns:                                                                                                */
/*               Version Number                                                                            */
/* Description:                                                                                            */
/*               The function is used to get DrvPDMACRC Version Number                                     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMACRC_GetVersion(void)
{
	return DRVPDMACRC_VERSION_NUM;
}


