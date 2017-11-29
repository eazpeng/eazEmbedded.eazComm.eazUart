/*************************************************************************
* Copyright:    EAZ LAB.
* File:         DirtComm.c
* Layer:        Driver.ex
* Author:       eaz
* Version:      0.0.171110
* Log:          eaz.mod.171124
                Allow to receive not only one frame. write RxBuffer and read
                RxBuffer is independent and rotary.
                eaz.mod.171128
                Add head identification.
*************************************************************************/
// common include
#include "system.h"
#include "SimuUart.h"

// comm-send micros
#define TRUE                    (1)
#define FALSE                   (0)

// functions declarition
static void SimuUartTxdClr(void);
static void SimuUartTxdSet(void);
static bit SimuUartRxdValue(void);
static bit FindFrameHead(PSimuUart pxsSimuUart);
static void CopyRxBuff(unsigned char *pucDstBuff, PSimuUart pxsSimuUart);
static unsigned char \
SimuCommCheckSum(const unsigned char Datastr[], const unsigned char LengCnt);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> override area ORG
// modulation include
#include "DirtComm.h"

// ports
#define P_SimuTXD				P1_5            // P_DirtTXD
#define P_SimuRXD				(P1 & (1 << 4)) // P_DirtRXD

// Baud rate counter
// If you need BaudRate = 2400bps, and TimerBase = 139us
// then BaudRateCounter = 1000000 / 2400 bps / 139us = 3.
#define COMM_BAUD_RATE_COUNTER	(4)         // TimerBase = 104us in this case.

// frame related
#define COMM_FRAME_LEN			(5)
#define	COMM_HEAD				(0x55)
#define	COMM_RX_FORMAT_BYTE 	(0xB1)
#define	COMM_CHECK				(0X41)
#define	COMM_SND_ERROR			(0X01)
#define	COMM_SND_NORMAL			(0X00)

// create a simulate uart, [NEED NOT TO CHANGE]
static SimuUart xdata s_xsSimuUart = {
    COMM_FRAME_LEN,
    COMM_BAUD_RATE_COUNTER, // BaudRateCounterBak

    0,      // bRI
	0,		// RxBitPosition
    0x01,   // RxShiftRegister
    0,      // RxBaudRateCounter
    0,      // RxBuffWritePos;
    0,      // RxBuffReadPos;
    {0},    // RxBuffer
    
    0,      // bTI
    0,      // TxPosition
	0,		// TxBitPosition
    0x01,   // TxShiftRegister
    0,      // TxBaudRateCounter
    {0},    // TxBuffer
    
    SimuUartTxdSet,     // TXD_SET
    SimuUartTxdClr,     // TXD_CLR
	SimuUartRxdValue    // RXD_VALUE
};

/**
 * SimuDirtUartInit
 * void, void
 * eaz.org.171123
 * GPIO initialization
**/
void SimuDirtUartInit(void)
{
    static bit s_bSimuDirtUartInitFlag = 0;
    
    if(!s_bSimuDirtUartInitFlag)
    {
        s_bSimuDirtUartInitFlag = 1;
        
        P1CR |= (1 << 5);       // P1_5 as TXD, set as output
        P1PCR &= ~(1 << 5);     // P1_5 disable internal pull-up resistor
        P1 |= (1 << 5);         // P1_5 set high
        
        P1CR &= ~(1 << 4);      // P1_4 as RXD, set as input
        P1PCR |= (1 << 4);      // P1_4 enable internal pull-up resistor
        
        P0CR |= (1 << 1);       // P0_1 as EN, set as output
        P0PCR &= ~(1 << 1);     // P0_1 disable internal pull-up resistor
        P0 |= (1 << 1);         // P0_1 set high
    }
}

/**
 * SimuDirtUartCommProcess
 * void, void
 * eaz.org.171110
 * To send or receive one data pack.
 * An interface to Timer.c in the driver layer,
   this function should call by Timer5 ISR!
**/
void SimuDirtUartCommProcess(void)
{
    SimuUartSend(&s_xsSimuUart);
    SimuUartReceive(&s_xsSimuUart);
}

/**
 * GetDirtValue
 * DIRT_COMM_STATE, unsigned char *
 * eaz.org.171110
 * To pick out and return the dirt value if there're
   datas in the buffer and the checksum is right.
 * An interface to Dirt.c in the application layer.
 * eaz.mod.171124
 * Allow to receive not only one frame.
**/
DIRT_COMM_STATE GetDirtValue(unsigned char *pucValue)
{
    if(FindFrameHead(&s_xsSimuUart))
    {
        unsigned char xdata ucTempRxBuff[COMM_FRAME_LEN];
        
        CopyRxBuff(&ucTempRxBuff, &s_xsSimuUart);
        if(ucTempRxBuff[4] == SimuCommCheckSum(&ucTempRxBuff[1], 3))
        {
            *pucValue = ucTempRxBuff[2];
            
            s_xsSimuUart.ucRxBuffReadPos = (s_xsSimuUart.ucRxBuffReadPos + s_xsSimuUart.ucFrameLen)
                                                % MAX_SIMU_UART_RX_BUFF_LEN;
            
            return DIRT_COMM_REV_OK;
        }
        else
        {
            return DIRT_COMM_REV_CHECK_FAIL;
        }
    }
    else
    {
        return DIRT_COMM_REV_NO_DATA;
    }
}

/**
 * CheckDirtValue
 * DIRT_COMM_STATE, void
 * eaz.org.171110
 * To Send a firmed data pack to the dirt sensor.
 * An interface to Dirt.c in the application layer.
 * This can be modified for further use.
**/
DIRT_COMM_STATE CheckDirtValue(void)
{
	if(!s_xsSimuUart.ucTI)
	{
		s_xsSimuUart.ucTxBuff[0] = COMM_HEAD;
		s_xsSimuUart.ucTxBuff[1] = COMM_RX_FORMAT_BYTE;
		s_xsSimuUart.ucTxBuff[2] = COMM_CHECK;
		s_xsSimuUart.ucTxBuff[3] = 0x00;
		s_xsSimuUart.ucTxBuff[4] \
					 = SimuCommCheckSum(&s_xsSimuUart.ucTxBuff[1], 3);

        s_xsSimuUart.ucTI = 1;
		return DIRT_COMM_SND_OK;
	}
	else
	{
		return DIRT_COMM_SND_BUSY;
	}
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> override area END

/**
 * SimuUartTxdSet
 * void, void
 * eaz.org.171110
 * Set the TTL state of the "TXD" port as H.
 * As a function member of the SimuUart.
**/
static void SimuUartTxdSet(void)
{
    P_SimuTXD = 1;
}

/**
 * SimuUartTxdClr
 * void, void
 * eaz.org.171110
 * Set the TTL state of the "TXD" port as L.
 * As a function member of the SimuUart.
**/
static void SimuUartTxdClr(void)
{
    P_SimuTXD = 0;
}

/**
 * SimuUartRxdValue
 * bit, void
 * eaz.org.171110
 * Get the TTL state of the "RXD" port.
 * As a function member of the SimuUart.
**/
static bit SimuUartRxdValue(void)
{
	return P_SimuRXD;
}

/**
 * SimuUartRxdValue
 * bit, void
 * eaz.org.171127
 * Find the frame head.
**/
static bit FindFrameHead(PSimuUart pxsSimuUart)
{
    while(pxsSimuUart->ucRxBuffReadPos != pxsSimuUart->ucRxBuffWritePos)
    {
        if(pxsSimuUart->ucRxBuff[pxsSimuUart->ucRxBuffReadPos] == COMM_HEAD)
        {
            if(pxsSimuUart->ucRxBuff[pxsSimuUart->ucRxBuffReadPos + 1] == COMM_RX_FORMAT_BYTE)
            {
                return 1;
            }
            else
            {
                pxsSimuUart->ucRxBuffReadPos ++;
            }
        }
        else
        {
            pxsSimuUart->ucRxBuffReadPos ++;
        }
        
        if(pxsSimuUart->ucRxBuffReadPos >= pxsSimuUart->ucFrameLen)
        {
            pxsSimuUart->ucRxBuffReadPos = 0;
        }
    }
    return 0;
}

/**
 * CopyRxBuff
 * void, unsigned char*, PSimuUart
 * eaz.org.171128
 * Maybe for common use.
**/
static void CopyRxBuff(unsigned char *pucDstBuff, PSimuUart pxsSimuUart)
{
    unsigned int xdata uiCounter;
    
    for(uiCounter = 0; uiCounter < pxsSimuUart->ucFrameLen; uiCounter++)
    {
        pucDstBuff[uiCounter] = pxsSimuUart->ucRxBuff[(pxsSimuUart->ucRxBuffReadPos + uiCounter) % MAX_SIMU_UART_RX_BUFF_LEN];
    }
}

/**
 * DirtCommCheckSum
 * unsigned char, const unsigned char*, const unsigned char
 * eaz.org.171110
 * Maybe for common use.
**/
static unsigned char
SimuCommCheckSum(const unsigned char Datastr[], const unsigned char LengCnt)
{
	unsigned char xdata i;
	unsigned char xdata CheckSum ;

	for(i=0, CheckSum = 0; i<LengCnt; i++)
	{
		CheckSum = CheckSum + Datastr[i];
	}

	CheckSum = (CheckSum ^ 0xff) + 1;

	return CheckSum;
}