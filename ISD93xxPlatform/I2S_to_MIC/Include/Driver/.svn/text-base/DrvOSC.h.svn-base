/*---------------------------------------------------------------------------------------------------*/
/*                                                                                                   */
/* Copyright(c) 2011 Nuvoton Technology Corp. All rights reserved.                                   */
/*                                                                                                   */
/*---------------------------------------------------------------------------------------------------*/
#ifndef DRVOSC_H
#define DRVOSC_H

#include "isd93xx.h"

/*---------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                              */
/*---------------------------------------------------------------------------------------------------*/
#define	DRVOSC_MAJOR_NUM 2
#define	DRVOSC_MINOR_NUM 00
#define	DRVOSC_BUILD_NUM 1

#define DRVOSC_VERSION_NUM		_SYSINFRA_VERSION(DRVOSC_MAJOR_NUM,DRVOSC_MINOR_NUM,DRVOSC_BUILD_NUM)  

// Define some reserve range in oscillator trim so it doesn't trim to maximum fine settings.
// This may be used in I2S slave mode for synchronization purposes.
#define DRVOSC_RESERVE_RANGE	3
#define DRVOSC_MAX_COARSE   	7
#define DRVOSC_MAX_FINE     	31
#define DRVOSC_NUM_SFINE     	8
#define DRVOSC_NUM_CYCLES   	36
#define DRVOSC_NUM_CYCLES_SFT  	80
#define DRVOSC_MEAS_HCLK_DIV   	2

#if(DRVOSC_NUM_SFINE == 4)
#define sfine(x)  (((x)==3) ? 0xE : (x) )
#define rsfine(x) (((x)==0xE) ? 3 : (x) )
#else
#define sfine(x)  (((x)==7) ? 0xf :((x)==6) ? 0xE : ((x)==5) ? 0x7 :((x)==4) ? 0x6 :(x) )
#define rsfine(x) (((x)==0xf) ? 7 :((x)==0xe) ? 6 : ((x)==7) ? 5   :((x)==6) ? 4 :(x) )
#endif
/*---------------------------------------------------------------------------------------------------*/
/*  Define Error Code									             */
/*---------------------------------------------------------------------------------------------------*/
/* Error code */
//E_DRVOSC_COARSE_OVF			Osc trimming error during coarse range
//E_DRVOSC_MEAS_TIMEOUT			Oscillator measurement error. Reference clock not available?
//E_DRVOSC_FINE_OVF 			Osc trimming error during fine range
#define E_DRVOSC_COARSE_OVF         _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVOSC, 1)
#define E_DRVOSC_MEAS_TIMEOUT       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVOSC, 2)
#define E_DRVOSC_FINE_OVF           _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVOSC, 3)   

int32_t DrvOSC_TrimOscillator(int32_t target, int32_t index);

int32_t DrvOSC_MeasureOsc(int32_t hclk_div, int32_t num_cycles);
#endif

