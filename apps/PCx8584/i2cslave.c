
/***************************************************************************/
/* Acronym : I2C Inter IC bus (for PCF8584) */
/* Name of module : I2CSLAVE.C */
/* Scope : Application software */
/* 12nc : xxxx xxx xxxx.x */
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
/* Part of the I²C driver that handles slave bus-transfers. */
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
extern void (*slaveProc)(); /* Handle Slave Transfer action */
static BYTE count; /* bytes send/received of current message */
static BYTE slaveStatus; /* status of the slave */
static BYTE size; /* size of slave mode buffer */
static BYTE *slaveBuf; /* ptr to rec/trm data into/from if slave */


void HandleSlaveState(void)
/**************************
* Input(s) : None.
* Output(s) : None.
* Returns : None.
* Description: Procedure to handle actions if addressed as slave.
***************************************************************************/
{
switch (slaveStatus)
{
case SLAVE_IDLE:
if (CS_8584 & AAS_MASK) {
/* addressed as slave ? */
count = 0;
if (DR_8584 & 1)
{
slaveStatus = SLAVE_TRX; /* slave transmitter */
DR_8584 = slaveBuf[count++]; /* sent first byte */
}
else /* slave receiver */
if (size > 1)
{
slaveStatus = SLAVE_RCV;
CS_8584 = PIN_MASK | ESO_MASK | ACK_MASK | intMask;
}
else
{
slaveStatus = SLAVE_LAST;
CS_8584 = PIN_MASK | ESO_MASK | intMask;
}
}
break;
case SLAVE_TRX:
if (CS_8584 & LRB_MASK)
{
CS_8584 = PIN_MASK; /* no ack from master */
slaveStatus = SLAVE_READY; /* last byte transmitted */
}
else
DR_8584 = slaveBuf[count++]; /* sent next byte */
break;
case SLAVE_RCV:
slaveBuf[count++] = DR_8584;
if (count == size)
{
CS_8584 = ESO_MASK | intMask; /* clear ACK */
slaveStatus = SLAVE_LAST;
}
else
CS_8584 = ESO_MASK | ACK_MASK | intMask; /* set ACK */
break;
case SLAVE_LAST:
slaveBuf[count] = DR_8584;
CS_8584 = ESO_MASK | ACK_MASK | intMask; slaveStatus = SLAVE_READY; break;
default:
CS_8584 = PIN_MASK; break;
/* set ACK */
/* last byte received */
/* clear interrupt */
}
}


void I2C_InitializeSlave(BYTE slv, BYTE *buf, BYTE len, BYTE speed) 
/******************************************************************
* Input(s) : slv Own slave address
* buf Pointer to slave data buffer
* size size of the slave data buffer
* speed clock register value for bus speed
* Output(s) : None.
* Returns : None.
* Description : Enable I²C (slave) bus and set the clock speed for I²C.
***************************************************************************/
{
slaveProc = HandleSlaveState; /* Set pointer to correct proc. */
CR_8584 = 0; /* disable i2c interface */
AR_8584 = slv; /* write own slave address */
CR_8584 = 0x20; /* write clock register */
CL_8584 = speed;
intMask = 0;
size = len;
slaveBuf = buf;
slaveStatus = SLAVE_IDLE;
CR_8584 = ESO_MASK; /* enable serial interface */
}


void I2C_ProcessSlave(void)
/**************************
* Input(s) : None.
* Output(s) : None.
* Returns : None.
* Description: Process the slave.
* This function must be called by the application to check
* the slave status. The USER should adapt this function to
* his personal needs (take the right action at a certain
* status).
***************************************************************************/
{
switch(slaveStatus)
{
case SLAVE_IDLE :
/* do nothing or fill transmit buffer for transfer */
break;
case SLAVE_TRX :
case SLAVE_RCV :
case SLAVE_LAST :
/* do nothing if interrupt driven, else poll PIN bit */
if (intMask == 0) /* 0 -> polling */
{
if (!(CR_8584 & PIN_MASK)) MainStateHandler();
/* wait until PIN bit is 0 */
}
break;
case SLAVE_READY :
/* read or fill buffer for next transfer, signal application slaveStatus = SLAVE_IDLE;
break;
*/
}
}