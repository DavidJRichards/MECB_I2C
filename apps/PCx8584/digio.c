
/***************************************************************************/
/* Acronym : I2C Inter IC bus */
/* Name of module : EXAMPLE.C */
/* Scope : Application software */
/* 12nc : xxxx xxx xxxx.x */
/* Creation date : 1995-08-01 */
/* Program language : C */
/* Name : P.H. Seerden */
/* */
/* */
/* (C) Copyright 1994 Philips Semiconductors B.V. */
/* Product Concept & Application Laboratory Eindhoven (PCALE) */
/* Eindhoven - The Netherlands */
/* */
/* All rights are reserved. Reproduction in whole or in part is */
/* prohibited without the written consent of the copyright owner. */
/* */
/***************************************************************************/
/* */
/* Description: */
/* */
/* I²C driver test, for PCx8584 */
/* */
/* Tested on MICROCORE 2 and I²C evaluation board type OM1016 */
/* */
/* - Read the time from the real time clock chip PCF8583. */
/* - Displays the time on LCD module PCF8577 and LED module SAA1064. */
/* - Reads keys from I/O expander PCF8574. */
/* - Depending of pushed keys send tone to loudspeaker PCD3312. */
/* */
/***************************************************************************/
/* */
/* History: */
/* */
/* 95-08-01 P.H. Seerden Initial version */
/* */
/* */
/***************************************************************************/

#include <stdio.h>
#include "i2cexprt.h"

#define PCA9685_WR 0x80
#define PCA9685_RD 0x81


static BYTE ledBuf[16];

static I2C_MESSAGE ledMsg;

static void Init(void)
{
//	void **ptr;
//	#define VECTOR_BASE 0x40000 /* start of vector table */
//	ptr = (void *) (VECTOR_BASE + (4 * 28));
	//*ptr = (void *) I2C_Interrupt;
//	I2C_InitializeMaster(0x10); /* 4.43MHz and SCL = 90Khz */
//	I2C_InitializeMaster(0x18); /* 8MHz and SCL = 90Khz */
//	I2C_InitializeMaster(0x19); /* 8MHz and SCL = 45Khz */
	I2C_InitializeMaster(0x1a); /* 8MHz and SCL = 11Khz */
//	I2C_InitializeMaster(0x1b); /* 8MHz and SCL = 1.5Khz */
	//I2C_InstallInterrupt(28); /* Interrupt vector number */

printf("\n\rinit auto1\n\r");
	ledMsg.address = PCA9685_WR;
	ledMsg.nrBytes = 2;
	ledMsg.buf = ledBuf;
	ledBuf[0] = 0; 		// sub address
	ledBuf[1] = 0xA0;	// auto increment
	I2C_Write(&ledMsg); 

printf("\n\rinit pwm\n\r");
	ledMsg.address = PCA9685_WR;
	ledMsg.nrBytes = 2;
	ledMsg.buf = ledBuf;
	ledBuf[0] = 0xFE; 	// sub address
	ledBuf[1] = 0x03;	// pwm
	I2C_Write(&ledMsg); 

printf("\n\nset led\n\r");
	ledMsg.address = PCA9685_WR;
	ledMsg.nrBytes = 9;
	ledMsg.buf = ledBuf;
	ledBuf[0] = 6; 		// sub address

	ledBuf[1] = 0x0;	// led 0
	ledBuf[2] = 0x0;
	ledBuf[3] = 0xFF;
	ledBuf[4] = 0x0F;

	ledBuf[5] = 0x0;	// led 1
	ledBuf[6] = 0x0;
	ledBuf[7] = 0xff;
	ledBuf[8] = 0x0F;

	I2C_Write(&ledMsg); /* led brightness */

#if 0
printf("\n\rinit auto0\n\r");
	ledMsg.address = PCA9685_WR;
	ledMsg.nrBytes = 2;
	ledMsg.buf = ledBuf;
	ledBuf[0] = 0; // sub address
	ledBuf[1] = 0xA0;
	I2C_Write(&ledMsg); /* led brightness */


	ledMsg.address = SAA1064_WR;
	ledMsg.nrBytes = 2;
	ledMsg.buf = ledBuf;
	ledBuf[0] = 0;
	ledBuf[1] = 0x47;
	I2C_Write(&ledMsg); /* led brightness */
	ledMsg.nrBytes = 5;
	
	rtcBuf[0] = 2; /* sub address */
	rtcBuf[1] = 0x00; /* seconds */
	rtcBuf[2] = 0x59; /* minutes */
	rtcBuf[3] = 0x23; /* hours */
	rtcMsg1.address = PCF8583_WR;
	rtcMsg1.nrBytes = 4;
	rtcMsg1.buf = rtcBuf;
	I2C_Write(&rtcMsg1); /* set clock */
	
	rtcBuf[0] = 2; /* sub address */
	rtcMsg1.nrBytes = 1;
	rtcMsg1.buf = rtcBuf;
	rtcMsg2.address = PCF8583_RD;
	rtcMsg2.nrBytes = 3;
	rtcMsg2.buf = rtcBuf;
	
	iopMsg.buf = iopBuf;
	iopMsg.nrBytes = 1;
	sndMsg.address = PCD3312_WR;
	sndMsg.buf = sndBuf;
	sndMsg.nrBytes = 1;
	
	lcdMsg.address = PCF8577_WR;
	lcdMsg.buf = lcdBuf;
	lcdMsg.nrBytes = 5;
#endif
}



void main(void)
{
	BYTE oldseconds = 0;
	
	Init();
	//ua_init(); /* init uart used for I/O redirection in printf() */
#if 0	
	while (1)
	{
		HandleKeys();
		rtcBuf[0] = 2; /* sub address */
		I2C_WriteRepRead(&rtcMsg1, &rtcMsg2);
		if (rtcBuf[0] != oldseconds) /* check if one second is passed */
		{
			oldseconds = rtcBuf[0];
			
			lcdBuf[0] = 0;
			if (oldseconds & 1)
				lcdBuf[1] = lcdTbl[rtcBuf[2] >> 4];
			else
				lcdBuf[1] = lcdTbl[rtcBuf[2] >> 4] | LCDDP;
			lcdBuf[2] = lcdTbl[rtcBuf[2] & 0x0F];
			lcdBuf[3] = lcdTbl[rtcBuf[1] >> 4];
			lcdBuf[4] = lcdTbl[rtcBuf[1] & 0x0F];
			I2C_Write(&lcdMsg);
			
			ledBuf[0] = 1;
			ledBuf[1] = 2;
			ledBuf[2] = ledTbl[rtcBuf[0] >> 4];
			ledBuf[3] = ledTbl[rtcBuf[0] & 0x0F];
			ledBuf[4] = 2;
			I2C_Write(&ledMsg);
		}
	}
#endif
}

