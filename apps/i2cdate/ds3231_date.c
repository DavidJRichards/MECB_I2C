/*
Copyright (c) 2015, Dieter Hauer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "i2c.h"

char weekdays[][10]={
	{"Sunday"},
	{"Monday"},
	{"Tuesday"},
	{"Wednesday"},
	{"Thursday"},
	{"Friday"},
	{"Saturday"}
};

char months[][4]={
	{"Jan"},
	{"Feb"},
	{"Mar"},
	{"Apr"},
	{"May"},
	{"Jun"},
	{"Jul"},
	{"Aug"},
	{"Sep"},
	{"Oct"},
	{"Nov"},
	{"Dec"}
};


char BCDToDecimal (char bcdByte); 
char DecimalToBCD (char decimalByte);

int main()
{
	int addr = 0x68;

	char status=0;
	char ack;
	char dayOfWeek;
	char dayOfMonth;
	char month;
	char year;
	char sec,min,hor;



    i2cStart();
    ack = i2cWrite(0x00);
    i2cStop();
	printf("\r\n");
		//ack [%d]\r\n", ack);


	i2cStart();
	status = i2cWrite( addr<<1 );
	if(status == 0)
	{
		printf("\r\ni2c error, no ACK [%d]\r\n", status);
		return 0;
	}
	else
	{
#if 0
		i2cWrite(0x00); // Addr
		i2cWrite(0x00); // seconds
		i2cWrite(0x00); // minutes
		i2cWrite(0x19); // hour
		i2cWrite(0x05); // weekday
		i2cWrite(0x29); // day
		i2cWrite(0x01); // month
		i2cWrite(0x26); // (20)year
		i2cStop();
#else
		i2cWrite( 0x00 ); // write sub addr
		i2cStop();
		i2cStart();
		i2cWrite( 0xd1 ); // read
		sec= BCDToDecimal(i2cRead()); // seconds
		i2cAck();
		min= BCDToDecimal(i2cRead()); // minutes
		i2cAck();
		hor= BCDToDecimal(i2cRead()); // hour
		i2cAck();
		dayOfWeek = BCDToDecimal(i2cRead());
		i2cAck();
		dayOfMonth = BCDToDecimal(i2cRead());
		i2cAck();
		month = BCDToDecimal(i2cRead());
		i2cAck();
		year = BCDToDecimal(i2cRead());
		i2cNack();
		i2cStop();
#endif
		}


	printf("\n\r%s",weekdays[dayOfWeek-1]);
	printf("\r\n20%02d-%s-%02d\r\n",year,months[month-1],dayOfMonth);
	printf("\r\n%02d:%02d:%02d\r\n",hor,min,sec);
	return 0;
}

char BCDToDecimal (char bcdByte)
{
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}
 
char DecimalToBCD (char decimalByte)
{
  return (((decimalByte / 10) << 4) | (decimalByte % 10));
}


