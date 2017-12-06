/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         I2C.h
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171121
* Brief:        TODO: without the dead-lock process
*************************************************************************/
#ifndef	_I2C_H_
#define	_I2C_H_

#include "stdbool.h"

// data structure
typedef struct tagSimuI2C
{
    unsigned char   ucDeviceAddr;   // include the higher address of eeprom

//    unsigned char   ucRI;           // receive ok
//    unsigned char   ucTI;           // need to send
//    unsigned char   ucTxData;       // one byte to send
//    unsigned char   ucRxData;       // one byte received

    void (*SCL_SET)(void);
    void (*SCL_CLR)(void);
    void (*SDA_SET)(void);
    void (*SDA_CLR)(void);
    bool (*SDA_VALUE)(void);
}SimuI2C, *PSimuI2C;

typedef enum
{
    SMI2C_VOID,
    SMI2C_SEND_OK,
    SMI2C_BUSY,
    SMI2C_RECEIVE_OK,
    SMI2C_PREEMPT_BUS,
    SMI2C_RELEASE_BUS,
    SMI2C_NO_REPLY,
}SIMU_I2C_STATE;

typedef enum
{
    I2C_ACK     = 0x00,
    I2C_NACK    = 0xFF
}I2C_ACK_STATE;

// interface
extern SIMU_I2C_STATE I2CSendByte(PSimuI2C pxsSimuI2C, unsigned char ucData);
extern SIMU_I2C_STATE I2CReceiveByte(PSimuI2C pxsSimuI2C, unsigned char *pucData);
extern I2C_ACK_STATE I2CWaitACK(PSimuI2C pxsSimuI2C);
extern void I2CSendACK(PSimuI2C pxsSimuI2C);
extern void I2CSendNACK(PSimuI2C pxsSimuI2C);
extern void I2CStop(PSimuI2C pxsSimuI2C);
extern void I2CStart(PSimuI2C pxsSimuI2C);

#endif  //_I2C_H_
