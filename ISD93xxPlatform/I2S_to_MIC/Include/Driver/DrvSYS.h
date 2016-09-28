/*-----------------------------------------------------------------------------------*/
/*                                                                                   */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                   */
/*                                                                                   */
/*-----------------------------------------------------------------------------------*/
#ifndef DRVSYS_H
#define DRVSYS_H

/*-----------------------------------------------------------------------------------*/
/* Include related headers                                                           */
/*-----------------------------------------------------------------------------------*/
#include "isd93xx.h"
/*-----------------------------------------------------------------------------------*/
/*  Define Version number					                     */
/*-----------------------------------------------------------------------------------*/
#define DRVSYS_MAJOR_NUM 1
#define DRVSYS_MINOR_NUM 00
#define DRVSYS_BUILD_NUM 1

/*-----------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                     */
/*-----------------------------------------------------------------------------------*/
#define DRVSYS_VERSION_NUM      _SYSINFRA_VERSION(DRVSYS_MAJOR_NUM,DRVSYS_MINOR_NUM,DRVSYS_BUILD_NUM)  

/*-----------------------------------------------------------------------------------*/
/*  Define Error Code						                     */
/*-----------------------------------------------------------------------------------*/
//E_DRVSYS_ERR_UNLOCK_FAIL      Unlock key address failed
//E_DRVSYS_ERR_LOCK_FAIL      Lock key address failed
//E_DRVSYS_ERR_ARGUMENT       Wrong Argument
//E_DRVSYS_ERR_IPSRC          Wrong IP clock selection
//E_DRVSYS_ERR_IPDIV          Wrong IP divider selection
#define E_DRVSYS_ERR_UNLOCK_FAIL    _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 1)
#define E_DRVSYS_ERR_LOCK_FAIL    _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 2)
#define E_DRVSYS_ERR_ARGUMENT       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 3)    
#define E_DRVSYS_ERR_IPSRC      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 4)    
#define E_DRVSYS_ERR_IPDIV      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVSYS, 5)    

#define MIN_WAIT   E_DRVFMC_ONE_WAIT
#define wait_cfg(x) ( (x > 73728000) ? E_DRVFMC_THREE_WAIT : (x >  49152000) ? E_DRVFMC_TWO_WAIT : (x >  24576000) ? E_DRVFMC_ONE_WAIT : MIN_WAIT ) 
#define num_wait_states(x) ( (x & E_DRVFMC_TWO_WAIT) ? 2 : (x & E_DRVFMC_ONE_WAIT) ? 1 : (x & E_DRVFMC_FOUR_WAIT) ? 4 : 3 )

/*----------------------------------------------------------------------------------*/
/*  HCLK frequency selector                                                         */
/*----------------------------------------------------------------------------------*/
typedef enum 
{
    E_DRVSYS_48M = 0,
    E_DRVSYS_32M = 1,   
    E_DRVSYS_36M = 2,   
    E_DRVSYS_72M = 3,   
    E_DRVSYS_64M = 4,   
    E_DRVSYS_96M = 5,
    E_DRVSYS_32K = 6,
    E_DRVSYS_10K = 7   
}E_SYS_HCLK_SRC;

/*----------------------------------------------------------------------------------*/
/*  IP reset                                                                        */
/*----------------------------------------------------------------------------------*/
typedef enum 
{
    E_SYS_TMR0_RST  = 6,
    E_SYS_TMR1_RST  = 7,
    E_SYS_I2C0_RST  = 8,
    E_SYS_SPI0_RST  = 12,
    E_SYS_DPWM_RST  = 13,
    E_SYS_UART0_RST  = 16,
    E_SYS_BIQ_RST  = 18,
    E_SYS_CRC_RST  = 19,
    E_SYS_PWM03_RST  = 20,
    E_SYS_PWM47_RST  = 21,
    E_SYS_ACMP_RST  = 22,
    E_SYS_ADC_RST = 28,
    E_SYS_I2S_RST = 29,
    E_SYS_ANA_RST = 30,   
    E_SYS_PDMA_RST = 32
}E_SYS_IP_RST;

/*-----------------------------------------------------------------------------------*/
/*  SYS IP Clcok Selector                                                            */
/*-----------------------------------------------------------------------------------*/
typedef enum 
{
    E_SYS_WDG_CLK   = 1,
    E_SYS_RTC_CLK   = 5,    
    E_SYS_TMR0_CLK  = 6,
    E_SYS_TMR1_CLK  = 7,
    E_SYS_I2C0_CLK  = 8,
    E_SYS_SPI0_CLK  = 12,
    E_SYS_DPWM_CLK  = 13,
    E_SYS_UART0_CLK = 16,
    E_SYS_BIQ_CLK   = 18,   
    E_SYS_CRC_CLK   = 19,     
    E_SYS_PWM01_CLK = 20,
    E_SYS_PWM23_CLK = 21,
    E_SYS_ACMP_CLK  = 22,
    E_SYS_SBRAM_CLK = 26,  
    E_SYS_ADC_CLK   = 28,
    E_SYS_I2S_CLK   = 29,
    E_SYS_ANA_CLK   = 30,
    E_SYS_PWM45_CLK = 31,
    E_SYS_PDMA_CLK  = 33,
    E_SYS_ISP_CLK   = 34
}E_SYS_IP_CLK;


/*-----------------------------------------------------------------------------------*/
/*  SYS IP Clcok Divider                                                             */
/*-----------------------------------------------------------------------------------*/
typedef enum 
{
    E_SYS_ADC_DIV,
    E_SYS_UART_DIV,
    E_SYS_HCLK_DIV
}E_SYS_IP_DIV;


/*-----------------------------------------------------------------------------------*/
/*  SYS IP Clcok Selector                                                            */
/*-----------------------------------------------------------------------------------*/
typedef enum 
{
    E_SYS_I2S_CLKSRC,
    E_SYS_PWM45_CLKSRC,
    E_SYS_PWM23_CLKSRC,
    E_SYS_PWM01_CLKSRC,
    E_SYS_TMR1_CLKSRC,
    E_SYS_TMR0_CLKSRC,
    E_SYS_WDG_CLKSRC, 
    E_SYS_DPWM_CLKSRC 
}E_SYS_IP_CLKSRC;

/*--------------------------------------------------------------------------------*/
/*  SYS Oscillator Control                                                        */
/*--------------------------------------------------------------------------------*/
typedef enum 
{
    E_SYS_NONE,
    E_SYS_XTL32K,
    E_SYS_OSC49M,
    E_SYS_OSC10K
}E_SYS_OSC_CTRL;

/*---------------------------------------------------------------------------------------------------------*/
/* Define GCR functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvSYS_ReadProductID(void);
int32_t DrvSYS_UnlockKeyAddr(void);
int32_t DrvSYS_LockKeyAddr(void);
uint32_t DrvSYS_GetRstSrc(void);
uint32_t DrvSYS_ClearRstSrc(uint32_t u32Src);
void DrvSYS_ResetIP(E_SYS_IP_RST eIpRst);
void DrvSYS_ResetCPU(void);
void DrvSYS_ResetChip(void);
void DrvSYS_SetRCAdjValue(uint32_t u32Adj);
void DrvSYS_SetIPClock(E_SYS_IP_CLK eIpClk, int32_t i32Enable);
int32_t DrvSYS_SetHCLK(E_SYS_HCLK_SRC eHClkSrc, uint8_t u8HclkDiv);
int32_t DrvSYS_SetSysTickSource(uint8_t u8ClkSrcSel);
int32_t DrvSYS_SetIPClockSource(E_SYS_IP_CLKSRC eIpClkSrc, uint8_t u8ClkSrcSel);
int32_t DrvSYS_SetClockDivider(E_SYS_IP_DIV eIpDiv , int32_t i32value);
int32_t DrvSYS_SetOscCtrl(E_SYS_OSC_CTRL eOscCtrl, int32_t i32Enable);
uint32_t DrvSYS_GetEXTClock(void);
uint32_t DrvSYS_GetHCLK(void);
uint32_t DrvSYS_GetHCLKHz(void);
uint32_t DrvSYS_GetVersion(void);

#endif

