
/*
 * libNuG726.h
 * Nuvoton G726 CODEC implementation
 * Header file for CCITT conversion routines.
 *
 */

#include "math.h"
#ifndef _G72X_H
#define	_G72X_H

#define	AUDIO_ENCODING_ULAW	(1)	/* ISDN u-law */
#define	AUDIO_ENCODING_ALAW	(2)	/* ISDN A-law */
#define	AUDIO_ENCODING_LINEAR	(3)	/* PCM 2's-complement (0-center) */

/*
 * The following is the definition of the state structure
 * used by the G.721/G.723 encoder and decoder to preserve their internal
 * state between successive calls.  The meanings of the majority
 * of the state structure fields are explained in detail in the
 * CCITT Recommendation G.721.  The field names are essentially indentical
 * to variable names in the bit level description of the coding algorithm
 * included in this Recommendation.
 */
typedef struct g726_state_s {
    long  yl;	/* Locked or steady state step size multiplier. */
    int yu;	/* Unlocked or non-steady state step size multiplier. */
    int dms;	/* Short term energy estimate. */
    int dml;	/* Long term energy estimate. */
    int ap;	/* Linear weighting coefficient of 'yl' and 'yu'. */

    int a[2];	/* Coefficients of pole portion of prediction filter. */
    int b[6];	/* Coefficients of zero portion of prediction filter. */
    int pk[2];	/*
                        ** Signs of previous two samples of a partially
                        ** reconstructed signal.
                        **/
    int dq[6];	/*
                        ** Previous 6 samples of the quantized difference
                        ** signal represented in an internal floating point
                        ** format.
                        **/
    int sr[2];	/*
                        ** Previous 2 samples of the quantized difference
                        ** signal represented in an internal floating point
                        ** format.
                        */
    char td;	/* delayed tone detect, new in 1988 version */
} g726_state;

/* External function definitions. */

void g726_init_state( g726_state *);

int g726_32_encoder(
    int sample,
    int in_coding,
    g726_state *state_ptr);
int g726_32_decoder(
    int code,
    int out_coding,
    g726_state *state_ptr);

#endif 


