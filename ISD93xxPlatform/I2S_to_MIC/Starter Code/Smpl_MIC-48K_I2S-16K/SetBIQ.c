#include "ISD93xx.h"


//============================================================
//BIQ parameters b0,b1,b2,a1,a2,b0,b1,b2,a1,a2,b0,b1,b2,a1,a2
//===========================
//SR=16k
//-----------------------
//BIQ 100~1200 Hz
//	uint32_t u32BiqCoeff[15]={0x03c10, 0x787e1, 0x03c10, 0x604a3, 0x0fbc3, 
//							  0x00aec, 0x010af, 0x00aec, 0x65e64, 0x0d3a9, 
//							  0x02b91, 0x00000, 0x7d46f, 0x6368a, 0x0cdcb};
//
//---------------------
//BIQ 100~2500 Hz
//uint32_t u32BiqCoeff[15]={0x03f90, 0x780e0, 0x03f90, 0x60502, 0x0fb63,
//						  0x02351, 0x0433e, 0x02351, 0x70f33, 0x0ae08,
//						  0x05835, 0x00000, 0x7a7cb, 0x66fb4, 0x098ca};
//---------------------
//BIQ 200~3000 Hz
//uint32_t u32BiqCoeff[15]={0x03d88, 0x784f0, 0x03d88, 0x60a59, 0x0f73a,
//							  0x031cc, 0x0605e, 0x031cc, 0x75a2f, 0x0a99c,
//							  0x066fc, 0x00000, 0x79904, 0x68b5a, 0x088be};
//
//---------------------
//BIQ 100~4000 Hz
//
//	uint32_t u32BiqCoeff[15]={0x040e2, 0x77e3b, 0x040e2, 0x60527, 0x0fb3e,
//							  0x04acb, 0x09393, 0x04acb, 0x7fe08, 0x09da7,
//							  0x088cc, 0x00000, 0x77734, 0x6b293, 0x05ac9 };
//---------------------------------
//SR=32k
//BIQ 20~8000 Hz
//	uint32_t u32BiqCoeff[15]={0x04289, 0x77aee, 0x04289 , 0x60080, 0x0ff81,
//								0x0465f, 0x08aea, 0x0465f, 0x7fed3, 0x09af4,
//								0x082c5, 0x00000, 0x77d3b, 0x6aa8a, 0x056ce };
//
//---------------------------------
//BIQ setting 2 for SR48k down sampling, biq_lpf_fs=48000Hz_f1=7600Hz_f2=8000Hz_Apass_ripple=0.25dB
const  int32_t i32BiqCoeff[15]={8742, 6267, 8742, -64957, 23172, 
								58594, -57650, 58594, -68860, 62862, 
								38607, -29533, 38607, -67378, 50913};
const uint32_t u32BiqCoeff[15]={0x02226, 0x0187b, 0x02226, 0x70243, 0x05a84,
								0x0e4e2, 0x71ece, 0x0e4e2, 0x6f304, 0x0f58e,
								0x096cf, 0x78ca3, 0x096cf, 0x6f8ce, 0x0c6e1};
//===========================


void Tdelay(void)
{
uint32_t	u32temp;

	for(u32temp=0; u32temp<32; u32temp++);
}


//Default sampling rate is 16K (need matched with ADC sampling rate) 
void u32BiqSetting(uint8_t u8Path)			//u8Path=0 for ADC, u8Path=1 for DPWM
{
uint32_t u32BiqCount, *pu32Biq;

   	SYSCLK->APBCLK.BIQALC_EN = 1;
	Tdelay();
	BIQ->BIQ_CTRL.RSTn = 1;
	Tdelay();
	BIQ->BIQ_CTRL.PRGCOEF=1;
	Tdelay();
	pu32Biq=(uint32_t *)BIQ_BA;
	for(u32BiqCount=0; u32BiqCount<15; u32BiqCount++)
		*pu32Biq++ = u32BiqCoeff[u32BiqCount];
	Tdelay();

	if(u8Path==1)
	{
		BIQ->BIQ_CTRL.SR_DIV=1535;	//32K base for UPSR
		BIQ->BIQ_CTRL.UPSR=3; 		//Sampling rate = 32K/(UPSR+1) when SR_DIV=1535
		//BIQ->BIQ_CTRL.UPSR=0;
		BIQ->BIQ_CTRL.SELPWM=1;
	}
	else
	{
		BIQ->BIQ_CTRL.SR_DIV=1023; 	//BIQ SR = 48K when SR_DIV=1023
		BIQ->BIQ_CTRL.UPSR=2; 		//(BIQ SR)*(UPSR+1) when SR_DIV=3071
		BIQ->BIQ_CTRL.SELPWM=0;
	}

	BIQ->BIQ_CTRL.PRGCOEF=0;
	Tdelay();
	BIQ->BIQ_CTRL.EN=1;
}

void i32BiqSetting(uint8_t u8Path)			//u8Path=0 for ADC, u8Path=1 for DPWM
{
uint32_t 	u32BiqCount;
int32_t	*pi32Biq;

   	UNLOCKREG();
	SYSCLK->APBCLK.BIQALC_EN = 1;
	Tdelay();
	BIQ->BIQ_CTRL.RSTn = 1;
	Tdelay();
	//outpw(0x400B0040,inpw(0x400B0040)|BIT31);
	Tdelay();
	BIQ->BIQ_CTRL.PRGCOEF=1; 				//Only can read/write BIQ parameters when PRGCOEF=1
	Tdelay();
	pi32Biq=(int32_t *)BIQ_BA;
	for(u32BiqCount=0; u32BiqCount<15; u32BiqCount++)
	{
		*pi32Biq++ = i32BiqCoeff[u32BiqCount];
		Tdelay();
	}
	
	if(u8Path==1)
	{
		BIQ->BIQ_CTRL.SR_DIV=1535;	//32K base for UPSR
		BIQ->BIQ_CTRL.UPSR=3; 		//Sampling rate = 32K/(UPSR+1) when SR_DIV=1535
		//BIQ->BIQ_CTRL.UPSR=0;
		BIQ->BIQ_CTRL.SELPWM=1;
	}
	else
	{
		BIQ->BIQ_CTRL.SR_DIV=1023; 	//BIQ SR = 48K when SR_DIV=1023
		BIQ->BIQ_CTRL.UPSR=2; 		//(BIQ SR)*(UPSR+1) when SR_DIV=3071
		BIQ->BIQ_CTRL.SELPWM=0;
	}

	Tdelay();
	BIQ->BIQ_CTRL.PRGCOEF=0;
	Tdelay();
	BIQ->BIQ_CTRL.EN=1;
}

