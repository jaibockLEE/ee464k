/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef DRVGPIO_H
#define DRVGPIO_H

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVGPIO_MAJOR_NUM	1
#define DRVGPIO_MINOR_NUM	00
#define DRVGPIO_BUILD_NUM	1

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVGPIO_VERSION_NUM     _SYSINFRA_VERSION(DRVGPIO_MAJOR_NUM, DRVGPIO_MINOR_NUM, DRVGPIO_BUILD_NUM)
							   
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
//E_DRVGPIO_ARGUMENT			Argument error 
//E_DRVGPIO_GROUP_PIN			Can't set bit function when it can't be set individually
//E_DRVGPIO_BUSY				GPIO is in useing
#define E_DRVGPIO_ARGUMENT          _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVGPIO, 1)
#define E_DRVGPIO_GROUP_PIN         _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVGPIO, 2)
#define E_DRVGPIO_BUSY              _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVGPIO, 3)   

typedef enum 
{
    GPA = 0,
    GPB = 1 
} DRVGPIO_PORT;

typedef void (*GPIO_GPAB_CALLBACK)(uint32_t u32GpaStatus, uint32_t u32GpbStatus);
typedef void (*GPIO_EINT0_CALLBACK)(void);
typedef void (*GPIO_EINT1_CALLBACK)(void);

/* Define GPIO PIN number */
#define		DRVGPIO_PIN15 	BIT15
#define		DRVGPIO_PIN14 	BIT14
#define 	DRVGPIO_PIN13 	BIT13
#define 	DRVGPIO_PIN12 	BIT12
#define 	DRVGPIO_PIN11 	BIT11
#define 	DRVGPIO_PIN10 	BIT10
#define 	DRVGPIO_PIN9 	BIT9
#define 	DRVGPIO_PIN8	BIT8
#define 	DRVGPIO_PIN7 	BIT7
#define 	DRVGPIO_PIN6 	BIT6
#define 	DRVGPIO_PIN5 	BIT5
#define 	DRVGPIO_PIN4 	BIT4
#define 	DRVGPIO_PIN3 	BIT3
#define 	DRVGPIO_PIN2 	BIT2
#define 	DRVGPIO_PIN1 	BIT1
#define 	DRVGPIO_PIN0 	BIT0

/*---------------------------------------------------------------------------------------------------------*/
/*  SYS IP Clcok Selector		     				                                                       */
/*---------------------------------------------------------------------------------------------------------*/		 
typedef enum {IO_INPUT=0 , IO_OUTPUT=1, IO_OPENDRAIN=2 , IO_QUASI=3} DRVGPIO_IO;
typedef enum {IO_PULL_UP=0 , IO_TRI=1} DRVGPIO_WAKE_IO;
typedef enum {IO_RISING, IO_FALLING, IO_BOTH_EDGE} DRVGPIO_INT_TYPE;
typedef enum {MODE_EDGE, MODE_LEVEL} DRVGPIO_INT_MODE;
typedef enum {DBCLKSRC_HCLK = 0 ,DBCLKSRC_10K =1} DRVGPIO_DBCLKSRC;	   

/*---------------------------------------------------------------------------------------------------------*/
/* Define GPIO functions prototype                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void GPAB_IRQHandler(void);
void EINT0_IRQHandler(void);
void EINT1_IRQHandler(void);
void DrvGPIO_SetIntCallback(GPIO_GPAB_CALLBACK pfGPABCallback);
int32_t DrvGPIO_Open(DRVGPIO_PORT port, uint32_t u32Bit, DRVGPIO_IO mode);
int32_t DrvGPIO_Close(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_SetPortBits(DRVGPIO_PORT port, uint16_t u16Data);
int32_t DrvGPIO_GetPortBits(DRVGPIO_PORT port);
int32_t DrvGPIO_GetBit(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_SetBit(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_ClrBit(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_GetDoutBit(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_GetPortDoutBits(DRVGPIO_PORT port);
int32_t DrvGPIO_EnableDebounce(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_SetDebounceTime(uint32_t u32DebounceClk, DRVGPIO_DBCLKSRC clockSource);
int32_t DrvGPIO_GetDebounceTime(void);
int32_t DrvGPIO_DisableDebounce(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_SetPortMask(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_SetBitMask(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_ClrBitMask(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_GetPortMask(DRVGPIO_PORT port);
int32_t DrvGPIO_EnableInt(DRVGPIO_PORT port, uint32_t u32Bit, DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode);
int32_t DrvGPIO_DisableInt(DRVGPIO_PORT port, uint32_t u32Bit);
void DrvGPIO_EnableEINT0(DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode, GPIO_EINT0_CALLBACK pfEINT0Callback);
void DrvGPIO_DisableEINT0(void);
void DrvGPIO_EnableEINT1(DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode, GPIO_EINT1_CALLBACK pfEINT1Callback);
void DrvGPIO_DisableEINT1(void);
uint32_t DrvGPIO_GetIntStatus(DRVGPIO_PORT port);
int32_t DrvGPIO_EnableSchmitt(DRVGPIO_PORT port, uint32_t u32Bit);
int32_t DrvGPIO_DisableSchmitt(DRVGPIO_PORT port, uint32_t u32Bit);
void DrvGPIO_WakeSetBit(void);
void DrvGPIO_WakeClrBit(void);
void DrvGPIO_WakeOpen(DRVGPIO_WAKE_IO mode);
int32_t DrvGPIO_GetWakeDoutBit(void);
void DrvGPIO_GPIOFunc(void);
void DrvGPIO_PWM0_PA12(void);
void DrvGPIO_PWM1_PA13(void);
void DrvGPIO_PWM01_PA12_PA13(void);
void DrvGPIO_PWM2_PB14(void);
void DrvGPIO_PWM2_PA8(void);
void DrvGPIO_PWM3_PB15(void);
void DrvGPIO_PWM3_PA9(void);
void DrvGPIO_PWM23_PB14_PB15(void);
void DrvGPIO_PWM4_PB12(void);
void DrvGPIO_PWM4_PA14(void);
void DrvGPIO_PWM5_PB13(void);
void DrvGPIO_PWM5_PA15(void);
void DrvGPIO_PWM45_PA14_PA15(void);
void DrvGPIO_PWM0B_PB4(void);
void DrvGPIO_PWM1B_PB5(void);
void DrvGPIO_PWM01B_PB4_PB5(void);
void DrvGPIO_PWM2B_PB15(void);
void DrvGPIO_PWM3B_PB14(void);
void DrvGPIO_PWM23B_PB14_PB15(void);
void DrvGPIO_PWM4B_PB13(void);
void DrvGPIO_PWM5B_PB12(void);
void DrvGPIO_PWM45B_PB12_PB13(void);
void DrvGPIO_I2C_PA10_PA11(void);
void DrvGPIO_I2C_PA1_PA3(void);
void DrvGPIO_I2C_PB2_PB3(void);
void DrvGPIO_I2C_SCL_PA11(void);
void DrvGPIO_I2C_SCL_PB2(void);
void DrvGPIO_I2C_SCL_PA1(void);
void DrvGPIO_I2C_SCL_PA5(void);
void DrvGPIO_I2C_SCL_PA7(void);
void DrvGPIO_I2C_SDA_PA10(void);
void DrvGPIO_I2C_SDA_PB3(void);
void DrvGPIO_I2C_SDA_PA3(void);
void DrvGPIO_I2C_SDA_PA4(void);
void DrvGPIO_I2S_PA4_PA5_PA6_PA7(void);
void DrvGPIO_I2S_PB8_PB9_PB10_PB11(void);
void DrvGPIO_I2S_FS_PA4(void);
void DrvGPIO_I2S_FS_PA12(void);
void DrvGPIO_I2S_FS_PB8(void);
void DrvGPIO_I2S_BCLK_PA5(void);
void DrvGPIO_I2S_BCLK_PA13(void);
void DrvGPIO_I2S_BCLK_PB9(void);
void DrvGPIO_I2S_SDI_PA6(void);
void DrvGPIO_I2S_SDI_PB6(void);
//void DrvGPIO_I2S_SDI_PB10(void);
void DrvGPIO_I2S_SDO_PA7(void);
void DrvGPIO_I2S_SDO_PB7(void);
void DrvGPIO_I2S_SDO_PB11(void);
void DrvGPIO_I2S_MCLK_PA0(void);
void DrvGPIO_I2S_MCLK_PB1(void);
void DrvGPIO_SPI_PA0_PA1_PA2_PA3(void);
void DrvGPIO_SPI_MOSI0_PA0(void);
void DrvGPIO_SPI_MOSI0_PB4(void);
void DrvGPIO_SPI_SCLK_PA1(void);
void DrvGPIO_SPI_SCLK_PB2(void);
void DrvGPIO_SPI_SSB0_PA2(void);
void DrvGPIO_SPI_SSB0_PB0(void);
void DrvGPIO_SPI_MISO0_PA3(void);
void DrvGPIO_SPI_MISO0_PB3(void);
void DrvGPIO_SPI_MOSI1_PA6(void);
void DrvGPIO_SPI_MOSI1_PB6(void);
void DrvGPIO_SPI_MOSI1_PB13(void);
void DrvGPIO_SPI_SSB1_PA5(void);
void DrvGPIO_SPI_SSB1_PB0(void);
void DrvGPIO_SPI_SSB1_PB1(void);
void DrvGPIO_SPI_MISO1_PA4(void);
void DrvGPIO_SPI_MISO1_PB5(void);
void DrvGPIO_SPI_MISO1_PB12(void);
void DrvGPIO_UART_TXRX_PA6_PA7(void);
void DrvGPIO_UART_TXRX_PA8_PA9(void);
void DrvGPIO_UART_TXRX_PB14_PB15(void);
void DrvGPIO_UART_RTSCTS_PA10_PA11(void);
void DrvGPIO_CMP_CMP0_PB0(void);
void DrvGPIO_CMP_CMP1_PB1(void);
void DrvGPIO_CMP_CMP2_PB2(void);
void DrvGPIO_CMP_CMP3_PB3(void);
void DrvGPIO_CMP_CMP4_PB4(void);
void DrvGPIO_CMP_CMP5_PB5(void);
void DrvGPIO_CMP_CMP6_PB6(void);
void DrvGPIO_CMP_CMP7_PB7(void);
void DrvGPIO_CMP_CMP8_PB8(void);
void DrvGPIO_CMP_CMP9_PB9(void);
void DrvGPIO_CMP_CMP10_PB10(void);
void DrvGPIO_CMP_CMP11_PB11(void);
void DrvGPIO_CMP_CMP12_PA8(void);
void DrvGPIO_CMP_CMP13_PA9(void);
void DrvGPIO_CMP_CMP14_PA10(void);
void DrvGPIO_CMP_CMP15_PA11(void);
void DrvGPIO_TIMER_TM0_PA14(void);
void DrvGPIO_TIMER_TM1_PA15(void);
void DrvGPIO_SPK_PA12_PA13(void);
void DrvGPIO_SPK_SPKP_PA12(void);
void DrvGPIO_SPK_SPKN_PA13(void);
void DrvGPIO_DMIC_PA14_PA15(void);
void DrvGPIO_DMIC_SDCLK_PA14(void);
void DrvGPIO_DMIC_SDIN_PA15(void);
int32_t DrvGPIO_GetVersion (void);

#endif








