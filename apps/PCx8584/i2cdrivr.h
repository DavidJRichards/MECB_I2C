/***************************************************************************/
	
/* Acronym : I2C (I2C Driver package for PCF8584) */
/* Name of module : I2CDRIVR.H */
/* Creation date : 1995-08-01 */
/* Program language : C */
/* Name : P.H. Seerden */
/* */
/* Description : This module consists a number of 'local' */
/* declarations of the I2C PCx8584 driver package. */
/* */
/* (C) Copyright 1995 Philips Semiconductors B.V. */
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

#define ST_IDLE 0
#define ST_SENDING 1
#define ST_AWAIT_ACK 2
#define ST_RECEIVING 3
#define ST_RECV_LAST 4

#define SLAVE_IDLE 0
#define SLAVE_TRX 1
#define SLAVE_RCV 2
#define SLAVE_LAST 3
#define SLAVE_READY 4

#define ACK_MASK 0x01
#define STO_MASK 0x02
#define STA_MASK 0x04
#define ENI_MASK 0x08
#define ESO_MASK 0x40

#define BB_MASK 0x01
#define LAB_MASK 0x02
#define AAS_MASK 0x04
#define LRB_MASK 0x08
#define BER_MASK 0x10
#define STS_MASK 0x20
#define PIN_MASK 0x80

extern BYTE master;
extern BYTE intMask;