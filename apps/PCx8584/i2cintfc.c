
/***************************************************************************/
/* Acronym : I²C Inter IC bus (for PCx8584) */
/* Name of module : I2CINTFC.C */
/* Creation date : 1995-08-01 */
/* Program language : C */
/* Name : P.H. Seerden */
/* */
/* Description : External interface to the PCx8584 I²C driver */
/* routines. This module contains the EXAMPLE */
/* interface functions, used by the application to */
/* do I²C master-mode transfers. */
/* */
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
#include <stdio.h>
#include "i2cexprt.h"
#include "i2cdrivr.h"
#include "reg8584.h"
static BYTE drvStatus; /* Status returned by driver */
static I2C_MESSAGE *p_iicMsg[2]; static I2C_TRANSFER iicTfr;
/* pointer to an array of (2) I2C mess */

static void I2cReady(BYTE status, BYTE msgsDone)
/***********************************************
* Input(s) : status Status of the driver at completion time
* msgsDone Number of messages completed by the driver
* Output(s) : None.
* Returns : None.
* Description: Signal the completion of an I²C transfer. This function is
* passed (as parameter) to the driver and called by the
* drivers state handler (!).
***************************************************************************/
{
	drvStatus = status;
}

static void StartTransfer(void)
/******************************
* Input(s) : None.
* Output(s) : statusfield of I2C_TRANSFER contains the driver status:
* I2C_OK Transfer was successful.
* I2C_TIME_OUT Timeout occurred
* Otherwise Some error occurred.
* Returns : None.
* Description: Start I²C transfer and wait (with timeout) until the
* driver has completed the transfer(s).
***************************************************************************/
{
LONG timeOut;
BYTE retries = 0;

	do
	{
		drvStatus = I2C_BUSY;
		I2C_Transfer(&iicTfr, I2cReady);

		timeOut = 0;
		while (drvStatus == I2C_BUSY)
		{
			if (++timeOut > 60000L)
				drvStatus = I2C_TIME_OUT;
			if (intMask == 0) 					/* 0 -> polling */
			{
				if (!(CR_8584 & PIN_MASK)) 		/* wait until PIN bit is 0 */
					MainStateHandler();
			}
		}

		if (retries == 6)
		{
			printf("retry counter expired\n"); 	/* fatal error ! So, .. */
//djrm			asm (" trap #15 "); 				/* escape to debug monitor */
			for(;;);
		}
		else
			retries++;

		switch (drvStatus)
		{
			case I2C_OK 				: break;
			case I2C_NO_DATA 			: printf("buffer empty\n"); break;
			case I2C_NACK_ON_DATA 		: printf("no ack on data\n"); break;
			case I2C_NACK_ON_ADDRESS 	: printf("no ack on address\n"); break;
			case I2C_DEVICE_NOT_PRESENT : printf("device not present\n"); break;
			case I2C_ARBITRATION_LOST 	: printf("arbitration lost\n"); break;
			case I2C_TIME_OUT 			: printf("time-out\n"); break;
			default 					: printf("unknown status\n"); break;
		}
	} while (drvStatus != I2C_OK);
}

void I2C_Write(I2C_MESSAGE *msg)
/*******************************
* Input(s) : msg I²C message
* Returns : None.
* Description: Write a message to a slave device.
* PROTOCOL : <S><SlvA><W><A><D1><A> ... <Dnum><N><P>
***************************************************************************/
{
	iicTfr.nrMessages = 1;
	iicTfr.p_message = p_iicMsg;
	p_iicMsg[0] = msg;

	StartTransfer();
}


void I2C_WriteRepWrite(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2) 
/***********************************************************
* Input(s) : msg1 first I²C message
* msg2 second I²C message
* Returns : None.
* Description: Writes two messages to different slave devices separated
* by a repeated start condition.
* PROTOCOL : <S><Slv1A><W><A><D1><A>...<Dnum1><A>
* <S><Slv2A><W><A><D1><A>...<Dnum2><A><P>
***************************************************************************/
{
	iicTfr.nrMessages = 2;
	iicTfr.p_message = p_iicMsg;
	p_iicMsg[0] = msg1;
	p_iicMsg[1] = msg2;

	StartTransfer();
}

void I2C_WriteRepRead(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2) 
/**********************************************************
* Input(s) : msg1 first I²C message
* msg2 second I²C message
* Returns : None.
* Description: A message is sent and received to/from two different
* slave devices, separated by a repeat start condition.
* PROTOCOL : <S><Slv1A><W><A><D1><A>...<Dnum1><A>
* <S><Slv2A><R><A><D1><A>...<Dnum2><N><P>
***************************************************************************/
{
	iicTfr.nrMessages = 2;
	iicTfr.p_message = p_iicMsg;
	p_iicMsg[0] = msg1;
	p_iicMsg[1] = msg2;

	StartTransfer();
}


void I2C_Read(I2C_MESSAGE *msg) 
/******************************
* Input(s) : msg I²C message
* Returns : None.
* Description: Read a message from a slave device.
* PROTOCOL : <S><SlvA><R><A><D1><A> ... <Dnum><N><P>
***************************************************************************/
{
	iicTfr.nrMessages = 1;
	iicTfr.p_message = p_iicMsg;
	p_iicMsg[0] = msg;

	StartTransfer();
}


void I2C_ReadRepRead(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2) 
/*********************************************************
* Input(s) : msg1 first I²C message
* msg2 second I²C message
* Returns : None.
* Description: Two messages are read from two different slave devices,
* separated by a repeated start condition.
* PROTOCOL : <S><Slv1A><R><A><D1><A>...<Dnum1><N>
* <S><Slv2A><R><A><D1><A>...<Dnum2><N><P>
***************************************************************************/
{
	iicTfr.nrMessages = 2;
	iicTfr.p_message = p_iicMsg;
	p_iicMsg[0] = msg1;
	p_iicMsg[1] = msg2;
	
	StartTransfer();
}


void I2C_ReadRepWrite(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2) 
/**********************************************************
* Input(s) : msg1 first I²C message
* msg2 second I²C message
* Returns : None.
* Description: A block data is received from a slave device, and also
* a(nother) block data is send to another slave device
* both blocks are seperated by a repeated start.
* PROTOCOL : <S><Slv1A><R><A><D1><A>...<Dnum1><N>
* <S><Slv2A><W><A><D1><A>...<Dnum2><A><P>
***************************************************************************/
{
	iicTfr.nrMessages = 2;
	iicTfr.p_message = p_iicMsg;
	p_iicMsg[0] = msg1;
	p_iicMsg[1] = msg2;
	
	StartTransfer();
}






