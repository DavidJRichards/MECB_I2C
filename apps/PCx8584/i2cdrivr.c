/***************************************************************************/
/* Acronym : I2C Inter IC bus (for PCF8584) */
/* Name of module : I2CDRIVR.C */
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
/* Main part of the I²C driver. */
/* Contains the interrupt handler and does calls to the master */
/* and/or slave driver part. */
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

static void NoInitErrorProc(void);

void (*masterProc)(void) = NoInitErrorProc;
void (*slaveProc)(void) = NoInitErrorProc;

BYTE master;
BYTE intMask;


static void NoInitErrorProc(void)
/********************************
* Input(s) : none.
* Output(s) : none.
* Returns : none.
* Description : ERROR: Master or slave handler called while not initialized
***************************************************************************/
{
	CR_8584 = 0xC0 | intMask;	/* release bus NoAck */
}


void MainStateHandler(void)
/**************************
* Input(s) : none.
* Output(s) : none.
* Returns : none.
* Description : Main event handler for I2C.
***************************************************************************/
{
	if (master)
		masterProc(); /* Master Mode */
	else
		slaveProc(); /* Slave Mode */
}

#define interrupt
interrupt void I2C_Interrupt(void)
/*********************************
* Input(s) : none.
* Output(s) : none.
* Returns : none.
* Description : Interrupt handler for I2C.
***************************************************************************/
{
	MainStateHandler();
}


void I2C_InstallInterrupt(BYTE vector)
/*************************************
* Input(s) : vector Interrupt vector for register S3
* Output(s) : none.
* Returns : none.
* Description : Install and enable interrupt for I²C.
***************************************************************************/
{
	CR_8584 = 0x10; /* write interrupt vector */
	VR_8584 = vector; /* set vector number */
	
	intMask = ENI_MASK;
	
	CR_8584 = ESO_MASK | ENI_MASK; /* set serial interface ON */
}


