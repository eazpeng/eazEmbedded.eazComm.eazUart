/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         I2C.c
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171121
* Brief:        TODO: without the dead-lock process
*************************************************************************/
#include "I2C.h"
//#include "A31G12x_series.h"
//#include "A31G12x_pcu.h"

/**TODO.eaz
 * I2CDelay
 * void, void
 * eaz.org.171122
**/
static void I2CDelay()
{
    //NOP();
}

/**OK
 * I2CStart
 * void, PSimuI2C
 * eaz.org.171122
**/
void I2CStart(PSimuI2C pxsSimuI2C)
{
    pxsSimuI2C->SDA_SET();
    pxsSimuI2C->SCL_SET();
    I2CDelay();
    pxsSimuI2C->SDA_CLR();
    I2CDelay();
    pxsSimuI2C->SCL_CLR();
}

/**OK
 * I2CStop
 * void, PSimuI2C
 * eaz.org.171122
**/
void I2CStop(PSimuI2C pxsSimuI2C)
{
    I2CDelay();
    pxsSimuI2C->SCL_CLR();
    pxsSimuI2C->SDA_CLR();
    I2CDelay();
    pxsSimuI2C->SCL_SET();
    I2CDelay();
    pxsSimuI2C->SDA_SET();
    I2CDelay();
}

/**TODO.eaz
 * I2CReset
 * void, PSimuI2C
 * eaz.org.171122
 * Include SDA hang up process.
**/
void I2CReset(PSimuI2C pxsSimuI2C)
{
    ;
}

/**OK
 * I2CSendACK
 * void, PSimuI2C
 * eaz.org.171122
**/
void I2CSendACK(PSimuI2C pxsSimuI2C)
{
    pxsSimuI2C->SDA_CLR();
    pxsSimuI2C->SCL_CLR();
    I2CDelay();
    pxsSimuI2C->SCL_SET();
    I2CDelay();
    pxsSimuI2C->SCL_CLR();
}

/**OK
 * I2CSendNACK
 * void, PSimuI2C
 * eaz.org.171122
**/
void I2CSendNACK(PSimuI2C pxsSimuI2C)
{
    pxsSimuI2C->SDA_SET();
    pxsSimuI2C->SCL_CLR();
    I2CDelay();
    pxsSimuI2C->SCL_SET();
    I2CDelay();
    pxsSimuI2C->SCL_CLR();
}

/**OK
 * I2CWaitACK
 * I2C_ACK_STATE, PSimuI2C
 * eaz.org.171122
**/
I2C_ACK_STATE I2CWaitACK(PSimuI2C pxsSimuI2C)
{
    I2C_ACK_STATE eACK;

    pxsSimuI2C->SCL_CLR();
    pxsSimuI2C->SDA_SET();
    I2CDelay();
    pxsSimuI2C->SCL_SET();
    I2CDelay();
    if(pxsSimuI2C->SDA_VALUE())
    {
        eACK = I2C_NACK;
    }
    else
    {
        eACK = I2C_ACK;
    }
    pxsSimuI2C->SCL_CLR();

    return eACK;
}

/**OK
 * I2CSendByte
 * SIMU_I2C_STATE, PSimuI2C, unsigned char
 * eaz.org.171122
**/
SIMU_I2C_STATE I2CSendByte(PSimuI2C pxsSimuI2C, unsigned char ucData)
{
    unsigned char ucCounter;
    unsigned char ucShiftRegister;

    //DI();
    I2CDelay();
    ucShiftRegister = 0x80;
    for(ucCounter=0; ucCounter<8; ucCounter++)
    {
        if(ucData & ucShiftRegister)
        {
            pxsSimuI2C->SDA_SET();
        }
        else
        {
            pxsSimuI2C->SDA_CLR();
        }

        I2CDelay();
        pxsSimuI2C->SCL_SET();
        I2CDelay();
        pxsSimuI2C->SCL_CLR();
        ucShiftRegister >>= 1;
    }

    //EI();
    return SMI2C_SEND_OK;
}

/**OK
 * I2CReceiveByte
 * SIMU_I2C_STATE, PSimuI2C, unsigned char *
 * eaz.org.171122
**/
SIMU_I2C_STATE I2CReceiveByte(PSimuI2C pxsSimuI2C, unsigned char *pucData)
{
    unsigned char ucReceiveByte;
    unsigned char ucCounter;
    unsigned char ucShiftRegister;

    //DI();

    ucShiftRegister  =0x80;
    ucReceiveByte = 0x00;
    I2CDelay();
    pxsSimuI2C->SDA_SET();
    pxsSimuI2C->SCL_CLR();

    for(ucCounter=0; ucCounter<8; ucCounter++)
    {
        I2CDelay();
        pxsSimuI2C->SCL_SET();
        I2CDelay();

        if(pxsSimuI2C->SDA_VALUE())
        {
            ucReceiveByte |= ucShiftRegister;
        }

        pxsSimuI2C->SCL_CLR();
        ucShiftRegister >>= 1;
    }

    I2CDelay();
    pxsSimuI2C->SCL_SET();
    I2CDelay();
    pxsSimuI2C->SCL_CLR();
    *pucData = ucReceiveByte;

    //EI();
    return SMI2C_RECEIVE_OK;
}
