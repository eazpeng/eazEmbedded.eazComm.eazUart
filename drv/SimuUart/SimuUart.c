/*************************************************************************
* Copyright:    EAZ LAB.
* File:         SimuUart.c
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171110
* Log:          eaz.mod.171124
                Allow to receive not only one frame. write RxBuffer and read
                RxBuffer is independent and rotary.
*************************************************************************/
// modulation include
#include "SimuUart.h"

// common micros
#define TRUE    (1)
#define FALSE   (0)

// common enums
typedef enum
{
	SMUART_NULL_BIT,
    SMUART_START_BIT,
	SMUART_DATA_BIT0,
	SMUART_DATA_BIT1,
	SMUART_DATA_BIT2,
	SMUART_DATA_BIT3,
	SMUART_DATA_BIT4,
	SMUART_DATA_BIT5,
	SMUART_DATA_BIT6,
	SMUART_DATA_BIT7,
    SMUART_STOP_BIT,
    SMUART_BYTE_END
}SIMU_UART_BITS;

typedef enum
{
    SMUART_NULL,
    SMUART_SND_OK,
    SMUART_SND_BUSY,
    SMUART_SND_FAILED,
    SMUART_REV_OK,
    SMUART_REV_BUSY,
    SMUART_REV_FAILED
}SIMU_UART_COMM_STATE;

/**
 * SimuUartSendByte
 * SIMU_UART_COMM_STATE, PSimuUart
 * eaz.org.171110
 * Send one byte in the RXD buffer, called by SimuUartSend function.
**/
static SIMU_UART_COMM_STATE SimuUartSendByte(PSimuUart pxsSimuUart)
{
    if(pxsSimuUart->ucTxBaudRateCounter == 0)
    {
        switch(++pxsSimuUart->ucTxBitPos)
        {
            case SMUART_START_BIT:
            {
                pxsSimuUart->TXD_CLR();
                // eaz.171111 timer5 with low precision, fix a little
                pxsSimuUart->ucTxBaudRateCounter ++;
            }break;
            
            default:    // data bits
            {
                if((pxsSimuUart->ucTxBuff[pxsSimuUart->ucTxPos]
					 & pxsSimuUart->ucTxShiftRegister) > 0)
                {
                    pxsSimuUart->TXD_SET();
                }
                else
                {
                    pxsSimuUart->TXD_CLR();
                }
                
                // LSB
                pxsSimuUart->ucTxShiftRegister <<= 1;
            }break;
            
            case SMUART_STOP_BIT:
            {
                pxsSimuUart->TXD_SET();
            }break;
            
            case SMUART_BYTE_END:
            {
                return SMUART_SND_OK;
            }
        }
    }
    
    if(++pxsSimuUart->ucTxBaudRateCounter >= pxsSimuUart->ucBaudRateCounterBak)
    {
        pxsSimuUart->ucTxBaudRateCounter = 0;
    }
    
    return SMUART_SND_BUSY;
}

/**
 * SimuUartSend
 * void, PSimuUart
 * eaz.org.171110
 * Send one data pack in the RXD buffer.
 * As an interface to the expand-driver files for further use.
**/
void SimuUartSend(PSimuUart pxsSimuUart)
{
    if(pxsSimuUart->ucTI)
    {
        if(pxsSimuUart->ucTxPos < pxsSimuUart->ucFrameLen)
        {
            if(SMUART_SND_OK == SimuUartSendByte(pxsSimuUart))
            {
                pxsSimuUart->ucTxPos ++;
                pxsSimuUart->ucTxBitPos = 0;
                pxsSimuUart->ucTxShiftRegister = 0x01;
            }
        }
        else
        {
            pxsSimuUart->ucTxPos = 0;
            pxsSimuUart->ucTxBitPos = 0;
            pxsSimuUart->ucTxShiftRegister = 0x01;
            pxsSimuUart->TXD_SET();
            pxsSimuUart->ucTI = FALSE;
        }
    }
}

/**
 * SimuUartReceive
 * void, PSimuUart
 * eaz.org.171110
 * As an interface to the expand-driver files for further use.
 * TODO.eaz: need to cut up.
**/
void SimuUartReceive(PSimuUart pxsSimuUart)
{
	switch(pxsSimuUart->ucRxBitPos)
    {
		case SMUART_NULL:
		{
			if(!pxsSimuUart->RXD_VALUE())
            {
                pxsSimuUart->ucRI = FALSE;              // receiving
                
				pxsSimuUart->ucRxBitPos++;
                pxsSimuUart->ucRxBaudRateCounter++;
            }
		}break;

        case SMUART_START_BIT:
		{
			pxsSimuUart->ucRxBaudRateCounter++;

            if(!pxsSimuUart->RXD_VALUE())
            {
                pxsSimuUart->ucRxBaudRateCounter += 0x10;
            }

            if((pxsSimuUart->ucRxBaudRateCounter & 0x0f) >= pxsSimuUart->ucBaudRateCounterBak)
            {
				// read the mid-value
				unsigned char ucTempFlag;
				ucTempFlag = ((pxsSimuUart->ucBaudRateCounterBak - 2) << 4) \
									+ pxsSimuUart->ucBaudRateCounterBak;

                if(pxsSimuUart->ucRxBaudRateCounter >= ucTempFlag)
                {
                    pxsSimuUart->ucRxBitPos++;
                }
				else
				{
					pxsSimuUart->ucRxBitPos = SMUART_NULL;
				}

                pxsSimuUart->ucRxBaudRateCounter = 0;
            }
		}break;

        case SMUART_DATA_BIT0:
		{
			pxsSimuUart->ucRxBaudRateCounter++;

            if(pxsSimuUart->ucRxBaudRateCounter >= pxsSimuUart->ucBaudRateCounterBak - 1)
            {
                pxsSimuUart->ucRxShiftRegister >>= 1;

                if(pxsSimuUart->RXD_VALUE())
                {
                    pxsSimuUart->ucRxShiftRegister |= 0x80;
                }
                else
                {
                    pxsSimuUart->ucRxShiftRegister &= 0x7f;
                }

                pxsSimuUart->ucRxBitPos++;
                pxsSimuUart->ucRxBaudRateCounter = 0;
            }
		}break;
        
        case SMUART_DATA_BIT1:
        case SMUART_DATA_BIT2:
        case SMUART_DATA_BIT3:
        case SMUART_DATA_BIT4:
        case SMUART_DATA_BIT5:
        case SMUART_DATA_BIT6:
        case SMUART_DATA_BIT7:
		{
			pxsSimuUart->ucRxBaudRateCounter++;

            if(pxsSimuUart->ucRxBaudRateCounter >= pxsSimuUart->ucBaudRateCounterBak)
            {
                pxsSimuUart->ucRxShiftRegister >>= 1;
                if(pxsSimuUart->RXD_VALUE())
                {
                    pxsSimuUart->ucRxShiftRegister |= 0x80;
                }
                else
                {
                    pxsSimuUart->ucRxShiftRegister &= 0x7f;
                }

                pxsSimuUart->ucRxBitPos++;
                pxsSimuUart->ucRxBaudRateCounter = 0;
            }
		}break;
        
        default:
		{
			pxsSimuUart->ucRxBaudRateCounter++;

            if(pxsSimuUart->ucRxBaudRateCounter == pxsSimuUart->ucBaudRateCounterBak)
            {
                if(pxsSimuUart->RXD_VALUE())
                {
                    if((pxsSimuUart->ucRxBuffWritePos + pxsSimuUart->ucFrameLen)
                            > MAX_SIMU_UART_RX_BUFF_LEN)
                    {
                        pxsSimuUart->ucRxBuffWritePos = 0;  // Align
                    }
                    
                    pxsSimuUart->ucRxBuff[pxsSimuUart->ucRxBuffWritePos] = pxsSimuUart->ucRxShiftRegister;
                    
                    if(!((++pxsSimuUart->ucRxBuffWritePos) % pxsSimuUart->ucFrameLen))
                    {
                        pxsSimuUart->ucRI = TRUE;
                    }
                    
                    pxsSimuUart->ucRxBitPos = 0;
                    pxsSimuUart->ucRxBaudRateCounter = 0;
                }
            }
            else if(pxsSimuUart->ucRxBaudRateCounter >= (pxsSimuUart->ucBaudRateCounterBak << 1))
            {
                if(!pxsSimuUart->RXD_VALUE())
                {
                    // always low
                }
                
                if((pxsSimuUart->ucRxBuffWritePos + pxsSimuUart->ucFrameLen)
                        > MAX_SIMU_UART_RX_BUFF_LEN)
                {
                    pxsSimuUart->ucRxBuffWritePos = 0;  // Align
                }

                pxsSimuUart->ucRxBuff[pxsSimuUart->ucRxBuffWritePos] = pxsSimuUart->ucRxShiftRegister;
                    
                if(!((++pxsSimuUart->ucRxBuffWritePos) % pxsSimuUart->ucFrameLen))
                {
                    pxsSimuUart->ucRI = TRUE;
                }
                
                pxsSimuUart->ucRxBitPos = 0;
                pxsSimuUart->ucRxBaudRateCounter = 0;
            }
		}break;     
    }
}