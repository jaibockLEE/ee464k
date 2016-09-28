/**********************************************************/ 
/* (C) 2011 Nuvoton                                       */
/**********************************************************/ 
/* Project:        ISD1960                                */
/* Author:         GBJ          Creation Date:  07 Nov 11 */
/*                                                        */
/**********************************************************/
/**********************************************************/ 
/* Global Definitions                      		          */ 
/**********************************************************/

#ifndef SUCCESS
#define SUCCESS    0
#endif
#ifndef FAIL
#define FAIL   -1
#endif

#ifndef FINISHED
#define FINISHED   0
#endif



#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

typedef unsigned char BOOL;
typedef unsigned char BYTE;
typedef unsigned short int WORD;
 
/*********************************************************/
/* Structure and union templates plus other definitions  */
/*********************************************************/
struct CompressParameters
{
    int CompandType;
    BOOL E2;
    BYTE Bits;
    BOOL VBR;
    BOOL MSVBR;
    BYTE ADPCMMaxBits;
    BYTE PCMRes;
    int CompSize;
    int Bitrate;
    int Bandwidth;
    int SampleRate;
    int SampleRateCode;
};

typedef struct S_WADPCMCodecContext
{
    // Data
    BYTE * pData;
    BYTE  byBitsLeft;
    BYTE * pEndData;
    // ADPCM State
    short		In;				// Current index of Quantizer step size
    short		nDecV;  			// Last decoded value
    short		nDecV2;				// Decoded value at time (n-2)
    short		Pn;				// Predicted Value
    unsigned char		Ln;				// ADPCM code
    // Quantizer Step Size table parameters
    //short*		pQStepTable;			// Pointer to the step quantization table
    unsigned short		nMaxQSteps;			// Size of quantization step table
    short		nEndCodeIndex;			// Last index of step table
    unsigned short		wUpperQTableIndexBound;	// Maximum Quantizer step index to use
    unsigned short		wStepAdjustTableIndex;	// Which stepsize table to use
    signed char*		pStepAdjustTableRoot;	// Pointer to the first step adjust table
    signed char*		pStepAdjustTable;		// Pointer to the step adjust table in use
    unsigned short		wStepAdjustTableNum;	// Number of Step Adjust tables
    // Predictor parameters
    unsigned char		byK1Index;				// K1 2nd Order Predictor coefficient index
    unsigned char		byK2Index;				// K2 2nd Order Predictor coefficient index

    // Data format specification parameters
    int			nSampleMiddleValue;		// Middle value of unsigned PCM code
    int			nSampleMaxValue;		// Maximum value of unsigned PCM code	
    int			nSampleSignedMaxValue;	//  Maximum value of signed PCM code	
    int			nSampleSignedMinValue;	//  Minimum value of signed PCM code	
    // ADPCM data format parameters
    unsigned short	wNumBits;					// Number of ADPCM bits
    unsigned char	    byADPCMBitMask;				// Bit mask for ADPCM bits e.g. b1111 for 4bit
    BYTE      SimpleADPCM;		   	// A flag to indicate simple APDCM - Ist order,
    BYTE        CompCode;               // Compression Code.
    // Variable bit rate encoding paramters.
    BYTE    MSVBR;			// A flag to turn on multi-standard variable
    // bit rate
    int SampleRate;
    int FrameSize;
    short *FrameBuffer;
}WADPCMCodecContext;

#define COMP_VBR        28
#define COMP_ADPCM      0x00
#define COMP_E2ADPCM    8
#define COMP_ULAW       16
#define COMP_PCM        24

/*********************************************************/
/* Check for inclusion in home module                    */ 
/*********************************************************/
#ifdef  __NUADPCM__
/*********************************************************/ 
/* Within Module Function Prototypes                     */ 
/*********************************************************/
void WADPCM_SetupCodec(
    BYTE    byADPCMbits,
    struct S_WADPCMCodecContext *pCon
    );
int NuAdpcmDecodeFrame( struct S_WADPCMCodecContext *pCon, short *Frame_ptr);
void NuUlawDecodeFrame(struct S_WADPCMCodecContext *pCon, int DuLawFlag);
short int ulaw2linear(unsigned char ulawbyte, int uLawBits);
WORD GetBits( struct S_WADPCMCodecContext *pCon );
void AdpcmDecodeFrame(struct S_WADPCMCodecContext *pCon);
short WADPCM_Decode(BYTE byLn, struct S_WADPCMCodecContext *pCon);
int WADPCMCodec_Predict(struct S_WADPCMCodecContext *pCon);
void NuPcmDecodeFrame(struct S_WADPCMCodecContext *pCon);
/*********************************************************/
/* Function prototypes from home module                  */
/*********************************************************/

/*********************************************************/ 
/* Within Module Data Declarations                       */ 
/*********************************************************/
int g_FrameCount;
const	unsigned char	g_byK1BinaryRep[8] =
{
    0x08,		//  0.5 + 0.000 ( 0.1000)
    0x0A,		//  0.5 + 0.125 ( 0.1010)
    0x0C,		//  0.5 + 0.250 ( 0.1100)
    0x0E,		//  0.5 + 0.375 ( 0.1110)
    0x10,		//  0.5 + 0.500 ( 1.0000)
    0x12,		//  0.5 + 0.625 ( 1.0010)
    0x14,		//  0.5 + 0.750 ( 1.0100)
    0x16,		//  0.5 + 0.875 ( 1.0110)
};

const	unsigned char	g_byK2BinaryRep[8] =
{
    0x80,		// -0.5 + 0.000 (-0.1000)
    0x82,		// -0.5 + 0.125 (-0.0110)
    0x84,		// -0.5 + 0.250 (-0.0100)
    0x86,		// -0.5 + 0.375 (-0.0010)
    0x00,		// -0.5 + 0.500 ( 0.0000)
    0x02,		// -0.5 + 0.625 ( 0.0010)
    0x04,		// -0.5 + 0.750 ( 0.0100)
    0x06,		// -0.5 + 0.875 ( 0.0110)
};

const signed char g_n5bitStepAdjustTable[4*4][16] = 
{
    // Table Zero
    { -1, -1, -1, -1, -1, -1, -1, -1,  2,  4,  6,  8, 10, 12, 14, 16 },
    { -2, -2, -2, -2, -2, -2, -2, -2,  5,  7,  9, 11, 13, 15, 17, 19 },
    { -1, -1, -1, -1, -1, -1, -1, -1,  4,  6,  8, 10, 12, 14, 16, 18 },
    { -2, -2, -2, -2, -2, -2, -2, -2,  2,  4,  8, 10, 10, 12, 14, 16 },
    // Table 1
    { -1, -1, -1, -1, -1, -1, -1, -1,  2,  4,  6,  8, 10, 12, 14, 16 },
    { -3, -3, -2, -2, -2, -1, -1, -1,  5,  7,  9, 11, 13, 15, 17, 19 },
    { -1, -1, -1, -1, -1, -1, -1, -1,  4,  6,  8, 10, 12, 14, 16, 18 },
    { -2, -2, -2, -2, -1, -1, -1, -1,  2,  4,  8, 10, 10, 12, 14, 16 },
    // Table 2
    { -1, -1, -1, -1, -1, -1, -1, -1,  2,  4,  6,  8, 10, 12, 14, 16 },
    { -2, -2, -2, -2, -2, -2, -2, -2,  5,  7,  9, 12, 15, 19, 22, 25 },
    { -1, -1, -1, -1, -1, -1, -1, -1,  4,  6,  8, 10, 12, 14, 16, 18 },
    { -2, -2, -2, -2, -2, -2, -2, -2,  2,  4,  8, 10, 10, 12, 14, 16 },
    // Table 3
    { -1, -1, -1, -1, -1, -1, -1, -1,  2,  4,  6,  8, 10, 12, 14, 16 },
    { -2, -2, -2, -2, -2, -2, -2, -2,  5,  7,  9, 11, 13, 15, 17, 19 },
    { -1, -1, -1, -1, -1, -1, -1, -1,  4,  6,  8, 10, 12, 14, 16, 18 },
    { -2, -2, -2, -2, -2, -2, -2, -2,  2,  4,  8, 10, 10, 12, 14, 16 },
};



const signed char g_n4bitStepAdjustTable[4*4][8] = 
{
    { -1, -1, -1, -1,  2,  4,  6,  8 },
    { -2, -2, -2, -2,  5,  7,  9, 11 },
    { -1, -1, -1, -1,  4,  6,  8, 10 },
    { -2, -2, -2, -2,  2,  4,  8, 10 },
    // Table two
    { -1, -1, -1, -1,  2,  4,  6,  8 },
    { -2, -2, -1, -1,  5,  7,  9, 11 },
    { -1, -1, -1, -1,  4,  6,  8, 10 },
    { -2, -2, -1, -1,  2,  4,  8, 10 },
    // Table three
    { -1, -1, -1, -1,  2,  4,  6,  8 },
    { -3, -2, -1, -1,  5,  7,  9, 11 },
    { -1, -1, -1, -1,  4,  6,  8, 10 },
    { -3, -2, -1, -1,  2,  4,  8, 10 },
    // Table four
    { -1, -1, -1, -1,  2,  4,  6,  8 },
    { -4, -3, -2, -1,  5,  7,  9, 11 },
    { -1, -1, -1, -1,  4,  6,  8, 10 },
    { -3, -2, -1, -1,  2,  4,  8, 10 },
	
};
const signed char g_n3bitStepAdjustTable[4*4][4] = 
{
    // Table Zero
    {  -1, -1,  2,  4 },
    {  -2, -2,  5,  7 },
    {  -1, -1,  4,  6 },
    {  -2, -2,  2,  4 },
    // Table one
    {  -1, -1,  2,  4 },
    {  -2, -1,  5,  7 },
    {  -1, -1,  4,  6 },
    {  -2, -1,  2,  4 },
    // Table two
    {  -1, -1,  1,  2 },
    {  -2, -2,  5,  7 },
    {  -1, -1,  3,  6 },
    {  -2, -2,  2,  4 },
    // Table three
    {  -1, -1,  2,  4 },
    {  -2, -2,  5,  7 },
    {  -1, -1,  4,  6 },
    {  -2, -2,  2,  4 },
};
const signed char g_n2bitStepAdjustTable[4][2] = 
{
    // Table Zero
    {   -1,  2 },
    {   -2,  5 },
    {   -1,  4 },
    {   -2,  2 },
	
};

const signed short g_qStepTable[64] =
{
    32,     36,     40,     44,     48,     52,     56,     60,
    64,     72,     80,     88,     96,     104,    112,    120,
    128,    144,    160,    176,    192,    208,    224,    240,
    256,    288,    320,    352,    384,    416,    448,    480,
    512,    576,    640,    704,    768,    832,    896,    960,
    1024,   1152,   1280,   1408,   1536,   1664,   1792,   1920,
    2048,   2304,   2560,   2816,   3072,   3328,   3584,   3840,
    4096,   4608,   5120,   5632,   6144,   6656,   7168,   7680
};


/*********************************************************/ 
#else
/*********************************************************/ 
/*********************************************************/
/* External function prototypes for use by other modules */
/*********************************************************/
void NuAdpcmInit(BYTE * StartAddress, BYTE * EndAddress, struct S_WADPCMCodecContext *pCon);
int NuAdpcmDecodeFrame( struct S_WADPCMCodecContext *pCon, short *Frame_ptr);
/*********************************************************/ 
/* External Data Declarations                            */ 
/*********************************************************/

/*********************************************************/
/* External data definitions for use by other modules    */
/*********************************************************/

/*********************************************************/
#endif
/*********************************************************/
