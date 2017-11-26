/*************************************************************************
* Copyright:    EAZ LAB.
* File:         SimuUart.h
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171110
* Log:          eaz.mod.171124
                Allow to receive not only one frame. write RxBuffer and read
                RxBuffer is independent and rotary.
*************************************************************************/
#ifndef	_SIMU_UART_H_
#define	_SIMU_UART_H_

// micros
#define MAX_SIMU_UART_RX_BUFF_LEN   (16)
#define MAX_SIMU_UART_TX_BUFF_LEN   (16)

// the simulate uart structure
typedef struct tagSimuUart
{
	unsigned char	ucHead;
	unsigned char	ucFrameLen;
    unsigned char   ucBaudRateCounterBak;
    
    // Rx
    unsigned char   ucRI;
	unsigned char	ucRxBitPos;
    unsigned char   ucRxShiftRegister;
    unsigned char   ucRxBaudRateCounter;
    unsigned char   ucRxBuffWritePos;
    unsigned char   ucRxBuffReadPos;
    unsigned char   ucRxBuff[MAX_SIMU_UART_RX_BUFF_LEN];
    
    // Tx
    unsigned char   ucTI;
    unsigned char   ucTxPos;
    unsigned char	ucTxBitPos;
    unsigned char   ucTxShiftRegister;
    unsigned char   ucTxBaudRateCounter;
    unsigned char   ucTxBuff[MAX_SIMU_UART_TX_BUFF_LEN];
    
    // functions
    void (*TXD_SET)(void);
    void (*TXD_CLR)(void);
    bit  (*RXD_VALUE)(void);
}SimuUart, *PSimuUart;

// interface
extern void SimuUartSend(PSimuUart pxsSimuUart);
extern void SimuUartReceive(PSimuUart pxsSimuUart);

#endif	//_SIMU_UART_H_