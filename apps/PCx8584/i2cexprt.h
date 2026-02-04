/***************************************************************************/
/* Acronym : I2C (I2C Driver package for PCF8584) */
/* Name of module : I2CEXPRT.H */
/* Creation date : 1995-08-01 */
/* Program language : C */
/* Name : P.H. Seerden */
/* */
/* Description : This module consists a number of exported */
/* declarations of the I2C driver package. Include */
/* this module in your source file if you want to */
/* make use of one of the interface functions of the */
/* package. */
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

//#define NULL ((void *) 0) /* a null pointer */

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long LONG;

typedef struct
{
	BYTE address; /* slave address to sent/receive message */
	BYTE nrBytes; /* number of bytes in message buffer */
	BYTE *buf; /* pointer to application message buffer */
} I2C_MESSAGE;

typedef struct
{
	BYTE nrMessages; /* number of message in one transfer */
	I2C_MESSAGE **p_message; /* pointer to pointer to message */
} I2C_TRANSFER;

/***************************************************************************/
/* E X P O R T E D D A T A D E C L A R A T I O N S */
/***************************************************************************/

#define FALSE 0
#define TRUE 1

#define I2C_WR 0
#define I2C_RD 1

/**** Status Errors ****/
#define I2C_OK 					0 /* transfer ended No Errors */
#define I2C_BUSY 				1 /* transfer busy */
#define I2C_ERR 				2 /* err: general error */
#define I2C_NO_DATA 			3 /* err: No data in block */
#define I2C_NACK_ON_DATA 		4 /* err: No ack on data */
#define I2C_NACK_ON_ADDRESS 	5 /* err: No ack on address */
#define I2C_DEVICE_NOT_PRESENT 	6 /* err: Device not present */
#define I2C_ARBITRATION_LOST 	7 /* err: Arbitration lost */
#define I2C_TIME_OUT 			8 /* err: Time out occurred */
#define I2C_SLAVE_ERROR 		9 /* err: Slave mode error */
#define I2C_INIT_ERROR 			10 /* err: Initialization (not done) */


/***************************************************************************/
/* I N T E R F A C E F U N C T I O N P R O T O T Y P E S */
/***************************************************************************/

extern void I2C_InitializeMaster(BYTE speed);
extern void I2C_InitializeSlave(BYTE slv, BYTE *buf, BYTE size, BYTE speed);
extern void I2C_InstallInterrupt(BYTE vector);
//extern interrupt void I2C_Interrupt(void);

extern void I2C_Write(I2C_MESSAGE *msg);
extern void I2C_WriteRepWrite(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2);
extern void I2C_WriteRepRead(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2);
extern void I2C_Read(I2C_MESSAGE *msg);
extern void I2C_ReadRepRead(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2);
extern void I2C_ReadRepWrite(I2C_MESSAGE *msg1, I2C_MESSAGE *msg2);

extern void I2C_Transfer(I2C_TRANSFER *p, void (*proc)(BYTE status, BYTE msgsDone));
extern void MainStateHandler(void);