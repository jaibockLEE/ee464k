/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVDPWM_H__
#define __DRVDPWM_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"
#include "System/SysInfra.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define	DRVDPWM_MAJOR_NUM 1
#define	DRVDPWM_MINOR_NUM 00
#define	DRVDPWM_BUILD_NUM 1

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVDPWM_VERSION_NUM    _SYSINFRA_VERSION(DRVDPWM_MAJOR_NUM, DRVDPWM_MINOR_NUM, DRVDPWM_BUILD_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define E_DRVDPWM_ARGUMENT           _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVDPWM, 1)

typedef int32_t	ERRCODE;

typedef unsigned char BOOL;

typedef enum {
	eDRVDPWM_FREQ_0, 
	eDRVDPWM_FREQ_1, 
	eDRVDPWM_FREQ_2, 
	eDRVDPWM_FREQ_3,
	eDRVDPWM_FREQ_4, 
	eDRVDPWM_FREQ_5, 
	eDRVDPWM_FREQ_6, 
	eDRVDPWM_FREQ_7  
}E_DRVDPWM_FREQ;

typedef enum {
	eDRVDPWM_DEADTIME_OFF,
	eDRVDPWM_DEADTIME_ON 
}E_DRVDPWM_DEADTIME;

typedef enum {
	E_DRVDPWM_DPWMCLK_HCLK,
	E_DRVDPWM_DPWMCLK_HCLKX2 
}E_DRVDPWM_DPWMCLK;

typedef enum {
	eDRVDPWM_DITHER_NONE=0,
	eDRVDPWM_DITHER_ONE=1, 
	eDRVDPWM_DITHER_TWO=3
}E_DRVDPWM_DITHERTYPE;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvDPWM_Open(void);
void DrvDPWM_Close(void);
void DrvDPWM_SetFrequency(E_DRVDPWM_FREQ eFrequency);
uint32_t DrvDPWM_GetFrequency(void);
void DrvDPWM_SetDeadTime(E_DRVDPWM_DEADTIME eDeadTime);
uint32_t DrvDPWM_GetDeadTime(void);
void DrvDPWM_Dither(E_DRVDPWM_DITHERTYPE eDitherType);
void DrvDPWM_Enable(void);
void DrvDPWM_Disable(void);
BOOL DrvDPWM_IsFIFOFull(void);
BOOL DrvDPWM_IsFIFOEmpty(void);
void DrvDPWM_EnablePDMA(void);
void DrvDPWM_DisablePDMA(void);
void DrvDPWM_WriteFIFO(int16_t *pi16Stream, uint32_t u32count);
uint32_t DrvDPWM_SetSampleRate(uint32_t u32SampleRate);
uint32_t DrvDPWM_GetSampleRate(void);
void DrvDPWM_SetDPWMClk(E_DRVDPWM_DPWMCLK eDpwmClk);
uint32_t DrvDPWM_GetVersion(void);

#endif
