#ifndef G722IMPHDR
#define G722IMPHDR

#ifdef MAX_SAMPLE_RATE
#undef MAX_SAMPLE_RATE
#endif


#define MAX_SAMPLE_RATE 32000
#define MAX_FRAMESIZE   (MAX_SAMPLE_RATE/50)

#ifdef SIREN14
/* Max bit rate is 48000 bits/sec. */
#define MAX_BITS_PER_FRAME 960
#else
/* Max bit rate is 32000 bits/sec. */
#define MAX_BITS_PER_FRAME 640
#endif

#ifndef MAX_DCT_LENGTH
#ifndef DCT_LENGTH
#define DCT_LENGTH          320
#endif // DCT_LENGTH
#ifdef SIREN14
#define MAX_DCT_LENGTH      640
#else
#define MAX_DCT_LENGTH  DCT_LENGTH
#endif
#endif // MAX_DCT_LENGTH

#ifndef NUMBER_OF_REGIONS
#define NUMBER_OF_REGIONS 14
#endif

#ifndef MAX_NUMBER_OF_REGIONS
#ifdef SIREN14
#define MAX_NUMBER_OF_REGIONS   28
#else
#define MAX_NUMBER_OF_REGIONS NUMBER_OF_REGIONS
#endif
#endif

#pragma pack(4)
typedef struct VPHeader
{
    unsigned int Config;
    unsigned int VPver;
    unsigned int VPptr;
    unsigned int pVPtable;
    unsigned int VPNum;
    unsigned int VGNum;
    unsigned int CheckSumEndAddr;
    unsigned int Spare[3];   
    char *       pVPInfo;
    unsigned int BaseAddress;
    unsigned int ExtMemChkSumAddr;
    unsigned int UserVersion;
}VP_HEADER;

typedef struct tagG722_CODEC_CTL
{
	signed long  bit_rate;
	signed short  bandwidth;
	signed short  number_of_bits_per_frame;
	signed short  number_of_regions;
	signed short  frame_size;
}G722_CODEC_CTL;

typedef struct tagG722_ENC_CTX
{
    signed short  history[MAX_FRAMESIZE];
    signed short  mlt_coefs[MAX_FRAMESIZE]; // this is not history data
}G722_ENC_CTX;

#ifndef Rand_Obj_defined
#define Rand_Obj_defined
typedef struct
{
    signed short seed0;
    signed short seed1;
    signed short seed2;
    signed short seed3;
}Rand_Obj;
#endif // Rand_Obj_defined

typedef struct tagG722_DEC_CTX
{
    signed short  old_mag_shift;
#ifndef NO_FRAME_ERROR_CHECK
    signed short  old_decoder_mlt_coefs[MAX_DCT_LENGTH];
#endif
    signed short  old_samples[MAX_DCT_LENGTH>>1];
    Rand_Obj randobj;
    signed short  decoder_mlt_coefs[MAX_DCT_LENGTH]; // this is not history data
}G722_DEC_CTX;

#pragma pack()

extern void g722Init(G722_CODEC_CTL *cctl, signed long bit_rate, signed short bandwidth);
extern void g722encReset( G722_CODEC_CTL *cctl, G722_ENC_CTX *ctx);
extern void g722enc( G722_CODEC_CTL *cctl, G722_ENC_CTX *ctx,
			 signed short *in, signed short *out);
extern void g722decReset( G722_CODEC_CTL *cctl, G722_DEC_CTX *ctx);
extern void g722dec( G722_CODEC_CTL *cctl, G722_DEC_CTX *ctx,
			 signed short *in, signed short *out, unsigned short Gain);
#ifdef ROMLIB
typedef struct tagG722CODEC
{
    unsigned int signature;	
    unsigned int ROMlibCode;	
    char * pVPptr;
    char * pChecksumEndptr;
    void (*g722Init)(G722_CODEC_CTL *cctl, signed long bit_rate, signed short bandwidth);
#ifdef G722_ENCODER
	void (*g722encReset)( G722_CODEC_CTL *cctl, G722_ENC_CTX *ctx);
	void (*g722enc)( G722_CODEC_CTL *cctl, G722_ENC_CTX *ctx, signed short *in, signed short *out);
#endif // G722_ENCODER
#ifdef G722_DECODER
	void (*g722decReset)( G722_CODEC_CTL *cctl, G722_DEC_CTX *ctx);
	void (*g722dec)( G722_CODEC_CTL *cctl, G722_DEC_CTX *ctx, signed short *in, signed short *out, unsigned short Gain);
#endif // G722_DECODER
}G722CODEC;
#endif 

#endif // G722IMPHDR
