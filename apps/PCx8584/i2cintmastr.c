/***************************************************************************/
/* Acronym : I2C Inter IC bus (for PCx8584) */
/* Name of module : I2CMASTR.C */
/* Creation date : 1995-08-01 */
/* Program language : C */
/* Name : P.H. Seerden */
/* */
/* */
/* (C) Copyright 1995 Philips Semiconductors B.V. */
/* Product Concept & Application Laboratory Eindhoven (PCALE) */
/* Eindhoven - The Netherlands */
/* */
/***************************************************************************/
/* */
/* Description: */
/* */
/* Master driver part for the Philips PCx8584 I²C bus controller. */
/* */
/* Everything between one Start and Stop condition is called a TRANSFER. */
/* One transfer consists of one or more MESSAGEs. */
/* To start a transfer call function "I2C_Transfer". */
/* */
/* */
/***************************************************************************/
/* */
/* History: */
/* */
/* 95-08-01 P.H. Seerden Initial version */
/* */
/* */
/***************************************************************************/

#include "i2cexprt.h"
#include "i2cdrivr.h"
#include "reg8584.h"

#define NULL ((void *) 0) /* a null pointer */

extern void (*masterProc)(); /* Handle Master Transfer action */
extern void (*slaveProc)(); /* Handle Slave Transfer action */

static I2C_TRANSFER *tfr; /* Ptr to active transfer block */
static I2C_MESSAGE *msg; /* ptr to active message block */

static void (*readyProc)(BYTE,BYTE); /* proc. to call if transfer ended */
static BYTE mssgCount; /* Number of messages sent */
static BYTE dataCount; /* nr of bytes of current message */
static BYTE state; /* state of the I²C driver */


static void GenerateStop(BYTE status)
/************************************
* Input(s) : status status of the driver.
* Output(s) : None.
* Returns : None.
* Description : Generate a stop condition.
***************************************************************************/
{
	CR_8584 = PIN_MASK | ESO_MASK | intMask | STO_MASK | ACK_MASK;
	master = FALSE;
	state = ST_IDLE;
	
	readyProc(status, mssgCount); /* Signal driver is finished */
}

static void HandleMasterState(void)
/**********************************
* Input(s) : None.
* Returns : None.
* Description : Master mode state handler for I2C bus.
***************************************************************************/
{
	if (CS_8584 & LAB_MASK)
	{ /* arbitration was lost */
		slaveProc(); /* check if addressed as slave */
		GenerateStop(I2C_ARBITRATION_LOST); /* leave the bus */
		return;
	}
	switch (state)
	{
		case ST_SENDING :
			if (CS_8584 & LRB_MASK)
				GenerateStop(I2C_NACK_ON_DATA);
			else
				if (dataCount < msg->nrBytes)
					DR_8584 = msg->buf[dataCount++]; /* sent next byte */
				else
					if (mssgCount < tfr->nrMessages)
					{
						dataCount = 0;
						msg = tfr->p_message[mssgCount++];
						state = (msg->address & 1) ? ST_AWAIT_ACK : ST_SENDING;
						CS_8584 = ESO_MASK | STA_MASK | ACK_MASK | intMask;
						DR_8584 = msg->address;
					}
					else
						GenerateStop(I2C_OK); /* transfer ready */
			break;
		case ST_AWAIT_ACK :
			if (CS_8584 & LRB_MASK)
				GenerateStop(I2C_NACK_ON_ADDRESS);
			else
			{
				BYTE dummy;
				if (msg->nrBytes == 1)
				{
					CS_8584 = ESO_MASK | intMask; /* clear ACK */
					state = ST_RECV_LAST;
				}
				else
					state = ST_RECEIVING;
				dummy = DR_8584; /* generate clk pulses for first byte */
			}
			break;
		case ST_RECEIVING :
			if (dataCount + 2 == msg->nrBytes)
			{
				CS_8584 = ESO_MASK | intMask; /* clear ACK */
				state = ST_RECV_LAST;
			}
			msg->buf[dataCount++] = DR_8584;
			break;
		case ST_RECV_LAST :
			if (mssgCount < tfr->nrMessages)
			{
				msg->buf[dataCount] = DR_8584;
				dataCount = 0;
				msg = tfr->p_message[mssgCount++];
				state = (msg->address & 1) ? ST_AWAIT_ACK : ST_SENDING;
				CS_8584 = ESO_MASK | STA_MASK | ACK_MASK | intMask;
				DR_8584 = msg->address;
			}
			else
			{
				GenerateStop(I2C_OK); /* transfer ready */
				msg->buf[dataCount] = DR_8584;
			}
			break;
		default : /* impossible */
			GenerateStop(I2C_ERR); /* just to be sure */
			break;
	}
}


void I2C_Transfer(I2C_TRANSFER *p, void (*proc)(BYTE, BYTE))
/***********************************************************
* Input(s) : p address of I²C transfer parameter block.
* proc procedure to call when transfer completed,
* with the driver status passed as parameter.
* Output(s) : None.
* Returns : None.
* Description: Start an I²C transfer, containing 1 or more messages. The
* application must leave the transfer parameter block
* untouched until the ready procedure is called.
***************************************************************************/
{
	tfr = p;
	readyProc = proc;
	mssgCount = 0;
	dataCount = 0;
	master = TRUE;
	msg = tfr->p_message[mssgCount++];
	
	state = (msg->address & 1) ? ST_AWAIT_ACK : ST_SENDING;
	CS_8584 = ESO_MASK | STA_MASK | ACK_MASK | intMask; /* generate start */
	DR_8584 = msg->address;
}


void I2C_InitializeMaster(BYTE speed) /************************************
* Input(s) : speed clock register value for bus speed.
* Output(s) : None.
* Returns : None.
* Description: Initialize the PCF8584 as I2C bus master.
***************************************************************************/
{
	state = ST_IDLE;
	readyProc = NULL;
	
	masterProc = HandleMasterState; /* Set pointer to correct proc. */
	
	AR_8584 = 0x26; /* dummy own slave address */

	CR_8584 = 0x20; /* write clock register */
	CL_8584 = speed;
	CR_8584 = ESO_MASK; /* enable serial interface */
	
	intMask = 0;
	master = FALSE;
}

