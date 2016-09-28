/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2013 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "isd93xx.h"
#include "DrvGPIO.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define PORT_OFFSET   0x40

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static void (*_pfGPABCallback)(uint32_t u32GpaStatus, uint32_t u32GpbStatus);
static void (*_pfEINT0Callback)(void);
static void (*_pfEINT1Callback)(void);              		 

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO A and GPIO B  ISR                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     GPAB_IRQHandler                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               ISR to handle GPAB interrupt event           		                                       */
/*---------------------------------------------------------------------------------------------------------*/
//#ifdef USE_DRIVER_GPIO_IRQ
void GPAB_IRQHandler(void)
{
    uint32_t u32GpaStatus, u32GpbStatus;
	
    /* Keep the interrupt source */
    u32GpaStatus = GPIOA->ISRC;// (0x5000_4000 + 8 << 2)[31:0] = 0x5000_4020[31:0]
    u32GpbStatus = GPIOB->ISRC;// (0x5000_4040 + 8 << 2)[31:0] = 0x5000_4060[31:0]

    /* Clear the interrupt */
    GPIOA->ISRC = u32GpaStatus;
    GPIOB->ISRC = u32GpbStatus;

    /* Call the callback function of GPIOAB interrupt */
    if(_pfGPABCallback)
        _pfGPABCallback(u32GpaStatus, u32GpbStatus);    
}
//	  #endif

/*---------------------------------------------------------------------------------------------------------*/
/* External INT0 & INT1  ISR                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     EINT0_IRQHandler                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               ISR to handle EINT0 interrupt event           		                                       */
/*---------------------------------------------------------------------------------------------------------*/
void EINT0_IRQHandler(void)
{
    /* EINT0 = GPB0. Clear the interrupt */
  	GPIOB->ISRC  = 1UL << 0;
    if(_pfEINT0Callback)
        _pfEINT0Callback();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     EINT1_IRQHandler                                                                         */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               ISR to handle EINT1 interrupt event           		                                       */
/*---------------------------------------------------------------------------------------------------------*/
void EINT1_IRQHandler(void)
{
    /* EINT1 = GPB1. Clear the interrupt */
  	GPIOB->ISRC  = 1UL << 1;
    if(_pfEINT1Callback)
        _pfEINT1Callback();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetIntCallback                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             pfGPABCallback  - [in] The callback function of GPA and GPB interrupts.                   */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               To install GPIO interrrupt callback functions                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SetIntCallback(GPIO_GPAB_CALLBACK pfGPABCallback)
{
    _pfGPABCallback  = (void (*)(uint32_t, uint32_t))pfGPABCallback;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_Open                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*               port    	-[in] Specified GPIO port. It could be GPA, GPB                                  */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.     */
/*	             mode    	-[in] Set the IO to be IO_INPUT , IO_OUTPUT ,IO_OPENDRAIN or IO_QUASI            */
/* Returns:                                                                                                */
/*              E_SUCCESS										      Operation successful                                     */
/*              E_DRVGPIO_ARGUMENT								Incorrect argument                                       */
/* Description:                                                                                            */
/*               To configure the specified GPIO to use it.                                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_Open(DRVGPIO_PORT port, uint32_t u32Bit, DRVGPIO_IO mode)
{
    if (u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }  
    
    if(mode == IO_INPUT)
    {
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (u32Bit * 2)))));
    }
    else if(mode == IO_OUTPUT)
    {
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (u32Bit * 2)))));
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) | (1 << (u32Bit * 2)));
    }
    else if(mode == IO_OPENDRAIN)
    {
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (u32Bit * 2)))));
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) | (2 << (u32Bit * 2)));
    }
    else if(mode == IO_QUASI)
    {
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (u32Bit * 2)))));
        outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) | (3 << (u32Bit * 2)));
    }
    else
    {
        return E_DRVGPIO_ARGUMENT;
    }  
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Function:     DrvGPIO_Close		 		                                                                     */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               port - [in]  Specified GPIO port. It could be GPA, GPB					                           */
/*               u32Bit -[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To close the opened IO and reset its configurations                                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_Close(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if (u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }		
    outpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET, (inpw((uint32_t)&GPIOA->PMD + port * PORT_OFFSET) & (~(3 << (u32Bit*2)))));	
    GPIO_DBNCECON->ICLK_ON = 0; // 0x5000_4180[5]  
    return E_SUCCESS;
}	

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetPortBits                                                                       */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*	             port    	  -[in] Specified GPIO port. It could be GPA, GPB                                  */
/*	             u16Data  	-[in] The data to write to the specified IO port.                              */
/* Returns:                                                                                                */
/*               E_SUCCESS									    	Operation successful                                     */
/*               E_DRVGPIO_ARGUMENT								Incorrect argument                                       */
/* Description:                                                                                            */
/*               The function is used to set the output port value to the specified GPIO port.             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortBits(DRVGPIO_PORT port, uint16_t u16Data)
{
    if(port == GPA)
        GPIOA->DOUT =	u16Data; //(0x5000_0000 + 2<<2)[31:0]= 0x5000_0004[31:0]
    else if(port == GPB)
        GPIOB->DOUT =	u16Data; //(0x5000_0040 + 2<<2)[31:0]= 0x5000_0044[31:0]
    else
        return E_DRVGPIO_ARGUMENT;
        
    return E_SUCCESS;	  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetPortBits                                                                       */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB                                  */
/* Returns:                                                                                                */
/*               The IO pin value of the specified IO port                                                 */
/* Description:                                                                                            */
/*               The function is used to get the data of the specified IO port.                            */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortBits(DRVGPIO_PORT port)
{
    uint32_t u32PortAddr;
    
    u32PortAddr = (uint32_t)&GPIOA->PIN + port * PORT_OFFSET; 	
    return inpw(u32PortAddr);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetBit                                                                            */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB                                  */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.     */
/* Returns:                                                                                                */
/*               The specified input pin value					0 / 1                                              */
/*               E_DRVGPIO_ARGUMENT							       	Incorrect argument                                 */
/* Description:                                                                                            */
/*               The function is used to Get the value of the specified IO bit .                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetBit(DRVGPIO_PORT port, uint32_t u32Bit)
{
	uint32_t u32PortAddr;

  	if (u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }

    u32PortAddr = (uint32_t)&GPIOA->PIN + port * PORT_OFFSET;	   
    return ((inpw(u32PortAddr) >> u32Bit) & 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetBit                                                                            */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB                                  */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.      */
/* Returns:                                                                                                */
/*               E_SUCCESS									    	Operation successful                                     */
/*               E_DRVGPIO_ARGUMENT								Incorrect argument                                       */
/* Description:                                                                                            */
/*               The function is used to Set the specified IO bit to 1  .                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBit(DRVGPIO_PORT port, uint32_t u32Bit)
{ 
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    if(port == GPA)
        GPIOA->DOUT |=	(1 << u32Bit);
    else if(port == GPB)
        GPIOB->DOUT |=	(1 << u32Bit);
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_ClrBit                                                                            */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					               */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.    */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT							  	Incorrect argument                                     */
/* Description:                                                                                            */
/*               The function is used to  Clear the specified IO bit to 0                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrBit(DRVGPIO_PORT port, uint32_t u32Bit)
{
    uint32_t u32PortAddr;
    
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }

    u32PortAddr = (uint32_t)&GPIOA->DOUT  + port * PORT_OFFSET;
   	
    outpw(u32PortAddr, inpw(u32PortAddr) & (~(1 << u32Bit)));
   
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetDoutBit                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB                                */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.    */
/* Returns:                                                                                                */
/*               The bit value of the specified register			0 / 1                                        */
/*               E_DRVGPIO_ARGUMENT								            Incorrect argument                           */
/* Description:                                                                                            */
/*               The function is used to get the value of the specified IO bit from GPIO Dout register.    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetDoutBit(DRVGPIO_PORT port, uint32_t u32Bit)
{  
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
 
    return ((inpw((uint32_t)&GPIOA->DOUT + port * PORT_OFFSET) >> u32Bit) & 1);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetPortDoutBits                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB						           */
/* Returns:                                                                                                */
/*               The port value of the specified register		0 ~ 0xFFFF                                     */
/*               E_DRVGPIO_ARGUMENT						           		Incorrect argument                             */
/* Description:                                                                                            */
/*               The function is used to get the Dout register value of the specified IO port.    		   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortDoutBits(DRVGPIO_PORT port)
{
    if(port > GPB)
    {
        return E_DRVGPIO_ARGUMENT;
    }

    return inpw((uint32_t)&GPIOA->DOUT + port *PORT_OFFSET);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableDebounce                                                                    */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB						           */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~7 in GPB.    */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT							  	Incorrect argument                                     */
/* Description:                                                                                            */
/*               To enable the debounce function of the specified GPIO bit.            	     	           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableDebounce(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    // GPIOA->DBEN: (0x5000_4000 + 5 << 2)[31:0] = 0x5000_4014[31:0]
    outpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET, inpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET) | (1 << u32Bit));

    //GPIO_DBNCECON->ICLK_ON = 1;

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetDebounceTime                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             u32DebounceClk    	-[in] The debounce timing is 2^(u32DebounceClk) * clockSource          */
/*	             clockSource    	-[in] The debounce clock source can be DBCLKSRC_HCLK or DBCLKSRC_10K   */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               The function is used to To set the debounce timing and select source.     		             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetDebounceTime(uint32_t u32DebounceClk, DRVGPIO_DBCLKSRC clockSource)
{
    /* Maximum debounce time is 2^(15) * clockSource */
    if(u32DebounceClk > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    GPIO_DBNCECON->DBCLKSEL = u32DebounceClk ; // 0x5000_4180[3:0]

    GPIO_DBNCECON->DBCLKSRC = clockSource ; // 0x5000_4180[4]

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetDebounceTime                                                                   */
/*                                                                                                         */
/* Parameter:        											                              										   */	
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               The debounce time setting.                                                                */
/* Description:                                                                                            */
/*               The function is used to To Get the debounce timing setting.            		           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetDebounceTime(void)
{
    volatile int32_t i32Reg;

    i32Reg = 	GPIO_DBNCECON->DBCLKSEL;//0x5000_4180[3:0]
   
    return i32Reg;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableDebounce                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					               */
/*	             u32Bit  	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~7 in GPB.    */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To disable the debounce function of the specified GPIO bit.            	     	           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableDebounce(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    // GPIOA->DBEN: (0x5000_4000 + 5 << 2)[31:0] = 0x5000_4014[31:0]
    outpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->DBEN + port * PORT_OFFSET) &(~(1<<u32Bit)));

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetPortMask                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					                         */
/*	             u32Bit   	-[in] The mask data for the specified IO port.                             */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To set GPIO port mask register.                                        	     	       */
/*				 When set the port = '0xFFFF', the corresponding register of GPIOx_DOUT is protected and   */
/*                   the write data to the protect port is ignored.                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetPortMask(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    //&GPIOA->DMASK: (0x5000_4000 + 3<< 4)[31:0] = 0x5000_400C[31:0]
    outpw((uint32_t)&GPIOA->DMASK + port * PORT_OFFSET, u32Bit);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SetBitMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB				                           */
/*	             u32Bit    	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.   */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To set bits of GPIO port mask                                           	     	       */
/*				 When set the bit = '1', the corresponding registre of GPIOx_DOUT bit[n] is protected and  */
/*                   the write data to the protect bit is ignored.                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_SetBitMask(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    outpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET , inpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET) | (1<<u32Bit));

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_ClrBitMask                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					               */
/*	             u32Bit    	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.    */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To clear bits of GPIO port mask	                                       	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_ClrBitMask(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    outpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET,inpw((uint32_t)&GPIOA->DMASK+ port*PORT_OFFSET) &(~(1<<u32Bit)));
	
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetPortMask                                                                      */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					               */
/* Returns:                                                                                                */
/*               the value of Specified GPIO port mask register .                                          */
/* Description:                                                                                            */
/*               To get current GPIO port mask setting	                                  	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetPortMask(DRVGPIO_PORT port)
{
    return inpw((uint32_t)&GPIOA->DMASK + port * PORT_OFFSET); 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableInt                                                                         */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    		   -[in] Specified GPIO port. It could be GPA, GPB					       */
/*	             u32Bit    	   -[in] Specified bit of the IO port. It could be 0~15 in GPA and 2~15 in GPB.*/
/*	             tiggerType		 -[in] Specified bit of the IO port. Specified INT type.                 */
/*                                       It could be IO_RISING, IO_FALLING or IO_BOTH_EDGE                 */
/*	             mode			     -[in] Specified INT mode.                                               */
/*                                       It could be MODE_EDGE, or MODE_LEVEL                              */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To set one bit of GPIO port to Interrupt type	                                  	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableInt(DRVGPIO_PORT port, uint32_t u32Bit, DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode)
{
    if((port == GPB) && ((u32Bit == 0) || (u32Bit == 1)))
    {
        // GPB0 is EINT0, GPB1 is EINT1. Both of them can't be assigned to GPAB interrupt
        return E_DRVGPIO_ARGUMENT;    
    }

    if((u32Bit > 15) || (port > GPB))
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    /* Configure the interrupt to be rising/falling when edge trigger or high/low level when level trigger */
    if(tiggerType == IO_RISING)//'1': rising edge or high level
        outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) | (1<<(u32Bit+16)));	 
    else if(tiggerType == IO_FALLING)//'1': falling edge or low level
        outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) | (1<<u32Bit));	 
    else if(tiggerType == IO_BOTH_EDGE) //'1': (rising edge + falling edge) or [(high level + low level) = no action] 
        outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) | (1<<u32Bit)| (1<<(u32Bit+16)));	 
    else
        return E_DRVGPIO_ARGUMENT;

    /* Configure to be level trigger or edge trigger: '1': level trigger; '0': edge trigger */
    if(mode == MODE_EDGE)
        outpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET)&(~(1<<u32Bit)));
    else if(mode == MODE_LEVEL)
        outpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + port*PORT_OFFSET)|(1<<u32Bit));
    else
        return E_DRVGPIO_ARGUMENT;
    
    /* Enable the relative interrupt of M0 */
    if(port == GPA || port == GPB)
    {
        NVIC_SetPriority (GPAB_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
        NVIC_EnableIRQ(GPAB_IRQn);
    }

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableInt                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    		   -[in] Specified GPIO port. It could be GPA, GPB					       */
/*               u32Bit    	   -[in] Specified bit of the IO port. It could be 0~15 in GPA and 2~15 in GPB.*/
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               To disable the interrupt of specified GPIO bit.                                 	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableInt(DRVGPIO_PORT port, uint32_t u32Bit)
{
    uint32_t u32Reg;

    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    /* Disable both rising/falling or both high level/low level interrupts */
    u32Reg = (1 << u32Bit) | (1 << (u32Bit + 16));
    outpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET , inpw((uint32_t)&GPIOA->IEN + port * PORT_OFFSET) & (~u32Reg));	 

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableEINT0                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             tiggerType		-[in] Specified trigger type. It could be IO_RISING, IO_FALLING, IO_BOTH_EDGE*/
/*               mode    	    -[in] Specified trigger mode. It could be MODE_EDGE, MODE_LEVEL.           */
/*				       pfEINT0Callback-[in] The callback function of EINT0 interrupts.    					   */
/* Returns:                                                                                                */
/*               None           			                                                               */
/* Description:                                                                                            */
/*               To enable interrupt 0; for GPIOB[0]						                                 	      	   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_EnableEINT0(DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode, GPIO_EINT0_CALLBACK pfEINT0Callback)
{
    uint32_t u32Reg;

    /* Configure the interrupt to be rising/falling when edge trigger or high/low level when level trigger */
    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
    if(tiggerType ==IO_RISING)
    {
        u32Reg = u32Reg | (1 << (0 + 16)); 
    }
    else if(tiggerType == IO_FALLING)
    {
        u32Reg = u32Reg | (1 << 0);  
    }
    else if(tiggerType == IO_BOTH_EDGE)
    {  
        u32Reg = u32Reg | (1 << 0) | (1 << (0 + 16));    
    }
		outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 

    /* Configure to be level trigger or edge trigger: '1': level trigger; '0': edge trigger */
    if(mode ==MODE_EDGE)
        outpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET)&(~(1<<0)));
    else if(mode ==MODE_LEVEL)
        outpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET)|(1<<0));

    _pfEINT0Callback = pfEINT0Callback;
    NVIC_EnableIRQ(EINT0_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableEINT0                                                                      */
/*                                                                                                         */
/* Parameter: 																							   */
/*               None																					   */
/* Returns:                                                                                                */
/*               None           			                                                               */
/* Description:                                                                                            */
/*               To disable interrupt 0; for GPIOB[0]					                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DisableEINT0(void)
{
    uint32_t u32Reg;
    
    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
    u32Reg = u32Reg & (~((1UL << 0) | (1UL << (0 + 16))));
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 
    NVIC_DisableIRQ(EINT0_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableEINT1                                                                       */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             tiggerType		-[in] Specified trigger type. It could be IO_RISING, IO_FALLING, IO_BOTH_EDGE*/
/*               mode    	    -[in] Specified trigger mode. It could be MODE_EDGE, MODE_LEVEL.           */
/*				       pfEINT0Callback-[in] The callback function of EINT1 interrupts.    					   */
/* Returns:                                                                                                */
/*               None           			                                                               */
/* Description:                                                                                            */
/*               To enable interrupt 1; for GPIOB[1]						                                 	      	   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_EnableEINT1(DRVGPIO_INT_TYPE tiggerType, DRVGPIO_INT_MODE mode, GPIO_EINT1_CALLBACK pfEINT1Callback)
{
    uint32_t u32Reg;

    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
    if(tiggerType ==IO_RISING)
    {
        u32Reg = u32Reg | (1UL << (1 + 16));    
    }
    else if(tiggerType == IO_FALLING)
    {
        u32Reg = u32Reg | (1UL << 1);  
    }
    else if(tiggerType == IO_BOTH_EDGE)
    {  
        u32Reg = u32Reg | (1UL << 1) | (1UL << (1 + 16)); 
    }
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 

    /* Configure to be level trigger or edge trigger: '1': level trigger; '0': edge trigger */
    if(mode ==MODE_EDGE)
        outpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET)&(~(1<<1)));
    else if(mode ==MODE_LEVEL)
        outpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET,inpw((uint32_t)&GPIOA->IMD + GPB*PORT_OFFSET)|(1<<1));
        
    _pfEINT1Callback = pfEINT1Callback;
    NVIC_EnableIRQ(EINT1_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableEINT1                                                                      */
/*                                                                                                         */
/* Parameter: 																							   */
/*               None																					   */
/* Returns:                                                                                                */
/*               None           			                                                               */
/*               									                                                       */
/* Description:                                                                                            */
/*               To disable interrupt 1; for GPIOB[1]					                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DisableEINT1(void)
{
    uint32_t u32Reg;
    
    u32Reg = inpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET);
    u32Reg = u32Reg & (~((1UL << 1) | (1UL << (1 + 16))));
    outpw((uint32_t)&GPIOA->IEN + GPB * PORT_OFFSET , u32Reg);	 
    NVIC_DisableIRQ(EINT1_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetIntStatus                                                                      */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    		   -[in] Specified GPIO port. It could be GPA, GPB                         */
/* Returns:                                                                                                */
/*               A value of the GPIO interrupt status register                                             */
/* Description:                                                                                            */
/*               This function is used to return a value to the GPIO interrupt status register.    	       */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvGPIO_GetIntStatus(DRVGPIO_PORT port)
{
    uint32_t u32Reg=0;
	
    if(port == GPA)
        u32Reg = GPIOA->ISRC;
    else if(port == GPB)
        u32Reg = GPIOB->ISRC;

    return u32Reg;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_EnableSchmitt                                                                     */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					                         */
/*	             u32Bit    	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.    */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               Enable GPIO schmitt                                                        	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_EnableSchmitt(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    outpw((uint32_t)&SYS->GPA_INP+ port*4 , inpw((uint32_t)&SYS->GPA_INP+ port*4) | (1<<u32Bit));

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DisableSchmitt                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             port    	-[in] Specified GPIO port. It could be GPA, GPB					               */
/*	             u32Bit    	-[in] Specified bit of the IO port. It could be 0~15 in GPA and 0~15 in GPB.    */
/* Returns:                                                                                                */
/*               E_SUCCESS										      Operation successful                                   */
/*               E_DRVGPIO_ARGUMENT						  		Incorrect argument                                     */
/* Description:                                                                                            */
/*               Disable GPIO schmitt                                                        	     	       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_DisableSchmitt(DRVGPIO_PORT port, uint32_t u32Bit)
{
    if(u32Bit > 15)
    {
        return E_DRVGPIO_ARGUMENT;
    }
    
    outpw((uint32_t)&SYS->GPA_INP+ port*4 , inpw((uint32_t)&SYS->GPA_INP+ port*4) & (~u32Bit));

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_WakeSetBit                                                                        */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to Set the wake IO bit to 1  .                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_WakeSetBit(void)
{
    SYS->WAKECR.WAKE_DOUT = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_WakeClrBit                                                                        */
/*                                                                                                         */
/* Parameter:        			                                                                                 */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*	             None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to clear the wake IO bit to 0  .                                     */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_WakeClrBit(void)
{
    SYS->WAKECR.WAKE_DOUT = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_WakeOpen                                                                          */
/*                                                                                                         */
/* Parameter:                                                                                              */	
/*	             mode    	-[in] Set the IO to be IO_INPUT , IO_OUTPUT ,IO_OPENDRAIN or IO_QUASI            */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               To configure the wake pin to use it.                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_WakeOpen(DRVGPIO_WAKE_IO mode)
{
    if(mode ==IO_PULL_UP)
    {
        SYS->WAKECR.WAKE_TRI = 0;
    }
    else
    {
        SYS->WAKECR.WAKE_TRI = 1;	 
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetWakeDoutBit                                                                    */
/*                                                                                                         */
/* Parameter:        			                                                                                 */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*	             None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to get the value of the specified IO bit from GPIO Dout register.    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetWakeDoutBit(void)
{  
    return (SYS->WAKECR.WAKE_DIN);
}

/**
 *  DrvGPIO_GPIOFunc()              Specify All IO pins to GPIO 
 *                                         
 *  DrvGPIO_PWM0_PA12()             Set PA12 as PWM0                                                      
 *  DrvGPIO_PWM1_PA13()             Set PA13 as PWM1         
 *  DrvGPIO_PWM01_PA12_PA13()       Set PA12 as PWM0, PA13 as PWM1                                     
 *  DrvGPIO_PWM2_PB14()             Set PB14 as PWM2                                                     
 *  DrvGPIO_PWM2_PA8()              Set PA8 as PWM2
 *  DrvGPIO_PWM3_PB15()             Set PB15 as PWM3
 *  DrvGPIO_PWM3_PA9()              Set PA9 as PWM3
 *  DrvGPIO_PWM23_PB14_PB15()       Set PB14 as PWM2, Set PB15 as PWM3
 *  DrvGPIO_PWM4_PB12()             Set PB12 as PWM4
 *  DrvGPIO_PWM4_PA14()             Set PA14 as PWM4
 *  DrvGPIO_PWM5_PB13()             Set PB13 as PWM5
 *  DrvGPIO_PWM5_PA15()             Set PA15 as PWM5
 *  DrvGPIO_PWM45_PA14_PA15()       Set PA14 as PWM4, Set PA15 as PWM5
 *  DrvGPIO_PWM0B_PB4()             Set PB4 as PWM0B
 *  DrvGPIO_PWM1B_PB5()             Set PB5 as PWM1B 
 *  DrvGPIO_PWM01B_PB4_PB5()        Set PB4 as PWM0B, Set PB5 as PWM1B    
 *  DrvGPIO_PWM2B_PB15()            Set PB15 as PWM2B
 *  DrvGPIO_PWM3B_PB14()            Set PB14 as PWM3B
 *  DrvGPIO_PWM23B_PB14_PB15()      Set PB14 as PWM3B, Set PB15 as PWM2B
 *  DrvGPIO_PWM4B_PB13()            Set PB13 as PWM4B
 *  DrvGPIO_PWM5B_PB12()            Set PB12 as PWM5B
 *  DrvGPIO_PWM45B_PB12_PB13()      Set PB12 as PWM5B, Set PB13 as PWM4B      
 *
 *  DrvGPIO_I2C_PA10_PA11()         Set PA10 as SDA, Set PA11 as SCL
 *  DrvGPIO_I2C_PA1_PA3()           Set PA3 as SDA, Set PA1 as SCL 
 *  DrvGPIO_I2C_PB2_PB3()           Set PB3 as SDA, Set PB2 as SCL 
 *  DrvGPIO_I2C_SCL_PA11()          Set PA11 as SCL
 *  DrvGPIO_I2C_SCL_PB2()           Set PB2 as SCL
 *  DrvGPIO_I2C_SCL_PA1()           Set PA1 as SCL
 *  DrvGPIO_I2C_SDA_PA10()          Set PA10 as SDA
 *  DrvGPIO_I2C_SDA_PB3()           Set PB3 as SDA
 *  DrvGPIO_I2C_SDA_PA3()           Set PA3 as SDA
 *
 *  DrvGPIO_I2S_PA4_PA5_PA6_PA7()   Set PA4 as FS, Set PA5 as BCLK, Set PA6 as SDI, Set PA7 as SDO
 *  DrvGPIO_I2S_PB8_PB9_PB10_PB11() Set PB8 as FS, Set PB9 as BCLK, Set PB10 as SDI, Set PB11 as SDO 
 *  DrvGPIO_I2S_FS_PA4()            Set PA4 as FS
 *  DrvGPIO_I2S_FS_PA12()           Set PA12 as FS
 *  DrvGPIO_I2S_FS_PB8()            Set PB8 as FS
 *  DrvGPIO_I2S_BCLK_PA5()          Set PA5 as BCLK 
 *  DrvGPIO_I2S_BCLK_PA13()         Set PA13 as BCLK
 *  DrvGPIO_I2S_BCLK_PB9()          Set PB9 as BCLK
 *  DrvGPIO_I2S_SDI_PA6()           Set PA6 as SDI
 *  DrvGPIO_I2S_SDI_PB6()           Set PB6 as SDI
 *  DrvGPIO_I2S_SDO_PA7()           Set PA7 as SDO
 *  DrvGPIO_I2S_SDO_PB7()           Set PB7 as SDO
 *  DrvGPIO_I2S_SDO_PB11()          Set PB11 as SDO
 *  DrvGPIO_I2S_MCLK_PA0()          Set PA0 as MCLK
 *  DrvGPIO_I2S_MCLK_PB1()          Set PB1 as MCLK
 *
 *  DrvGPIO_SPI_PA0_PA1_PA2_PA3()   Set PA0 as MOSI0, Set PA1 as SCLK, Set PA2 as SSB0, Set PA3 as MISO0 
 *  DrvGPIO_SPI_MOSI0_PA0()         Set PA0 as MOSI0		
 *  DrvGPIO_SPI_MOSI0_PB4()         Set PB4 as MOSI0		   
 *  DrvGPIO_SPI_SCLK_PA1()          Set PA1 as SCLK			 
 *  DrvGPIO_SPI_SCLK_PB2()          Set PB2 as SCLK				 
 *  DrvGPIO_SPI_SSB0_PA2()          Set PA2 as SSB0				 
 *  DrvGPIO_SPI_SSB0_PB0()          Set PB0 as SSB0			
 *  DrvGPIO_SPI_MISO0_PA3()         Set PA3 as MISO0			
 *  DrvGPIO_SPI_MISO0_PB3()         Set PB3 as MISO0
 *  DrvGPIO_SPI_MOSI1_PB6()         Set PB6 as MOSI1		
 *  DrvGPIO_SPI_MOSI1_PB13()        Set PB13 as MOSI1			 
 *  DrvGPIO_SPI_SSB1_PA5()          Set PA5 as SSB1				 
 *  DrvGPIO_SPI_SSB1_PB0()          Set PB0 as SSB1
 *  DrvGPIO_SPI_SSB1_PB1()          Set PB1 as SSB1 
 *  DrvGPIO_SPI_MISO1_PB5()         Set PB5 as MISO1  
 *  DrvGPIO_SPI_MISO1_PB12()        Set PB12 as MISO1			 
 *
 *  DrvGPIO_UART_TX_PA6()           Set PA6 as TX
 *  DrvGPIO_UART_TXRX_PA8_PA9()     Set PA8 as TX, Set PA9 as RX
 *  DrvGPIO_UART_TX_PB14()          Set PB14 as TX
 *  DrvGPIO_UART_RTSCTS_PA10_PA11() Set PA10 as RTSn, Set PA11 as CTSn
 *
 *  DrvGPIO_CMP_CMP0_PB0()          Set PB0 as CMP0
 *  DrvGPIO_CMP_CMP1_PB1()          Set PB1 as CMP1
 *  DrvGPIO_CMP_CMP2_PB2()          Set PB2 as CMP2
 *  DrvGPIO_CMP_CMP3_PB3()          Set PB3 as CMP3 
 *  DrvGPIO_CMP_CMP4_PB4()          Set PB4 as CMP4 
 *  DrvGPIO_CMP_CMP5_PB5()          Set PB5 as CMP5 
 *  DrvGPIO_CMP_CMP6_PB6()          Set PB6 as CMP6
 *  DrvGPIO_CMP_CMP7_PB7()          Set PB7 as CMP7
 *  DrvGPIO_CMP_CMP8_PB8()          Set PB8 as CMP8
 *  DrvGPIO_CMP_CMP9_PB9()          Set PB9 as CMP9
 *  DrvGPIO_CMP_CMP10_PB10()        Set PB10 as CMP10
 *  DrvGPIO_CMP_CMP11_PB11()        Set PB11 as CMP11
 *  DrvGPIO_CMP_CMP12_PA8()         Set PA8 as CMP12 
 *  DrvGPIO_CMP_CMP13_PA9()         Set PA9 as CMP13 
 *  DrvGPIO_CMP_CMP14_PA10()        Set PA10 as CMP14
 *  DrvGPIO_CMP_CMP15_PA11()        Set PA11 as CMP15 
 *
 *  DrvGPIO_TIMER_TM0_PA14()        Set Event counter input or timer toggle output pin of Timer0 
 *  DrvGPIO_TIMER_TM1_PA15()        Set Event counter input or timer toggle output pin of Timer1 
 *
 *  DrvGPIO_SPK_PA12_PA13()         Set PA12 as SPKP, Set PA13 as SPKN     
 *  DrvGPIO_SPK_SPKP_PA12()         Set PA12 as SPKP 
 *  DrvGPIO_SPK_SPKN_PA13()         Set PA13 as SPKN
 *
 *  DrvGPIO_DMIC_PA14_PA15()        Set PA14 as SDCLK, Set PA15 as SDIN 
 *  DrvGPIO_DMIC_SDCLK_PA14()       Set PA14 as SDCLK  
 *  DrvGPIO_DMIC_SDIN_PA15()        Set PA15 as SDIN 
 */  

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GPIOFunc                                                                          */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Specify All IO pins                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_GPIOFunc(void)
{
    outpw(&SYS->GPA_ALT,0); 
    outpw(&SYS->GPB_ALT,0);	
}

/*---------------------------------------------------------------------------------------------------------*/
/* PWM                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM0_PA12                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA12 as PWM0                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM0_PA12(void)
{
    SYS->GPA_ALT.GPA12 = 1;	 // PWM0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM1_PA13                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA13 as PWM1                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM1_PA13(void)
{
    SYS->GPA_ALT.GPA13 = 1;	 // PWM1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM01_PA12_PA13                                                                   */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA12 as PWM0, set PA13 as PWM1                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM01_PA12_PA13(void)
{
    SYS->GPA_ALT.GPA12 = 1;	 // PWM0
	SYS->GPA_ALT.GPA13 = 1;	 // PWM1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM2_PB14                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB14 as PWM2                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM2_PB14(void)
{
    SYS->GPB_ALT.GPB14 = 1;	 // PWM2
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM2_PA8                                                                          */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA8 as PWM2                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM2_PA8(void)
{
    SYS->GPA_ALT.GPA8 = 3;	 // PWM2
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM3_PB15                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB15 as PWM3                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM3_PB15(void)
{
    SYS->GPB_ALT.GPB15 = 1;	 // PWM3
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM3_PA9                                                                          */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA9 as PWM3                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM3_PA9(void)
{
    SYS->GPA_ALT.GPA9 = 3;	 // PWM3
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM2_PB14_PB15                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB14 as PWM2, Set PB15 as PWM3                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM23_PB14_PB15(void)
{
    SYS->GPB_ALT.GPB14 = 1;	 // PWM2
	SYS->GPB_ALT.GPB15 = 1;	 // PWM3
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM4_PB12                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB12 as PWM4                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM4_PB12(void)
{
    SYS->GPB_ALT.GPB12 = 2;	 // PWM4
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM4_PA14                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA14 as PWM4                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM4_PA14(void)
{
    SYS->GPA_ALT.GPA14 = 3;	 // PWM4
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM5_PB13                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB13 as PWM5                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM5_PB13(void)
{
    SYS->GPB_ALT.GPB13 = 2;	 // PWM5
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM5_PA15                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA15 as PWM5                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM5_PA15(void)
{
    SYS->GPA_ALT.GPA15 = 3;	 // PWM5
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM45_PA14_PA15                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA15 as PWM5, Set PA14 as PWM4                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM45_PA14_PA15(void)
{
    SYS->GPA_ALT.GPA14 = 3;	 // PWM4
    SYS->GPA_ALT.GPA15 = 3;	 // PWM5
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM0B_PB4                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB4 as PWM0B                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM0B_PB4(void)
{
    SYS->GPB_ALT.GPB4 = 1;	 // PWM0B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM1B_PB5                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB5 as PWM1B                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM1B_PB5(void)
{
    SYS->GPB_ALT.GPB5 = 1;	 // PWM1B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM01B_PB4_PB5                                                                         */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB4 as PWM0B, Set PB5 as PWM1B                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM01B_PB4_PB5(void)
{
    SYS->GPB_ALT.GPB4 = 1;	 // PWM0B
	SYS->GPB_ALT.GPB5 = 1;	 // PWM1B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM2B_PB15                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB15 as PWM2B                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM2B_PB15(void)
{
    SYS->GPB_ALT.GPB15 = 3;	 // PWM2B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM3B_PB14                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB14 as PWM3B                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM3B_PB14(void)
{
    SYS->GPB_ALT.GPB14 = 3;	 // PWM3B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM23B_PB14_PB15                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB15 as PWM2B, Set PB14 as PWM3B                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM23B_PB14_PB15(void)
{
    SYS->GPB_ALT.GPB15 = 3;	 // PWM2B
	SYS->GPB_ALT.GPB14 = 3;	 // PWM3B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM4B_PB13                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB13 as PWM4B                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM4B_PB13(void)
{
    SYS->GPB_ALT.GPB13 = 3;	 // PWM4B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM5B_PB12                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB12 as PWM5B                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM5B_PB12(void)
{
    SYS->GPB_ALT.GPB12 = 3;	 // PWM5B
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_PWM45B_PB12_PB13                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB13 as PWM4B, Set PB13 as PWM4B                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_PWM45B_PB12_PB13(void)
{
    SYS->GPB_ALT.GPB13 = 3;	 // PWM4B
	SYS->GPB_ALT.GPB12 = 3;	 // PWM5B
}

/*---------------------------------------------------------------------------------------------------------*/
/* I2C                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_PA10_PA11                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA10 as SDA, Set PA11 as SCL                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_PA10_PA11(void)
{
    SYS->GPA_ALT.GPA10 = 1;  //SDA
    SYS->GPA_ALT.GPA11 = 1;  //SCL
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_PA1_PA3                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA3 as SDA, Set PA1 as SCL                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_PA1_PA3(void)
{
    SYS->GPA_ALT.GPA1 = 2;	// SCL
    SYS->GPA_ALT.GPA3 = 2;	// SDA
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_PA4_PA5                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA4 as SDA, Set PA5 as SCL                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_PA4_PA5(void)
{
    SYS->GPA_ALT.GPA4 = 2;	// SDA
    SYS->GPA_ALT.GPA5 = 2;	// SCL
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_PB2_PB3                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB3 as SDA, Set PB2 as SCL                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_PB2_PB3(void)
{
    SYS->GPB_ALT.GPB3 = 2;	// SDA
    SYS->GPB_ALT.GPB2 = 2;	// SCL
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_SCL_PA11                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA11 as SCL                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_SCL_PA11(void)
{
    SYS->GPA_ALT.GPA11 = 1;	// SCL
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_SCL_PB2                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB2 as SCL                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_SCL_PB2(void)
{
    SYS->GPB_ALT.GPB2 = 1;	// SCL
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_SCL_PA1                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA1 as SCL                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_SCL_PA1(void)
{
    SYS->GPA_ALT.GPA1 = 2;	// SCL
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_SDA_PA10                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA10 as SDA                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_SDA_PA10(void)
{
    SYS->GPA_ALT.GPA10 = 1;	// SDA
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_SDA_PB3                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB3 as SDA                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_SDA_PB3(void)
{
    SYS->GPB_ALT.GPB3 = 1;	// SDA
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2C_SDA_PA3                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA3 as SDA                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2C_SDA_PA3(void)
{
    SYS->GPA_ALT.GPA3 = 2;	// SDA
}

/*---------------------------------------------------------------------------------------------------------*/
/* I2S                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_PA4_PA5_PA6_PA7                                                                               */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA4 as FS, Set PA5 as BCLK, Set PA6 as SDI, Set PA7 as SDO                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_PA4_PA5_PA6_PA7(void)
{
    SYS->GPA_ALT.GPA4 = 1;	// FS
    SYS->GPA_ALT.GPA5 = 1;	// BCLK
    SYS->GPA_ALT.GPA6 = 1;	// SDI
    SYS->GPA_ALT.GPA7 = 1;	// SDO
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_PB8_PB9_PB10_PB11                                                             */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB8 as FS, Set PB9 as BCLK, Set PB10 as SDI, Set PB11 as SDO                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_PB8_PB9_PB10_PB11(void)
{
    SYS->GPB_ALT.GPB8 = 1;	// FS
    SYS->GPB_ALT.GPB9 = 1;	// BCLK
    SYS->GPB_ALT.GPB10 = 1;	// SDI
    SYS->GPB_ALT.GPB11 = 1;	// SDO
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_FS_PA4                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA4 as FS                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_FS_PA4(void)
{
    SYS->GPA_ALT.GPA4 = 1;	// FS
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_FS_PA12                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA12 as FS                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_FS_PA12(void)
{
    SYS->GPA_ALT.GPA12 = 3;	// FS
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_FS_PB8                                                                        */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB8 as FS                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_FS_PB8(void)
{
    SYS->GPB_ALT.GPB8 = 1;	// FS
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_BCLK_PA5                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA5 as BCLK                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_BCLK_PA5(void)
{
    SYS->GPA_ALT.GPA5 = 1;	// BCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_BCLK_PA13                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA13 as BCLK                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_BCLK_PA13(void)
{
    SYS->GPA_ALT.GPA13 = 3;	// BCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_BCLK_PB9                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB9 as BCLK                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_BCLK_PB9(void)
{
    SYS->GPB_ALT.GPB9 = 1;	// BCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_SDI_PA6                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA6 as SDI                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_SDI_PA6(void)
{
    SYS->GPA_ALT.GPA6 = 1;	// SDI
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_SDI_PB6                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB6 as SDI                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_SDI_PB6(void)
{
    SYS->GPB_ALT.GPB6 = 1;	// SDI
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_SDO_PA7                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA7 as SDO                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_SDI_PA7(void)
{
    SYS->GPA_ALT.GPA7 = 1;	// SDO
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_SDO_PB7                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB7 as SDO                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_SDI_PB7(void)
{
    SYS->GPB_ALT.GPB7 = 1;	// SDO
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_SDO_PB11                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB11 as SDO                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_SDI_PB11(void)
{
    SYS->GPB_ALT.GPB11 = 1;	// SDO
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_MCLK_PA0                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA0 as MCLK                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_MCLK_PA0(void)
{
    SYS->GPA_ALT.GPA0 = 2;	// MCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_I2S_MCLK_PB1                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB1 as MCLK                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_I2S_MCLK_PB1(void)
{
    SYS->GPB_ALT.GPB1 = 1;	// MCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* SPI                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_PA0_PA1_PA2_PA3                                                               */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA0 as MOSI0, Set PA1 as SCLK, Set PA2 as SSB0, Set PA3 as MISO0                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_PA0_PA1_PA2_PA3(void)
{
    SYS->GPA_ALT.GPA0 = 1;	// MOSI0
    SYS->GPA_ALT.GPA1 = 1;	// SCLK
    SYS->GPA_ALT.GPA2 = 1;	// SSB0
    SYS->GPA_ALT.GPA3 = 1;	// MISO0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MOSI0_PA0                                                                     */
/*    																														*/
/* Parameter:                         																					  */
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA0 as MOSI0                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MOSI0_PA0(void)
{
    SYS->GPA_ALT.GPA0 = 1;	// MOSI0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MOSI0_PB4                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB4 as MOSI0                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MOSI0_PB4(void)
{
    SYS->GPB_ALT.GPB4 = 3;	// MOSI0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SCLK_PA2                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA1 as SCLK                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SCLK_PA1(void)
{
    SYS->GPA_ALT.GPA1 = 1;	// SCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SCLK_PB2                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB2 as SCLK                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SCLK_PB2(void)
{
    SYS->GPB_ALT.GPB2 = 3;	// SCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SSB0_PA2                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA2 as SSB0                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SSB0_PA2(void)
{
    SYS->GPA_ALT.GPA2 = 1;	// SSB0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SSB0_PB0                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB0 as SSB0                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SSB0_PB0(void)
{
    SYS->GPB_ALT.GPB0 = 3;	// SSB0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MISO0_PA3                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA3 as MISO0                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MISO0_PA3(void)
{
    SYS->GPA_ALT.GPA3 = 1;	// MISO0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MISO0_PB3                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB3 as MISO0                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MISO0_PB3(void)
{
    SYS->GPB_ALT.GPB3 = 3;	// MISO0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MOSI1_PB6                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB6 as MOSI1                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MOSI1_PB6(void)
{
    SYS->GPB_ALT.GPB6 = 3;	// MOSI1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MOSI1_PB13                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB13 as MOSI1                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MOSI1_PB13(void)
{
    SYS->GPB_ALT.GPB13 = 1;	// MOSI1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SSB1_PA5                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA5 as SSB1                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SSB1_PA5(void)
{
    SYS->GPA_ALT.GPA5 = 3;	// SSB1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SSB1_PB0                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB0 as SSB1                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SSB1_PB0(void)
{
    SYS->GPB_ALT.GPB0 = 1;	// SSB1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_SSB1_PB1                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB1 as SSB1                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_SSB1_PB1(void)
{
    SYS->GPB_ALT.GPB1 = 3;	// SSB1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MISO1_PB5                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB5 as MISO1                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MISO1_PB5(void)
{
    SYS->GPB_ALT.GPB5 = 3;	// MISO1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPI_MISO1_PB12                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB12 as MISO1                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPI_MISO1_PB12(void)
{
    SYS->GPB_ALT.GPB12 = 1;	// MISO1
}

/*---------------------------------------------------------------------------------------------------------*/
/* UART                                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_UART_TX_PA6                                                                       */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA6 as TX                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_UART_TX_PA6(void)
{
    SYS->GPA_ALT.GPA6 = 2;	// TX
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_UART_TXRX_PA8_PA9                                                                 */
/*                                                                                                         */
/* Parameter:        														     					   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA8 as TX, Set PA9 as RX                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_UART_TXRX_PA8_PA9(void)
{
    SYS->GPA_ALT.GPA8 = 1;	// TX
    SYS->GPA_ALT.GPA9 = 1;	// RX
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_UART_TX_PB14                                                                   */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB14 as TX                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_UART_TX_PB14(void)
{
    SYS->GPB_ALT.GPB14 = 2;	// TX
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_UART_RTSCTS_PA10_PA11                                                             */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA10 as RTSn, Set PA11 as CTSn                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_UART_RTSCTS_PA10_PA11(void)
{
    SYS->GPA_ALT.GPA10 = 3;	// RTS
    SYS->GPA_ALT.GPA11 = 3;	// CTS
}

/*---------------------------------------------------------------------------------------------------------*/
/* CMP                                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP0_PB0                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB0 as CMP0                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP0_PB0(void)
{
    SYS->GPB_ALT.GPB0 = 2;	// CMP0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP1_PB1                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB1 as CMP1                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP1_PB1(void)
{
    SYS->GPB_ALT.GPB1 = 2;	// CMP1
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP2_PB2                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB2 as CMP2                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP2_PB2(void)
{
    SYS->GPB_ALT.GPB2 = 2;	// CMP2
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP3_PB3                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB3 as CMP3                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP3_PB3(void)
{
    SYS->GPB_ALT.GPB3 = 2;	// CMP3
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP4_PB4                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB4 as CMP4                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP4_PB4(void)
{
    SYS->GPB_ALT.GPB4 = 2;	// CMP4
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP5_PB5                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB5 as CMP5                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP5_PB5(void)
{
    SYS->GPB_ALT.GPB5 = 2;	// CMP5
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP6_PB6                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB6 as CMP6                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP6_PB6(void)
{
    SYS->GPB_ALT.GPB6 = 2;	// CMP6
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP7_PB7                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB7 as CMP7                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP7_PB7(void)
{
    SYS->GPB_ALT.GPB7 = 2;	// CMP7
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP8_PB8                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB8 as CMP8                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP8_PB8(void)
{
    SYS->GPB_ALT.GPB8 = 2;	// CMP8
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP9_PB9                                                                      */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB9 as CMP9                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP9_PB9(void)
{
    SYS->GPB_ALT.GPB9 = 2;	// CMP9
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP10_PB10                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB10 as CMP10                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP10_PB10(void)
{
    SYS->GPB_ALT.GPB10 = 2;	// CMP10
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP11_PB11                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PB11 as CMP11                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP11_PB11(void)
{
    SYS->GPB_ALT.GPB11 = 2;	// CMP11
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP12_PA8                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA8 as CMP12                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP12_PA8(void)
{
    SYS->GPA_ALT.GPA8 = 2;	// CMP12
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP13_PA9                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA9 as CMP13                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP13_PA9(void)
{
    SYS->GPA_ALT.GPA9 = 2;	// CMP13
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP14_PA10                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA10 as CMP14                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP14_PA10(void)
{
    SYS->GPA_ALT.GPA10 = 2;	// CMP14
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_CMP_CMP15_PA11                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA11 as CMP15                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_CMP_CMP15_PA11(void)
{
    SYS->GPA_ALT.GPA11 = 2;	// CMP15
}

/*---------------------------------------------------------------------------------------------------------*/
/* TIMER                                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_TIMER_TM0_PA14                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set Event counter input or timer toggle output pin of Timer0                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_TIMER_TM0_PA14(void)
{
    SYS->GPA_ALT.GPA14 = 1;	// TM0
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_TIMER_TM1_PA15                                                                    */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set Event counter input or timer toggle output pin of Timer1                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_TIMER_TM1_PA15(void)
{
    SYS->GPA_ALT.GPA15 = 1;	// TM1
}

/*---------------------------------------------------------------------------------------------------------*/
/* SPEAKER                                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPK_PA12_PA13                                                                               */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA12 as SPKP, Set PA13 as SPKN                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPK_PA12_PA13(void)
{
    SYS->GPA_ALT.GPA12 = 2;	// SPKP
    SYS->GPA_ALT.GPA13 = 2;	// SPKN
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPK_SPKP_PA12                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA12 as SPKP                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPK_SPKP_PA12(void)
{
    SYS->GPA_ALT.GPA12 = 2;	// SPKP
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_SPK_SPKN_PA13                                                                     */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA13 as SPKN                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_SPK_SPKN_PA13(void)
{
    SYS->GPA_ALT.GPA13 = 2;	// SPKN
}

/*---------------------------------------------------------------------------------------------------------*/
/* DMIC                                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DMIC_PA14_PA15                                                                              */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA14 as SDCLK, Set PA15 as SDIN                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DMIC_PA14_PA15(void)
{
    SYS->GPA_ALT.GPA14 = 2;	// SDCLK
    SYS->GPA_ALT.GPA15 = 2;	// SDIN
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DMIC_SDCLK_PA14                                                                   */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA14 as SDCLK                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DMIC_SDCLK_PA14(void)
{
    SYS->GPA_ALT.GPA14 = 2;	// SDCLK
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_DMIC_SDIN_PA15                                                                   */
/*                                                                                                         */
/* Parameter:        														                                          							   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Set PA15 as SDIN                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvGPIO_DMIC_SDIN_PA15(void)
{
    SYS->GPA_ALT.GPA15 = 2;	// SDIN
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvGPIO_GetVersion                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             None                                                                                      */
/* Returns:                                                                                                */
/*               The DrvGPIO version number                                                                */
/* Description:                                                                                            */
/*               This function is used to return the version number of GPIO driver.    	                   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvGPIO_GetVersion (void)
{
   return DRVGPIO_VERSION_NUM;
}


