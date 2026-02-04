
/***************************************************************************/
/* Acronym : GENERAL */
/* Name of module : REG8584.H */
/* Creation date : 1995-08-01 */
/* Program language : C */
/* Name : P.H. Seerden */
/* */
/* Description : Hardware register (I/O port) description file of */
/* the PCx8584 I²C - bus controller, for use in C */
/* programs. */
/* */
/* !!!! CHANGE ADDRESSES FOR OTHER APPLICATIONS !!!! */
/* */
/* Copyright (C) Philips Semiconductors B.V. */
/* Product Concept & Application Laboratory Eindhoven (PCALE) */
/* Eindhoven - The Netherlands */
/* */
/***************************************************************************/
/* */
/* History: */
/* */
/* 95-08-01 P.H. Seerden Initial version */
/* */
/* */
/***************************************************************************/

#define BYTE_AT(x) (*(unsigned char *)x)

/* !!!! CHANGE ADDRESSES FOR OTHER APPLICATIONS !!!! */
									/*               ES0 ES1 ES2 */
#define AR_8584 BYTE_AT(0xBF78) 	/* Address Register 0 0 0 */
#define VR_8584 BYTE_AT(0xBF78) 	/* Vector Register  0 0 1 */
#define CL_8584 BYTE_AT(0xBF78) 	/* Clock Register   0 1 0 */
#define DR_8584 BYTE_AT(0xBF78) 	/* Data Register    1 0 0 */

#define CR_8584 BYTE_AT(0xBF79) 	/* Control Register 0 x x */
#define CS_8584 BYTE_AT(0xBF79) 	/* Cntrl/Status Reg 1 x x */
