/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2011 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVTALARM_H__
#define __DRVTALARM_H__

#include "ISD93xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (PFN_DRVTALARM_CALLBACK)(void);

#define TALARM_SEL_145C    0x08
#define TALARM_SEL_135C    0x04
#define TALARM_SEL_125C    0x02
#define TALARM_SEL_115C    0x01
#define TALARM_SEL_105C    0x00

#define TALARM_EN_ENENABLE   0x01
#define TALARM_EN_ENDISABLE  0x00
#define TALARM_EN_IEENABLE   0x01
#define TALARM_EN_IEDISABLE  0x00
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVTALARM_MAJOR_NUM 1
#define DRVTALARM_MINOR_NUM 00
#define DRVTALARM_BUILD_NUM 1


#define DRVTALARM_VERSION_NUM    _SYSINFRA_VERSION(DRVTALARM_MAJOR_NUM, DRVTALARM_MINOR_NUM, DRVTALARM_BUILD_NUM)
/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code	                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Global interface variables declarations                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototype                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTALARM_SetTALARMselect(uint32_t u32LVL);
void DrvTALARM_EnableTALARM(uint32_t u32Enable);
uint32_t DrvTALARM_GetTALARMstatus(void);
void DrvTALARM_SetTALARMIE(uint32_t u32TALARMIE);
void DrvTALARM_InstallISR(PFN_DRVTALARM_CALLBACK callback);
int32_t DrvTALARM_GetVersion(void);

#endif	// __DRVTALARM_H__



