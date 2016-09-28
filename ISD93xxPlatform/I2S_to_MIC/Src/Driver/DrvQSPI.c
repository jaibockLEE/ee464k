/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"
#include "core_cm0.h"
#include "DrvQSPI.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    BOOL bBusy;
    PFN_DRVSPI_CALLBACK pfncallback;
} S_DRVSPI_HANDLE;

static S_DRVSPI_HANDLE g_sSpiHandler[1];

static SPI_T * SPI_PORT[1]={SPI0};

/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function: SPI0_IRQHandler                                                                               */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    SPI0 interrupt handler.                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void SPI0_IRQHandler(void)
{
	uint32_t u32Status;
		
	/* Keep the interrupt source */
    u32Status = inpw(&SPI0->STATUS);
    
    /* Call the callback function of SPI interrupt */
    if(g_sSpiHandler[0].pfncallback != NULL)
    {
		g_sSpiHandler[0].pfncallback(u32Status);
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Open                                                                                        */
/*                                                                                                              */
/* Parameters:        	                                                                                        */
/*         eSpiPort     [in]: Specify the SPI port                                                              */
/*         eMode        [in]: Specify the operation mode (eDRVSPI_MASTER/eDRVSPI_SLAVE/eDRVSPI_JOYSTICK)        */
/*         eType        [in]: Specify the transfer type (eDRVSPI_TYPE0 ~ eDRVSPI_TYPE7)                         */
/*         i32BitLength [in]: Specify the bit length in a transaction (1~32)                                    */
/*                                                                                                              */
/* Returns:                                                                                                     */
/*         E_DRVSPI_ERR_INIT: The specified SPI port has been opened before.                                    */
/*         E_DRVSPI_ERR_BUSY: The specified SPI port is in busy status.                                         */
/*         E_DRVSPI_ERR_BIT_LENGTH: The specified bit length is out of range.                                   */
/*         E_SUCCESS: Success.                                                                                  */
/*                                                                                                              */
/* Description:                                                                                                 */
/*       Configure the operation mode, transfer type and bit length of a transaction of the specified SPI port. */
/*        The Timing of each SPI types:                                                                         */
/*
// DRVSPI_TYPE0:          
//             _________________________________
//    CS    __|                                 |___
//               _   _   _   _   _   _   _   _  
//    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//              
//    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//          
//    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//

// DRVSPI_TYPE1:          
//              _________________________________
//     CS    __|                                 |___
//                _   _   _   _   _   _   _   _  
//     CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//               
//     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//           
//     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
// 

//DRVSPI_TYPE2:          
//             _________________________________
//    CS    __|                                 |___
//               _   _   _   _   _   _   _   _  
//    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//              
//    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//          
//    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
//


// DRVSPI_TYPE3:          
//              _________________________________
//     CS    __|                                 |___
//                _   _   _   _   _   _   _   _  
//     CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
//               
//     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
//           
//     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---


// DRVSPI_TYPE4:          
//              __________________________________
//     CS    __|                                  |___
//            ___   _   _   _   _   _   _   _   ______ 
//     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//               
//    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
//           
//     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


// DRVSPI_TYPE5:
//              _________________________________
//     CS    __|                                 |____
//            ___   _   _   _   _   _   _   _   ______ 
//     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//               
//     Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//           
//     Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


// DRVSPI_TYPE6:
//              _________________________________
//     CS    __|                                 |____
//            ___   _   _   _   _   _   _   _   ______ 
//     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//               
//     Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
//           
//     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------


// DRVSPI_TYPE7:
//              _________________________________
//     CS    __|                                 |____
//            ___   _   _   _   _   _   _   _   ______ 
//     CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
//               
//     Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
//           
//     Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


Master / Slave Transfer Type Matching Table

DRVSPI_TYPE0 <==> DRVSPI_TYPE3
DRVSPI_TYPE1 <==> DRVSPI_TYPE1
DRVSPI_TYPE2 <==> DRVSPI_TYPE2
DRVSPI_TYPE3 <==> DRVSPI_TYPE0
DRVSPI_TYPE4 <==> DRVSPI_TYPE7
DRVSPI_TYPE5 <==> DRVSPI_TYPE5
DRVSPI_TYPE6 <==> DRVSPI_TYPE6
DRVSPI_TYPE7 <==> DRVSPI_TYPE4
*/
/*--------------------------------------------------------------------------------------------------------------*/
ERRCODE DrvSPI_Open(E_DRVSPI_PORT eSpiPort, E_DRVSPI_MODE eMode, E_DRVSPI_TRANS_TYPE eType, int32_t i32BitLength)
{
    int32_t i32TimeOut;

    if(g_sSpiHandler[eSpiPort].bBusy)
    {
        return E_DRVSPI_ERR_INIT;
    }

    if(eSpiPort == SPI_PORT0)
    {
        SYSCLK->APBCLK.SPI0_EN        =1;
        SYS->IPRSTC2.SPI0_RST         =1;
        SYS->IPRSTC2.SPI0_RST         =0;
    }
	
    /* Check busy*/
    i32TimeOut = 0x10000;
    while(SPI_PORT[eSpiPort]->STATUS.BUSY== 1)
    {
        if(i32TimeOut-- <= 0)
            return E_DRVSPI_ERR_BUSY;
    }
	
    g_sSpiHandler[eSpiPort].bBusy = TRUE;
    g_sSpiHandler[eSpiPort].pfncallback = NULL;
	
    /* Bit length 1 ~ 32 */
    if((i32BitLength < 1) || (i32BitLength > 32))
    {
        return E_DRVSPI_ERR_BIT_LENGTH;
    }
   
    /* "i32BitLength = 0" means 32 bits */
    if(i32BitLength == 32)
    {
        i32BitLength = 0;
    }
    SPI_PORT[eSpiPort]->CNTRL.DWIDTH = i32BitLength;

    if(eMode == eDRVSPI_SLAVE)
	{
        SPI_PORT[eSpiPort]->CNTRL.SLAVE = 1;
	}
		
    /* Transition types */
    if(eType==eDRVSPI_TYPE0)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE1)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE2)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }
    else if(eType==eDRVSPI_TYPE3)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 0;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }
    else if(eType==eDRVSPI_TYPE4)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE5)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 0;
    }
    else if(eType==eDRVSPI_TYPE6)
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 0;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }
    else
    {
        SPI_PORT[eSpiPort]->CNTRL.CLKP = 1;
        SPI_PORT[eSpiPort]->CNTRL.TX_NEG = 1;
        SPI_PORT[eSpiPort]->CNTRL.RX_NEG = 1;
    }	
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Close                                                                                  */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort [in]:  Specify the SPI port.                                                                */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/* Description:                                                                                            */
/*    Close the specified SPI module and disable the SPI interrupt.                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_Close(E_DRVSPI_PORT eSpiPort)
{
    int32_t i32TimeOut;
	
    g_sSpiHandler[eSpiPort].bBusy = FALSE;
    g_sSpiHandler[eSpiPort].pfncallback = NULL;

    /* Wait SPIMS Busy */
    i32TimeOut = 0x10000;
    while(SPI_PORT[eSpiPort]->STATUS.BUSY == 1)
    {
        if(i32TimeOut-- <= 0)
            break;
    }
   
    if(eSpiPort == SPI_PORT0)
    {
        NVIC_DisableIRQ(SPI0_IRQn);
        SYS->IPRSTC2.SPI0_RST=1;
        SYS->IPRSTC2.SPI0_RST=0;
        SYSCLK->APBCLK.SPI0_EN=0;
    } 
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Set2BitSerialDataIOMode                                                                           */
/*                                                                                                                    */
/* Parameters:        	                                                                                              */
/*    eSpiPort [in]: Specify the SPI port                                                                             */
/*    bEnable  [in]: Enable (TRUE) / Disable (FALSE)                                                                  */
/*                                                                                                                    */
/* Returns:                                                                                                           */
/*    None.                                                                                                           */
/*                                                                                                                    */
/* Description:                                                                                                       */
/*    Set 2-bit serial data I/O mode.                                                                                 */
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_Set2BitSerialDataIOMode(E_DRVSPI_PORT eSpiPort, BOOL bEnable)
{
    if(bEnable)
    {
        SPI_PORT[eSpiPort]->CNTRL.TWOB = 1;
    }
    else
    {
        SPI_PORT[eSpiPort]->CNTRL.TWOB = 0;
    }
}

/*------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetEndian                                                                                       */
/*                                                                                                                  */
/* Parameters:        	                                                                                            */
/*    eSpiPort [in]: Specify the SPI port                                                                           */
/*    eEndian  [in]: Specify LSB first or MSB first                                                                 */
/*                                                                                                                  */
/* Returns:                                                                                                         */
/*    None.                                                                                                         */
/*                                                                                                                  */
/* Description:                                                                                                     */
/*    Dertermine to transfer data with LSB first or MSB first                                                       */
/*------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetEndian(E_DRVSPI_PORT eSpiPort, E_DRVSPI_ENDIAN eEndian)
{
    if(eEndian == eDRVSPI_LSB_FIRST)
    {
        SPI_PORT[eSpiPort]->CNTRL.LSB = 1;
    }
    else
    {
        SPI_PORT[eSpiPort]->CNTRL.LSB = 0;
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetBitLength                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort     [in]: Specify the SPI port                                                              */
/*    i32BitLength [in]: Specify the bit length (1~32 bits)                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    E_SUCCESS:                 Success.                                                                  */
/*    E_DRVSPI_ERR_BIT_LENGTH: The bit length is out of range.                                             */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the bit length of SPI transfer.                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
ERRCODE DrvSPI_SetBitLength(E_DRVSPI_PORT eSpiPort, int32_t i32BitLength)
{
    if((i32BitLength < 1) || (i32BitLength > 32))
    {
        return E_DRVSPI_ERR_BIT_LENGTH;
    }
    if(i32BitLength == 32)
        i32BitLength = 0;

    SPI_PORT[eSpiPort]->CNTRL.DWIDTH = i32BitLength;
	
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetByteReorder                                                                         */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort     [in]: Specify the SPI port                                                              */
/*    bEnable      [in]: Enable (TRUE) / Disable (FALSE)                                                   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    E_SUCCESS:                 Success.                                                                  */
/*    E_DRVSPI_ERR_BIT_LENGTH: The bit length is not 16-, 24- or 32-bit.                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable/disable Byte Reorder function.                                                                */
/*    The Byte Reorder function is supported only in 16-, 24- and 32-bit transaction mode.                 */
/*---------------------------------------------------------------------------------------------------------*/
ERRCODE DrvSPI_SetByteEndian(E_DRVSPI_PORT eSpiPort, BOOL bEnable)
{
    if( (SPI_PORT[eSpiPort]->CNTRL.DWIDTH) % 8 )
        return E_DRVSPI_ERR_BIT_LENGTH;

    if(bEnable)
        SPI_PORT[eSpiPort]->CNTRL.REORDER = 1;
    else
        SPI_PORT[eSpiPort]->CNTRL.REORDER = 0;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetSuspendCycle                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort    [in]: Specify the SPI port.                                                              */
/*    i32Interval [in]: In burst transfer mode, this value specified the delay clocks between successive   */
/*                      transactions. If the Byte Suspend function is enabled, it specified the delay      */
/*                      clocks among each byte. It could be 2~17 which indicate 0.5~15.5 SPI clock cycles. */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    E_DRVSPI_ERR_SUSPEND_INTERVAL: The suspend interval setting is out of range.                         */
/*    E_SUCCESS: Success.                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the number of clock cycle of the suspend interval. Only for master mode.                         */
/*    The suspend cycle setting is shared with burst mode and byte suspend function.                       */
/*---------------------------------------------------------------------------------------------------------*/
ERRCODE DrvSPI_SetSuspendCycle(E_DRVSPI_PORT eSpiPort, int32_t i32Interval)
{
    /* In burst mode and byte suspend function, it could be 0 ~ 15. */
    if((i32Interval < 0) || (i32Interval > 15))
    {
        /* If out of range, specify the maximum suspend cycle and return error code. */
        SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = 15;
        return E_DRVSPI_ERR_TRANSMIT_INTERVAL;
    }
    SPI_PORT[eSpiPort]->CNTRL.SP_CYCLE = i32Interval;
    return E_SUCCESS;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetSlaveSelectActiveLevel                                                               */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort   [in]: Specify the SPI port                                                                 */
/*    eSSActType [in]: Select the active type of slave select pin.                                          */
/*                     eDRVSPI_ACTIVE_LOW_FALLING: Slave select pin is active low in level-trigger mode;    */
/*                                                 or falling-edge trigger in edge-trigger mode.            */
/*                     eDRVSPI_ACTIVE_HIGH_RISING: Slave select pin is active high in level-trigger mode;   */
/*                                                 or rising-edge trigger in edge-trigger mode.             */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    None.                                                                                                 */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Set the active level of slave select.                                                                    */
/*----------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetSlaveSelectActiveLevel(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SS_ACT_TYPE eSSActType)
{
    SPI_PORT[eSpiPort]->SSR.SS_LVL = eSSActType;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableAutoCS                                                                            */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort   [in]: Specify the SPI port                                                                 */
/*    eSlaveSel  [in]: Select the slave select pins which will be used.                                     */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    None.                                                                                                 */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Enable the automatic slave select function and set the specified slave select pin.                    */
/*----------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableAutoCS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel)
{
    SPI_PORT[eSpiPort]->SSR.AUTO_SS = 1;
    SPI_PORT[eSpiPort]->SSR.SSR = eSlaveSel;
}

/*----------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableAutoCS                                                                           */
/*                                                                                                          */
/* Parameters:        	                                                                                    */
/*    eSpiPort   [in]: Specify the SPI port                                                                 */
/*                                                                                                          */
/* Returns:                                                                                                 */
/*    None.                                                                                                 */
/*                                                                                                          */
/* Description:                                                                                             */
/*    Disable the automatic slave Select function and deselect slave select pins.                            */
/*----------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableAutoCS(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.AUTO_SS = 0;
    SPI_PORT[eSpiPort]->SSR.SSR = eDRVSPI_NONE;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetCS                                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*   eSpiPort   [in]: Specify the SPI port                                                                           */
/*   eSlaveSel  [in]: In automatic slave select operation, to use this parameter to select the slave select pins     */
/*                    which will be used.                                                                            */
/*                    In manual slave select operation, the specified slave select pins will be set to active state. */
/*                    It could be eDRVSPI_NONE, eDRVSPI_SS0, eDRVSPI_SS1 or eDRVSPI_SS0_SS1.                         */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Configure the slave select pins.                                                                               */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetCS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel)
{
    SPI_PORT[eSpiPort]->SSR.SSR = eSlaveSel;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrCS                                                                                            */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]: Specify the SPI port                                                                           */
/*    eSlaveSel [in]: Specify slave select pins                                                                      */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Set the specified slave select pins to inactive state.                                                         */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClrCS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel)
{
    uint32_t u32Reg;
	
    u32Reg = SPI_PORT[eSpiPort]->SSR.SSR;
    u32Reg = u32Reg & (~eSlaveSel);
    SPI_PORT[eSpiPort]->SSR.SSR = u32Reg;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetClock                                                                                         */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]: Specify the SPI port                                                                           */
/*    u32Clock  [in]: Specify the SPI clock rate in Hz.                                                              */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The actual value of divisor 1 is returned. SPI engine clock rate: APB clock rate / (divisor + 1)               */	  	    
/*    The actual clock may be different to the target SPI clock due to hardware limitation.                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Configure the SPI clock.                                                                                       */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_SetClock(E_DRVSPI_PORT eSpiPort, uint32_t u32DesiredClockFreq)
{
    int32_t i32Div;
	uint32_t u32Pclk;

	u32Pclk = DrvSYS_GetHCLKHz();
   
    if(u32DesiredClockFreq!=0)
    { 
        i32Div = (2 * u32Pclk) / u32DesiredClockFreq;
        i32Div += 1;
        i32Div >>= 1;
        i32Div -= 1;
        
        if(i32Div > 255)
            i32Div = 255;
        if(i32Div < 0)
            i32Div = 0;
        SPI_PORT[eSpiPort]->DIVIDER.DIVIDER = i32Div;
        
        return i32Div;
    }
    else
        return 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetClock                                                                                         */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort  [in]: Specify the SPI port                                                                           */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    The actual frquency is returned. SPI engine clock rate: APB clock rate / (divisor + 1)                         */	  	    
/*    The actual clock may be different to the target SPI clock due to hardware limitation.                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Get the SPI clock.                                                                                       */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_GetClock(E_DRVSPI_PORT eSpiPort)
{
    uint32_t u32SrcClock, u32Div;

	u32SrcClock = DrvSYS_GetHCLKHz();
	u32Div = SPI_PORT[eSpiPort]->DIVIDER.DIVIDER;

	return (u32SrcClock	/(u32Div+1));

}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableInt                                                                                        */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort    [in]: Specify the SPI port                                                                         */
/*    pfnCallback [in]: The callback function of SPI interrupt.                                                      */
/*    eSpiCallType[in]:	To install GPIO interrrupt callback functions                                                */
/*                      eDRVSPI_UNIT/eDRVSPI_SSINA/eDRVSPI_SSACT/eDRVSPI_SLVER1/eDRVSPI_SLVER0                       */
/*							eDRVSPI_SLVTO/eDRVSPI_TXUDR/eDRVSPI_RXOVR/eDRVSPI_RXTO/eDRVSPI_TXTH/eDRVSPI_RXTH            */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Enable the SPI interrupt of the specified SPI port and install the callback function.                          */
/* Example:                                                                                                          */
/*    DrvSPI_EnableInt(eDRVSPI_PORT0, eDRVSPI_UNIT|eDRVSPI_SSACT|eDRVSPI_RXOVR);                                     */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableInt(E_DRVSPI_PORT eSpiPort, uint16_t eSpiCallType, PFN_DRVSPI_CALLBACK pfnCallback)
{
    g_sSpiHandler[eSpiPort].pfncallback = pfnCallback;

    if((eSpiCallType & eDRVSPI_UNIT) == eDRVSPI_UNIT)
        SPI_PORT[eSpiPort]->CNTRL.UNIT_INTEN = 1;
	if((eSpiCallType & eDRVSPI_SSINA) == eDRVSPI_SSINA)
		SPI_PORT[eSpiPort]->SSR.SS_INACTIVE_IE = 1;
	if((eSpiCallType & eDRVSPI_SSACT) == eDRVSPI_SSACT)
		SPI_PORT[eSpiPort]->SSR.SS_ACTIVE_IE = 1;
	if((eSpiCallType & eDRVSPI_SLVER1) == eDRVSPI_SLVER1)
		SPI_PORT[eSpiPort]->SSR.SLAVE_ERROR1_IE = 1;
	if((eSpiCallType & eDRVSPI_SLVER0) == eDRVSPI_SLVER0)
		SPI_PORT[eSpiPort]->SSR.SLAVE_ERROR0_IE = 1;
	if((eSpiCallType & eDRVSPI_SLVTO) == eDRVSPI_SLVTO)
		SPI_PORT[eSpiPort]->SSR.SLAVE_TIMEOUT_IE = 1;
	if((eSpiCallType & eDRVSPI_TXUDR) == eDRVSPI_TXUDR)
		SPI_PORT[eSpiPort]->FIFO_CTRL.TXUDR_INTEN = 1;
	if((eSpiCallType & eDRVSPI_RXOVR) == eDRVSPI_RXOVR)
		SPI_PORT[eSpiPort]->FIFO_CTRL.RXOVR_INTEN = 1;
    if((eSpiCallType & eDRVSPI_RXTO) == eDRVSPI_RXTO)
		SPI_PORT[eSpiPort]->FIFO_CTRL.RXTO_INTEN = 1;
    if((eSpiCallType & eDRVSPI_TXTH) == eDRVSPI_TXTH)
		SPI_PORT[eSpiPort]->FIFO_CTRL.TXTH_INTEN = 1;
    if((eSpiCallType & eDRVSPI_RXTH) == eDRVSPI_RXTH)
		SPI_PORT[eSpiPort]->FIFO_CTRL.RXTH_INTEN = 1;
	
    if(eSpiPort == SPI_PORT0)
    {
        NVIC_EnableIRQ(SPI0_IRQn);
    }
}	

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableInt                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*    eSpiCallType[in]:	eDRVSPI_UNIT/eDRVSPI_SSINA/eDRVSPI_SSACT/eDRVSPI_SLVER1/eDRVSPI_SLVER0                       */
/*							  /eDRVSPI_SLVTO/eDRVSPI_TXUDR/eDRVSPI_RXOVR/eDRVSPI_RXTO/eDRVSPI_TXTH/eDRVSPI_RXTH            */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Disable the SPI interrupt.                                                                                     */
/* Example:                                                                                                          */
/*    DrvSPI_DisableInt(eDRVSPI_PORT0, eDRVSPI_UNIT|eDRVSPI_SSACT|eDRVSPI_RXOVR);                                    */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableInt(E_DRVSPI_PORT eSpiPort, uint16_t eSpiCallType)
{
    if((eSpiCallType & eDRVSPI_UNIT) == eDRVSPI_UNIT)
        SPI_PORT[eSpiPort]->CNTRL.UNIT_INTEN = 0;
	if((eSpiCallType & eDRVSPI_SSINA) == eDRVSPI_SSINA)
		SPI_PORT[eSpiPort]->SSR.SS_INACTIVE_IE = 0;
	if((eSpiCallType & eDRVSPI_SSACT) == eDRVSPI_SSACT)
		SPI_PORT[eSpiPort]->SSR.SS_ACTIVE_IE = 0;
	if((eSpiCallType & eDRVSPI_SLVER1) == eDRVSPI_SLVER1)
		SPI_PORT[eSpiPort]->SSR.SLAVE_ERROR1_IE = 0;
	if((eSpiCallType & eDRVSPI_SLVER0) == eDRVSPI_SLVER0)
		SPI_PORT[eSpiPort]->SSR.SLAVE_ERROR0_IE = 0;
	if((eSpiCallType & eDRVSPI_SLVTO) == eDRVSPI_SLVTO)
		SPI_PORT[eSpiPort]->SSR.SLAVE_TIMEOUT_IE = 0;
	if((eSpiCallType & eDRVSPI_TXUDR) == eDRVSPI_TXUDR)
		SPI_PORT[eSpiPort]->FIFO_CTRL.TXUDR_INTEN = 0;
	if((eSpiCallType & eDRVSPI_RXOVR) == eDRVSPI_RXOVR)
		SPI_PORT[eSpiPort]->FIFO_CTRL.RXOVR_INTEN = 0;
    if((eSpiCallType & eDRVSPI_RXTO) == eDRVSPI_RXTO)
		SPI_PORT[eSpiPort]->FIFO_CTRL.RXTO_INTEN = 0;
    if((eSpiCallType & eDRVSPI_TXTH) == eDRVSPI_TXTH)
		SPI_PORT[eSpiPort]->FIFO_CTRL.TXTH_INTEN = 0;
    if((eSpiCallType & eDRVSPI_RXTH) == eDRVSPI_RXTH)
		SPI_PORT[eSpiPort]->FIFO_CTRL.RXTH_INTEN = 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClearIntFlag                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*    eSpiCallType[in]:	eDRVSPI_UNIT_FLAG_CLR/eDRVSPI_SSINA_FLAG_CLR/eDRVSPI_SSACT_FLAG_CLR   				     	*/
/*                       eDRVSPI_TXUDR_FLAG_CLR/eDRVSPI_RXOVR_FLAG_CLR/eDRVSPI_RXTO_FLAG_CLR                         */        
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Disable the SPI interrupt.                                                                                     */
/* Example:                                                                                                          */
/*    DrvSPI_ClearIntFlag(eDRVSPI_PORT0, eDRVSPI_UNIT_FLAG_CLR|eDRVSPI_SSINA_FLAG_CLR|eDRVSPI_SSACT_FLAG_CLR);                                    */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClearIntFlag(E_DRVSPI_PORT eSpiPort, uint16_t eSpiCallType)
{	
    if((eSpiCallType & eDRVSPI_UNIT_FLAG_CLR) == eDRVSPI_UNIT_FLAG_CLR)
        //SPI_PORT[eSpiPort]->STATUS.UNIT_INTSTS = 1;  
		outpw(&(SPI_PORT[eSpiPort]->STATUS), 0x00000002);
    if((eSpiCallType & eDRVSPI_SSINA_FLAG_CLR) == eDRVSPI_SSINA_FLAG_CLR)  	
	    //SPI_PORT[eSpiPort]->STATUS.SSINA_INTSTS = 1;
		outpw(&(SPI_PORT[eSpiPort]->STATUS), 0x00000008);
	if((eSpiCallType & eDRVSPI_SSACT_FLAG_CLR) == eDRVSPI_SSACT_FLAG_CLR) 
	    //SPI_PORT[eSpiPort]->STATUS.SSACT_INTSTS = 1;
		outpw(&(SPI_PORT[eSpiPort]->STATUS), 0x00000004);
	if((eSpiCallType & eDRVSPI_TXUDR_FLAG_CLR) == eDRVSPI_TXUDR_FLAG_CLR) 
	     //SPI_PORT[eSpiPort]->STATUS.TXUDR_INTSTS = 1;
		 outpw(&(SPI_PORT[eSpiPort]->STATUS), 0x00080000);
	if((eSpiCallType & eDRVSPI_RXOVR_FLAG_CLR) == eDRVSPI_RXOVR_FLAG_CLR) 
	     //SPI_PORT[eSpiPort]->STATUS.RXOVR_INTSTS = 1;
		 outpw(&(SPI_PORT[eSpiPort]->STATUS), 0x00000800);
	if((eSpiCallType & eDRVSPI_RXTO_FLAG_CLR) == eDRVSPI_RXTO_FLAG_CLR) 
	     //SPI_PORT[eSpiPort]->STATUS.RXTO_INTSTS = 1;
		 outpw(&(SPI_PORT[eSpiPort]->STATUS), 0x00001000);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetIntFlag                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*    eSpiCallType[in]:	eDRVSPI_UNIT/eDRVSPI_SSINA/eDRVSPI_SSACT/eDRVSPI_SLVER1/eDRVSPI_SLVER0                       */
/*										  /eDRVSPI_SLVTO/eDRVSPI_TXUDR/eDRVSPI_RXOVR/eDRVSPI_RXTO/eDRVSPI_TXTH/eDRVSPI_RXTH            */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    0: None                                                                                                        */
/*    1: interrupt occcurs                                                                                           */
/* Description:                                                                                                      */
/*    The function is used to get the interrupt flag                                                                 */
/*-------------------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_GetIntFlag(E_DRVSPI_PORT eSpiPort, E_DRVSPI_CALLBACK_TYPE eSpiCallType)
{	
    if(eSpiCallType == eDRVSPI_UNIT)
        return (SPI_PORT[eSpiPort]->STATUS.UNIT_INTSTS);  
    else if(eSpiCallType == eDRVSPI_SSINA)  	
	    return (SPI_PORT[eSpiPort]->STATUS.SSINA_INTSTS);
	else if(eSpiCallType == eDRVSPI_SSACT) 
	    return (SPI_PORT[eSpiPort]->STATUS.SSACT_INTSTS);
	else if(eSpiCallType == eDRVSPI_SLVER1) 
	    return (SPI_PORT[eSpiPort]->STATUS.SLVER1_INTSTS);
	else if(eSpiCallType == eDRVSPI_SLVER0) 
	    return (SPI_PORT[eSpiPort]->STATUS.SLVER0_INTSTS);
	else if(eSpiCallType == eDRVSPI_SLVTO) 
	    return (SPI_PORT[eSpiPort]->STATUS.SLVTO_INTSTS);
	else if(eSpiCallType == eDRVSPI_TXUDR) 
	    return (SPI_PORT[eSpiPort]->STATUS.TXUDR_INTSTS);
	else if(eSpiCallType == eDRVSPI_RXOVR) 
	    return (SPI_PORT[eSpiPort]->STATUS.RXOVR_INTSTS);
	else if(eSpiCallType == eDRVSPI_RXTO) 
	    return (SPI_PORT[eSpiPort]->STATUS.RXTO_INTSTS);
	else if(eSpiCallType == eDRVSPI_TXTH) 
	    return (SPI_PORT[eSpiPort]->STATUS.TXTH_INTSTS);
	else //if(eSpiCallType == eDRVSPI_RXTH) 
	    return (SPI_PORT[eSpiPort]->STATUS.RXTH_INTSTS);

}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Status                                                                                           */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    Status                                                                                                         */
/* Description:                                                                                                      */
/*    The function is used to get the status                                                                         */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_Status(E_DRVSPI_PORT eSpiPort)
{	   
    return (inpw(&SPI_PORT[eSpiPort]->STATUS));  
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ReadRx                                                                                           */
/*                                                                                                                   */
/* Parameters:                                                                                                       */
/*    eSpiPort   [in]:  Specify the SPI port                                                                         */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    Rx                                                                                                             */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Get the SPI Rx data.                                                                                           */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_ReadRx(E_DRVSPI_PORT eSpiPort)
{
    return (SPI_PORT[eSpiPort]->RX);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_WriteTx                                                                                          */
/*                                                                                                                   */
/* Parameters:                                                                                                       */
/*    eSpiPort   [in]:  Specify the SPI port                                                                         */
/*    u32Data   [in]:  Store the data which will be transmitted through the SPI bus.                                 */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Write data to the SPI Tx.                                                                                      */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_WriteTx(E_DRVSPI_PORT eSpiPort, uint32_t u32Data)
{
    SPI_PORT[eSpiPort]->TX = u32Data;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableSPITransfer                                                                                */
/*                                                                                                                   */
/* Parameters:                                                                                                       */
/*    eSpiPort     [in]:  Specify the SPI port                                                                       */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Enable SPI transfer function                                                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableSPITransfer(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.SPIEN = 1;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableSPITransfer                                                                               */
/*                                                                                                                   */
/* Parameters:                                                                                                       */
/*    eSpiPort     [in]:  Specify the SPI port                                                                       */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Disable SPI transfer function                                                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableSPITransfer(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.SPIEN = 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_StartPDMA                                                                                        */
/*                                                                                                                   */
/* Parameters:                                                                                                       */
/*    eSpiPort     [in]: Specify the SPI port                                                                        */
/*    eDmaMode     [in]: Specify the DMA mode.                                                                       */
/*    bEnable      [in]: TRUE  -- Enable DMA;                                                                        */
/*                       FALSE -- Disable DMA.                                                                       */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Configure the DMA settings.                                                                                    */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_StartPDMA(E_DRVSPI_PORT eSpiPort, E_DRVSPI_DMA_MODE eDmaMode, BOOL bEnable)
{
    if(eDmaMode==eDRVSPI_TX_DMA)
    {
        if(bEnable)
            SPI_PORT[eSpiPort]->DMA.TX_DMA_GO = 1;
        else
            SPI_PORT[eSpiPort]->DMA.TX_DMA_GO = 0;
    }
    else
    {
        if(bEnable)
            SPI_PORT[eSpiPort]->DMA.RX_DMA_GO = 1;
        else
            SPI_PORT[eSpiPort]->DMA.RX_DMA_GO = 0;
    }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_PDMAReset                                                                                    */
/*                                                                                                                   */
/* Parameters:                                                                                                       */
/*    eSpiPort     [in]: Specify the SPI port                                                                        */
/*    eDmaMode     [in]: Specify the DMA mode.                                                                       */
/*    bEnable      [in]: TRUE  -- Enable DMA;                                                                        */
/*                       FALSE -- Disable DMA.                                                                       */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Reset the DMA settings.                                                                                    */
/*-------------------------------------------------------------------------------------------------------------------*/
void DrvSPI_PDMAReset(E_DRVSPI_PORT eSpiPort)
{
    uint32_t u32Timeout;

	u32Timeout = 0x100000;
    SPI_PORT[eSpiPort]->DMA.DMA_RST = 1;
	while(SPI_PORT[eSpiPort]->DMA.DMA_RST != 0 && u32Timeout) u32Timeout--;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_IsRxEmpty                                                                                        */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  Rx buffer empty.                                                                                        */
/*    FALSE: Rx buffer is not empty.                                                                                 */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Check the status of the Rx buffer of the specified SPI port.                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
BOOL DrvSPI_IsRxEmpty(E_DRVSPI_PORT eSpiPort)
{
    return ((SPI_PORT[eSpiPort]->STATUS.RX_EMPTY)?TRUE:FALSE);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_IsRxFull                                                                                         */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  Rx buffer full.                                                                                         */
/*    FALSE: Rx buffer is not full.                                                                                  */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Check the status of the Rx buffer of the specified SPI port.                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
BOOL DrvSPI_IsRxFull(E_DRVSPI_PORT eSpiPort)
{
    return ((SPI_PORT[eSpiPort]->STATUS.RX_FULL)?TRUE:FALSE);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_IsTxEmpty                                                                                        */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  Tx buffer empty.                                                                                        */
/*    FALSE: Tx buffer is not empty.                                                                                 */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Check the status of the Tx buffer of the specified SPI port.                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
BOOL DrvSPI_IsTxEmpty(E_DRVSPI_PORT eSpiPort)
{
    return ((SPI_PORT[eSpiPort]->STATUS.TX_EMPTY)?TRUE:FALSE);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_IsTxFull                                                                                         */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    TRUE:  Tx buffer full.                                                                                         */
/*    FALSE: Tx buffer is not full.                                                                                  */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*    Check the status of the Tx buffer of the specified SPI port.                                                   */
/*-------------------------------------------------------------------------------------------------------------------*/
BOOL DrvSPI_IsTxFull(E_DRVSPI_PORT eSpiPort)
{
    return ((SPI_PORT[eSpiPort]->STATUS.TX_FULL)?TRUE:FALSE);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Enable3Wire                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the SPI 3-wire function. In master mode, executing this function is unmeaningful.             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_Enable3Wire(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SLAVE_3WIRE = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_Disable3Wire                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the SPI 3-wire function.                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_Disable3Wire(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SLAVE_3WIRE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableRxMode                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the FIFO receive mode                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableRxMode(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.RX_MODE_EN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableRxMode                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the FIFO receive mode                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableRxMode(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->CNTRL.RX_MODE_EN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetRxTransCnt                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    eSpiPort     [in]: Specify the SPI port                                                              */
/*    bEnable      [in]: Enable (TRUE) / Disable (FALSE)                                                   */
/*    u16BitLength [in]: Specify the bit length (0~16 bits)                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable and set the RX receive transaction count                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetRxTransCnt(E_DRVSPI_PORT eSpiPort, BOOL bEnable, uint16_t u16BitLength)
{
	SPI_PORT[eSpiPort]->RX_TRANS_CNT = u16BitLength;
    SPI_PORT[eSpiPort]->CNTRL.RX_TRANS_CNT_EN = bEnable;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_IoMode                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*    eMode      [in]: Specify the mode                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable quad or dual or standard mode                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_IoMode(E_DRVSPI_PORT eSpiPort, E_DRVSPI_STANDARD_QUAD_DUAL_MODE eMode)
{
	if(eMode == eDRVSPI_STANDARD)
	{
	   SPI_PORT[eSpiPort]->CNTRL.QUAD_IO_EN = 0;
       SPI_PORT[eSpiPort]->CNTRL.DUAL_IO_EN = 0;
	}
	else if(eMode == eDRVSPI_QUAD)
	{
       SPI_PORT[eSpiPort]->CNTRL.QUAD_IO_EN = 1;
       SPI_PORT[eSpiPort]->CNTRL.DUAL_IO_EN = 0;
	}
	else // eDRVSPI_DUAL
    {
       SPI_PORT[eSpiPort]->CNTRL.QUAD_IO_EN = 0;
       SPI_PORT[eSpiPort]->CNTRL.DUAL_IO_EN = 1;	
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_QuadDualIoDir                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*    eQDIoDir   [in]: Specify the Direction                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set io direction                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_QuadDualIoDir(E_DRVSPI_PORT eSpiPort, E_DRVSPI_QD_IO_DIR eQDIoDir)
{
    SPI_PORT[eSpiPort]->CNTRL.QD_IO_DIR = eQDIoDir;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetTxFifoCount                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Indicates the valid data count of transmit FIFO buffer                                               */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_GetTxFifoCount(E_DRVSPI_PORT eSpiPort)
{
    return (SPI_PORT[eSpiPort]->STATUS.TX_FIFO_COUNT);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetRxFifoCount                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Indicates the valid data count of receive FIFO buffer                                               */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DrvSPI_GetRxFifoCount(E_DRVSPI_PORT eSpiPort)
{
    return (SPI_PORT[eSpiPort]->STATUS.RX_FIFO_COUNT);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrTxFifo                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    This function will clear all valid data of Tx FIFO                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClrTxFifo(E_DRVSPI_PORT eSpiPort)
{
	uint32_t u32Timeout;

	u32Timeout = 0x100000;
    SPI_PORT[eSpiPort]->FIFO_CTRL.TX_CLR = 1;
    
    while(SPI_PORT[eSpiPort]->FIFO_CTRL.TX_CLR != 0 && u32Timeout) u32Timeout--;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrRxFifo                                                                              */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    This function will clear all valid data of Rx FIFO                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClrRxFifo(E_DRVSPI_PORT eSpiPort)
{
    uint32_t u32Timeout;

	u32Timeout = 0x100000;
    SPI_PORT[eSpiPort]->FIFO_CTRL.RX_CLR = 1;
    
    while(SPI_PORT[eSpiPort]->FIFO_CTRL.RX_CLR != 0 && u32Timeout) u32Timeout--;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_EnableSlvTimeoutFifoClr                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable TX_CLR and RX_CLR are activated automatically when there is a slave mode time-out event       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_EnableSlvTimeoutFifoClr(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SLAVE_TIMEOUT_FIFO_CLR = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_DisableSlvTimeoutFifoClr                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable TX_CLR and RX_CLR are activated automatically when there is a slave mode time-out event      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_DisableSlvTimeoutFifoClr(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->SSR.SLAVE_TIMEOUT_FIFO_CLR = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SlvTimeoutPeriod                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*    u16Slvtoprd   [in]: Specify the period                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    This function indicate the time out period when there is serial clock input during slave select active  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_SlvTimeoutPeriod(E_DRVSPI_PORT eSpiPort, uint16_t u16Slvtoprd)
{
    SPI_PORT[eSpiPort]->SSR.SLAVE_TIMEOUT_PERIOD = u16Slvtoprd;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_RxFifoThr                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*    u8RxTh     [in]: Specify the Rx FIFO threshold.  It could be 0~7                                     */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVSPI_ERR_BIT_LENGTH  Wrong length                                                     */
/* Description:                                                                                            */
/*    Set RX fifo threshold                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
ERRCODE DrvSPI_RxFifoThr(E_DRVSPI_PORT eSpiPort, uint8_t u8RxTh)
{
	if(u8RxTh > 7)
	    return E_DRVSPI_ERR_BIT_LENGTH;
			
    SPI_PORT[eSpiPort]->FIFO_CTRL.RX_THRESHOLD = u8RxTh;	
    
    return E_SUCCESS; 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_TxFifoThr                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*    u8TxTh     [in]: Specify the Tx FIFO threshold. It could be 0~7                                      */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVSPI_ERR_BIT_LENGTH  Wrong length                                                     */
/* Description:                                                                                            */
/*    Set TX fifo threshold                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
ERRCODE DrvSPI_TxFifoThr(E_DRVSPI_PORT eSpiPort, uint8_t u8TxTh)
{
    if(u8TxTh > 7)
		return E_DRVSPI_ERR_BIT_LENGTH;
		    
    SPI_PORT[eSpiPort]->FIFO_CTRL.TX_THRESHOLD = u8TxTh;   
    
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_SetTxFifoUnderrunDo                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set TX FIFO underrun data out                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_SetTxFifoUnderrunDo(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->FIFO_CTRL.TXUDR_DO = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_ClrTxFifoUnderrunDo                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Clear TX FIFO underrun data out                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvSPI_ClearTxFifoUnderrunDo(E_DRVSPI_PORT eSpiPort)
{
    SPI_PORT[eSpiPort]->FIFO_CTRL.TXUDR_DO = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetBusyStatus                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Get SPI bus status                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
BOOL DrvSPI_GetBusyStatus(E_DRVSPI_PORT eSpiPort)
{
    return (SPI_PORT[eSpiPort]->STATUS.BUSY);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetSSLineStatus                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*    eSpiPort   [in]: Specify the SPI port                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Get slave select line bus status                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
BOOL DrvSPI_GetSSLineStatus(E_DRVSPI_PORT eSpiPort)
{
    return (SPI_PORT[eSpiPort]->STATUS.SS_LINE);
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvSPI_GetVersion                                                                                       */
/*                                                                                                                   */
/* Parameters:        	                                                                                             */
/*    None.                                                                                                          */
/*                                                                                                                   */
/* Returns:                                                                                                          */
/*    Version number.                                                                                                */
/*                                                                                                                   */
/* Description:                                                                                                      */
/*               This function is used to return the version number of SPI driver.    	                             */
/*-------------------------------------------------------------------------------------------------------------------*/
uint32_t DrvSPI_GetVersion(void)
{
    return DRVSPI_VERSION_NUM;
}
