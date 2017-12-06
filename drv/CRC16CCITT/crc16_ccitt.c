#include "crc16_ccitt.h"

#define POLY 0x1021 /* crc-ccitt mask */
#define SEED 0xffff /* initial value */

static unsigned short crc = SEED;


static void update_crc(unsigned short ch) 
{ 
    /* based on code found at 
    http://www.programmingparadise.com/utility/crc.html 
    */ 

    unsigned short i, xor_flag; 

    /* 
    Why are they shifting this byte left by 8 bits?? 
    How do the low bits of the poly ever see it? 
    */ 
    ch<<=8; 

    for(i=0; i<8; i++) 
    { 
        if ((crc ^ ch) & 0x8000) 
        { 
            xor_flag = 1; 
        } 
        else 
        { 
            xor_flag = 0; 
        } 
        crc = crc << 1; 
        if (xor_flag) 
        { 
            crc = crc ^ POLY; 
        } 
        ch = ch << 1; 
    } 
}


unsigned short cal_crc(char* data, int size)
{
	int i = 0;
	unsigned short result;
	crc = SEED;

	for( i=0; i < size; i++)
	{
		update_crc( data[i] );
	}

	result = crc;
	crc = SEED;

	return result;
}
