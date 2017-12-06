/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         Led.c
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171118
* Brief:        for AiP1820 LED
*************************************************************************/
#include "bsp.h"
#include "Led.h"
#include "A31G12x_pcu.h"
#include "A31G12x_series.h"

#define LED_CMD_WR_SINGLE_DATA  	(0x44)
#define LED_CMD_WR_SEQ_DATA     	(0x40)
#define LED_CMD_DISP_MODE_4N13  	(0x00)  // 4 Grids and 13 Segments
#define LED_CMD_DISP_MODE_5N12  	(0x01)  // 5 Grids and 12 Segments
#define LED_CMD_DISP_MODE_6N11  	(0x02)  // 6 Grids and 11 Segments
#define LED_CMD_DISP_MODE_7N10  	(0x03)  // 7 Grids and 10 Segments
#define LED_CMD_DISP_MODE			LED_CMD_DISP_MODE_7N10

#define LED_CMD_DISP_ENABLE			(0x88 | s_ucBrightnessClass)
#define LED_CMD_DISP_DISABLE		(0x80 | s_ucBrightnessClass)

LedMap  g_xsLedMap = {0};
LedMap  g_xsLedMapBlinkFlags = {0};
static bool s_bBlinkActiveFlag = false;
static UInt08 s_ucBrightnessClass = LED_BRIGHTNESS_CLASS_L;

const UInt08 c_ucDigitLedMap[] =
{
    //  AAAAA
    // F     B
    // F     B
    //  GGGGG
    // E     C
    // E     C
    //  DDDDD
            //   XGFE DCBA
	0x00,	// 0b0000 0000, space
	0x3F,	// 0b0011 1111, 0
	0x06,	// 0b0000 0110, 1
	0x5B,	// 0b0101 1011, 2
	0x4F,	// 0b0100 1111, 3
	0x66,	// 0b0110 0110, 4
	0x6D,	// 0b0110 1101, 5
	0x7D,	// 0b0111 1101, 6
    0x27,	// 0b0010 0111, 7
	0x7F,	// 0b0111 1111, 8
	0x6F,	// 0b0110 1111, 9
    0x77,   // 0b0111 0111, A
    0x7F,   // 0b0111 1111, B(8)
    0x39,   // 0b0011 1001, C
    0x3F,   // 0b0011 1111, D(0)
    0x79,   // 0b0111 1001, E
    0x71,   // 0b0111 0001, F
    0x3D,   // 0b0011 1101, G
    0x76,   // 0b0111 0110, H
    0x06,   // 0b0000 0110, I(1)
    0x1E,   // 0b0001 1110, J
    0x75,   // 0b0111 0101, K
    0x38,   // 0b0011 1000, L
    0x37,   // 0b0011 0111, M(N)
    0x37,   // 0b0011 0111, N
    0x3F,   // 0b0011 1111, O(0)
    0x73,   // 0b0111 0011, P
    0x3F,	// 0b0011 1111, Q(0)
    0x33,   // 0b0011 0011, R
    0x6D,	// 0b0110 1101, S(5)
    0x07,	// 0b0000 0111, T
    0x3E,	// 0b0011 1110, U
    0x3E,	// 0b0011 1110, V(U)
    0x3E,	// 0b0011 1110, W(U)
    0x76,   // 0b0111 0110, X(H)
    0x66,	// 0b0110 0110, Y(4)
    0x5B,	// 0b0101 1011, Z(2)
    0x5C,   // 0b0101 1100, a(o)
    0x7C,   // 0b0111 1100, b
    0x58,   // 0b0101 1000, c
    0x5E,   // 0b0101 1110, d
    0x7B,   // 0b0111 1011, e
    0x71,   // 0b0111 0001, f(F)
    0x6F,	// 0b0110 1111, g(9)
    0x74,   // 0b0111 0100, h
    0x04,   // 0b0000 0100, i
    0x1E,   // 0b0001 1110, j(J)
    0x75,   // 0b0111 0101, k(K)
    0x38,   // 0b0011 1000, l(L)
    0x54,   // 0b0101 0100, m(n)
    0x54,   // 0b0101 0100, n
    0x5C,   // 0b0101 1100, o
    0x73,   // 0b0111 0011, p(P)
    0x67,   // 0b0110 0111, q
    0x50,   // 0b0101 0000, r
    0x6D,	// 0b0110 1101, s(5)
    0x78,	// 0b0111 1000, t
    0x1C,	// 0b0001 1100, u
    0x1C,	// 0b0001 1100, v(u)
    0x1C,	// 0b0001 1100, w(u)
    0x76,   // 0b0111 0110, x(H)
    0x6E,	// 0b0110 1110, y
    0x5B,	// 0b0101 1011, z(2)
    0x40,   // 0b0100 0000, -
};

/**
 * GetDigitValueOfChar
 * UInt08, UInt08
 * eaz.org.171121
 *
**/
UInt08 GetDigitValueOfChar(UInt08 ucChar)
{
    if('0' <= ucChar && ucChar <= '9')
    {
        ucChar = ucChar + 1 - '0';
    }
    else if('A' <= ucChar && ucChar <= 'Z')
    {
        ucChar = ucChar + 1 + 10 - 'A';
    }
    else if('a' <= ucChar && ucChar <= 'z')
    {
        ucChar = ucChar + 1 + 10 + 26 - 'a';
    }
    else if(ucChar == '-')
    {
        ucChar = 1 + 10 + 26 + 26;
    }
    else
    {
        ucChar = 0;    // space
    }

    return c_ucDigitLedMap[ucChar];
}

/**
 * LED_STB_L
 * void, UInt08
 * eaz.org.171130
 * Chip select.
**/
static void LED_STB_L(UInt08 ucChipIndex)
{
	switch(ucChipIndex)
	{
		case 1:
		{
			P_LED_STB1_L;
		}break;
		
		case 2:
		{
			P_LED_STB2_L;
		}break;
		
		case 3:
		{
			P_LED_STB3_L;
		}break;
		
		case 4:
		{
			P_LED_STB4_L;
		}break;
		
		case 5:
		{
			P_LED_STB5_L;
		}break;
		
		default:
			break;
	}
}

/**
 * LED_STB_H
 * void, UInt08
 * eaz.org.171130
 * Chip not-select.
**/
static void LED_STB_H(UInt08 ucChipIndex)
{
	switch(ucChipIndex)
	{
		case 1:
		{
			P_LED_STB1_H;
		}break;
		
		case 2:
		{
			P_LED_STB2_H;
		}break;
		
		case 3:
		{
			P_LED_STB3_H;
		}break;
		
		case 4:
		{
			P_LED_STB4_H;
		}break;
		
		case 5:
		{
			P_LED_STB5_H;
		}break;
		
		default:
			break;
	}
}

/**
 * LedCommDelay
 * void, void
 * eaz.org.171130
**/
static void LedCommDelay(void)
{
	NOP();
}

/**
 * SendByteToLedChip
 * UInt08, UInt08, Bool
 * eaz.org.171130
 * Send one byte to AiP1822, LSB.
 * !!! This part can be interrupted by ISR.
**/
static void SendByteToLedChip(UInt08 ucChipIndex, UInt08 ucData, bool bContinueTransmitFlag)
{
    UInt08 ucCounter;
    UInt08 ucTemp = 0x01;

    LED_STB_L(ucChipIndex);
    LedCommDelay();

    for(ucCounter = 0; ucCounter < 8; ucCounter++)
    {
        P_LED_CLK_L;
        LedCommDelay();

        if(ucData & ucTemp)
        {
            P_LED_DIO_H;
        }
        else
        {
            P_LED_DIO_L;
        }

        LedCommDelay();
        P_LED_CLK_H;
        LedCommDelay();
        ucTemp <<= 1;
    }
	
    if(!bContinueTransmitFlag)
	{
        LED_STB_H(ucChipIndex);
	}
}

/**
 * SetLedChipsDispMode
 * void, void
 * eaz.org.171130
 * Power on setting once, refers to micro: LED_CMD_DISP_MODE
**/
static void SetLedChipsDispMode(void)
{
    SendByteToLedChip(1, LED_CMD_DISP_MODE, false);
    SendByteToLedChip(2, LED_CMD_DISP_MODE, false);
    SendByteToLedChip(3, LED_CMD_DISP_MODE, false);
    SendByteToLedChip(4, LED_CMD_DISP_MODE, false);
    SendByteToLedChip(5, LED_CMD_DISP_MODE, false);
}

/**
 * SendDataToLedChip
 * void, UInt08, UInt08, UInt08
 * eaz.org.171130
 * Include address-setting and one-byte send.
**/
static void SendDataToLedChip(UInt08 ucChipIndex, UInt08 ucAddrIndex, UInt08 ucSendData)
{
    SendByteToLedChip(ucChipIndex, LED_CMD_WR_SINGLE_DATA, false);
    SendByteToLedChip(ucChipIndex, (0xC0 | ucAddrIndex), true);
    SendByteToLedChip(ucChipIndex, ucSendData, false);
}

/**
 * LedSendDataSequencely
 * void, UInt08, UInt08, UInt08 *, UInt08
 * eaz.org.171130
 * Include address-setting and ucLen-bytes send.
**/
static void SendDatasToLedChipSequencely(UInt08 ucChipIndex, UInt08 ucStartAddrIndex, UInt08 *pucSendData, UInt08 ucLen)
{
    UInt08 ucCounter;

    SendByteToLedChip(ucChipIndex, LED_CMD_WR_SEQ_DATA, false);
    SendByteToLedChip(ucChipIndex, (0xC0 | ucStartAddrIndex), true);

    for(ucCounter = 0; ucCounter <= ucLen - 1; ucCounter++)
    {
        SendByteToLedChip(ucChipIndex, pucSendData[ucCounter], true);
    }
    SendByteToLedChip(ucChipIndex, pucSendData[ucCounter], false);
}

/**
 * SetAllLedSegmentsOn
 * void, void
 * eaz.org.171130
 * Light on all Segments AND not blink.
**/
void SetAllLedSegmentsOn(void)
{
    UInt08 ucCounter;

    for(ucCounter = 0; ucCounter < 7; ucCounter++)
    {
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.ucDatas[ucCounter] = 0xFF;

        g_xsLedMap.AllSegs.xsChips.U1.ucDatas[ucCounter] = 0xFF;
        g_xsLedMap.AllSegs.xsChips.U2.ucDatas[ucCounter] = 0xFF;
        g_xsLedMap.AllSegs.xsChips.U3.ucDatas[ucCounter] = 0xFF;
        g_xsLedMap.AllSegs.xsChips.U4.ucDatas[ucCounter] = 0xFF;
        g_xsLedMap.AllSegs.xsChips.U5.ucDatas[ucCounter] = 0xFF;
    }
	
    LedChipsDataRefresh();
}

/**
 * SetAllLedSegmentsOff
 * void, void
 * eaz.org.171130
 * Light off all Segments AND not blink.
**/
void SetAllLedSegmentsOff(void)
{
    UInt08 ucCounter;

    for(ucCounter = 0; ucCounter < 7; ucCounter++)
    {
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.ucDatas[ucCounter] = 0xFF;
        g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.ucDatas[ucCounter] = 0xFF;

        g_xsLedMap.AllSegs.xsChips.U1.ucDatas[ucCounter] = 0;
        g_xsLedMap.AllSegs.xsChips.U2.ucDatas[ucCounter] = 0;
        g_xsLedMap.AllSegs.xsChips.U3.ucDatas[ucCounter] = 0;
        g_xsLedMap.AllSegs.xsChips.U4.ucDatas[ucCounter] = 0;
        g_xsLedMap.AllSegs.xsChips.U5.ucDatas[ucCounter] = 0;
    }
	
    LedChipsDataRefresh();
}

/**
 * LedChipsInit
 * void, void
 * eaz.org.171130
 * Set display mode, enable display.
 * Power on 3s all-on.
**/
void LedChipsInit(void)
{
    // set ports properties
    s_bBlinkActiveFlag = false;
    SetLedChipsDispMode();
    SetAllLedSegmentsOn();
    SetLedBrightness(LED_BRIGHTNESS_CLASS_H);
}

/**
 * SetLedBrightness
 * void, UInt08
 * eaz.org.171130
**/
void SetLedBrightness(UInt08 ucBrightnessClass)
{
    s_ucBrightnessClass = ucBrightnessClass;

    SendByteToLedChip(1, LED_CMD_DISP_ENABLE, false);
    SendByteToLedChip(2, LED_CMD_DISP_ENABLE, false);
    SendByteToLedChip(3, LED_CMD_DISP_ENABLE, false);
    SendByteToLedChip(4, LED_CMD_DISP_ENABLE, false);
    SendByteToLedChip(5, LED_CMD_DISP_ENABLE, false);
}

/**
 * SwitchLedBlinkState
 * void, void
 * eaz.org.171130
**/
void SwitchLedBlinkState(void)
{
    s_bBlinkActiveFlag = 1 - s_bBlinkActiveFlag;
}

/**
 * LedChipsDataRefresh
 * void, void
 * eaz.org.171130
 * Send display buffer datas to LED chips.
**/
void LedChipsDataRefresh(void)
{
    UInt08 ucChipIndex, ucAddrIndex;
    UInt08 ucSendData;
	
    for(ucChipIndex = 0; ucChipIndex < 5; ucChipIndex ++)
    {
        for(ucAddrIndex = 0; ucAddrIndex < 7; ucAddrIndex ++)
        {
            if(s_bBlinkActiveFlag
               && (g_xsLedMapBlinkFlags.AllSegs.ucDatas[ucChipIndex][ucAddrIndex] < 0xFF))
            {
                ucSendData = g_xsLedMapBlinkFlags.AllSegs.ucDatas[ucChipIndex][ucAddrIndex]
                           & g_xsLedMap.AllSegs.ucDatas[ucChipIndex][ucAddrIndex];
            }
            else
            {
                ucSendData = g_xsLedMap.AllSegs.ucDatas[ucChipIndex][ucAddrIndex];
            }
			
            SendDataToLedChip((ucChipIndex + 1), ucAddrIndex << 1, ucSendData);
        }
    }
}

