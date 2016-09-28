/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVBOD_H__
#define __DRVBOD_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "ISD93xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVBOD_MAJOR_NUM 1
#define DRVBOD_MINOR_NUM 00
#define DRVBOD_BUILD_NUM 1

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVBOD_VERSION_NUM    _SYSINFRA_VERSION(DRVBOD_MAJOR_NUM, DRVBOD_MINOR_NUM, DRVBOD_BUILD_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code	                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*PFN_DRVBOD_CALLBACK)(void);

typedef enum {
	eDRVBOD_21V = 0,
	eDRVBOD_22V = 1,
	eDRVBOD_24V = 2,
	eDRVBOD_25V = 3,
	eDRVBOD_265V = 4,
	eDRVBOD_28V = 5,
	eDRVBOD_30V = 6,
	eDRVBOD_46V = 7,
	eDRVBOD_31V = 8,
	eDRVBOD_32V = 9,
	eDRVBOD_34V = 10,
	eDRVBOD_36V = 11,
	eDRVBOD_37V = 12,
	eDRVBOD_38V = 13,
	eDRVBOD_39V = 14,
	eDRVBOD_42V = 15
} E_DRVBOD_LVL;

typedef enum {
	eDRVBOD_DISABLE,
	eDRVBOD_TIME_MULTIPLEXED,
	eDRVBOD_CONTINUOUS	
} E_DRVBOD_EN;

typedef enum {
	eDRVBOD_HYS_DISABLE,
	eDRVBOD_HYS_ENABLE
} E_DRVBOD_HYS;

/*---------------------------------------------------------------------------------------------------------*/
/* Global interface variables declarations                                                                 */                                                                            
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototype                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void BOD_IRQHandler(void);
void DrvBOD_SelectBODVolt(E_DRVBOD_LVL eLevel);
void DrvBOD_SelectBODHyst(E_DRVBOD_HYS eHYS);
void DrvBOD_EnableBOD(E_DRVBOD_EN eEnable);
void DrvBOD_EnableInt(PFN_DRVBOD_CALLBACK pfnBODCallBack);
void DrvBOD_DisableInt(void);
uint32_t DrvBOD_GetBODout(void);
void DrvBOD_SetDetectionTime(uint8_t u8OnDUR, uint16_t u16OffDUR);
int32_t DrvBOD_GetVersion(void);

#endif	// __DRVBOD_H__



