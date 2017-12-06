/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         Display.c
* Layer:        App.base
* Author:       eaz
* Version:      0.0.171120
* Brief:        Display characters on the position, animation controls
*               inside module.
*               STR_ID->CHAR_MAP->Modify the led buffer.
*************************************************************************/
#include "Led.h"
#include "Timer.h"
#include "Display.h"

typedef struct tagString
{
    DISP_STR        eStrId;
    UInt08          pucStr[OBM_STR_MAX_LEN];
}String, *PString;

typedef struct tagScroller
{
    DISP_POSITION   eDispPos;
    DISP_STR        eStrId;
    UInt08          ucCurStep;
    UInt08          ucCurHeadCharPos;
    bool            bScrollerActiveFlag;
}Scroller, *PScroller;

const String c_xsStrings[DISP_STR_ID_SUM] = // string map, index equals id
{
    {
        DISP_STR_NULL,
        "___"
    },
    {
        DISP_STR_OFF,
        "OFF"
    },
    {
        DISP_STR_ON,
        "ON"
    },
    {
        DISP_STR_NO,
        "No"
    },
    {
        DISP_STR_YES,
        "yES"
    },
    {
        DISP_STR_HIGH,
        "HI"
    },
    {
        DISP_STR_MEDIUM,
        "M7ed"
    },
    {
        DISP_STR_LOW,
        "LO"
    },
    {
        DISP_STR_LOCK,
        "LOCK"
    },
    {
        DISP_STR_PREHEAT,
        "PrE"
    },
    {
        DISP_STR_SERVICE,
        "SrVC"
    },
    {
        DISP_STR_TURN_OFF_COOKTOP,
        "turn oFF cooKtoP"
    },
    {
        DISP_STR_CLOSE_DOOR,
        "cLosE door"
    },
    {
        DISP_STR_TEST,
        "tESt"
    },
    {
        DISP_STR_ERR1,
        "Err1"
    },
    {
        DISP_STR_ERR2,
        "Err2"
    },
    {
        DISP_STR_ERR3,
        "Err3"
    },
    {
        DISP_STR_ERR4,
        "Err4"
    },
    {
        DISP_STR_ERR5,
        "Err5"
    },
    {
        DISP_STR_ERR6,
        "Err6"
    },
    {
        DISP_STR_ERR7,
        "Err7"
    },
    {
        DISP_STR_ERR8,
        "Err8"
    },
    {
        DISP_STR_CLOCK_TYPE,
        "CLOCK tyPE"
    },
    {
        DISP_STR_12HR,
        "12hr"
    },
    {
        DISP_STR_24HR,
        "24hr"
    },
    {
        DISP_STR_CLOCK_SLEEP,
        "CLOCK SLEEP"
    },
    {
        DISP_STR_VOLUME,
        "VOLUME"
    },
    {
        DISP_STR_BRIGHTNESS,
        "BrIGHtNESS"
    },
    {
        DISP_STR_WAKE_UP_LIGHT,
        "WJAKE UP LIGHt"
    },
    {
        DISP_STR_WAKE_UP_MODE,
        "WJAKE UP M7odE"
    },
    {
        DISP_STR_ONE,
        "ONE"
    },
    {
        DISP_STR_ANY,
        "ANy"
    },
    {
        DISP_STR_UPPER_OFFSET,
        "UPPEr OFFSEt"
    },
    {
        DISP_STR_LOWER_OFFSET,
        "LOWJr OFFSEt"
    },
    {
        DISP_STR_TEMPERATURE_UNITS,
        "tEM7PUnitS"
    },
    {
        DISP_STR_UNIT_F,
        "F"
    },
    {
        DISP_STR_UNIT_C,
        "c"
    },
    {
        DISP_STR_BAKE_STOP,
        "bAKE Stop"
    },
    {
        DISP_STR_BROIL_STOP,
        "brOIL Stop"
    },
    {
        DISP_STR_SABBATH_MODE,
        "SAbbAth"
    },
    {
        DISP_STR_COOK_THEN_HOLD_MODE,
        "HOLd Food"
    },
    {
        DISP_STR_DEMO_MODE,
        "dEM7O"
    },
};
static Scroller s_xsScroller[3] = {{DISP_POS_MAIN_TIME}, {DISP_POS_UPPER_TIME}, {DISP_POS_LOWER_TIME}};

/**
 * Number2Chars
 * void, UInt16, UInt08 *, HEAD_ZERO_STATE
 * eaz.org.171121
 * c_ucDispCharMap 0x20, 0-9, A-Z, a-z, -
**/
static void Number2Chars(Int16 iValue, UInt08 *pucChars, HEAD_ZERO_STATE eHeadZeroState)
{
    UInt08 pucValue[4];
    UInt16  uiValue = iValue < 0 ? -iValue : iValue;

    pucValue[3] = uiValue % 10;
    pucValue[2] = uiValue % 100 / 10;
    pucValue[1] = uiValue % 1000 / 100;
    pucValue[0] = uiValue % 10000 / 1000;

    pucChars[0] = pucValue[0] > 9 ? 0 : pucValue[0] + 0x30;
    pucChars[1] = pucValue[1] > 9 ? 0 : pucValue[1] + 0x30;
    pucChars[2] = pucValue[2] > 9 ? 0 : pucValue[2] + 0x30;
    pucChars[3] = pucValue[3] > 9 ? 0 : pucValue[3] + 0x30;

    if(eHeadZeroState == HEAD_ZERO_STATE_HIDE)
    {
        if(pucValue[0] == 0)
        {
            pucChars[0] = 0;
            if(pucValue[1] == 0)
            {
                pucChars[1] = 0;
                if(pucValue[2] == 0)
                {
                    pucChars[2] = 0;
					if(pucChars[3] == 0)
					{
						pucChars[3] = '0';
					}
                    else if(iValue < 0)
                    {
                        pucChars[2] = '-';
                    }
                }
                else if(iValue < 0)
                {
                    pucChars[1] = '-';
                }
            }
            else if(iValue < 0)
            {
                pucChars[0] = '-';
            }
        }
    }
}

/**
 * CopySubString
 * void, UInt08 *, UInt08 *, UInt08, UInt08
 * eaz.org.171121
**/
static void CopySubString(UInt08 *pucDst, UInt08 *pucSrc, UInt08 ucPos, UInt08 ucLen)
{
    UInt08 i;

    for(i=0; i<ucLen; i++)
    {
		if(ucPos + ucLen < OBM_STR_MAX_LEN)
		{
			pucDst[i] = pucSrc[ucPos + i];
		}
        else
		{
			pucDst[i] = '\0';
		}
    }
}

/**
 * UpdateStringValueToLedBuff
 * void, UInt08 *, DISP_POSITION
 * eaz.org.171121
**/
static void UpdateStringValueToLedBuff(UInt08 *pucCurChars, DISP_POSITION eDispPos)
{
    switch(eDispPos)
    {
        case DISP_POS_UPPER_TIME:
        {
            LED_UP_TIME_VALUE_POS1 = GetDigitValueOfChar(pucCurChars[0]);
            LED_UP_TIME_VALUE_POS2 = GetDigitValueOfChar(pucCurChars[1]);
            LED_UP_TIME_VALUE_POS3 = GetDigitValueOfChar(pucCurChars[2]);
            LED_UP_TIME_VALUE_POS4 = GetDigitValueOfChar(pucCurChars[3]);
        }break;

        case DISP_POS_MAIN_TIME:
        {
            LED_CLOCK_VALUE_POS1 = GetDigitValueOfChar(pucCurChars[0]);
            LED_CLOCK_VALUE_POS2 = GetDigitValueOfChar(pucCurChars[1]);
            LED_CLOCK_VALUE_POS3 = GetDigitValueOfChar(pucCurChars[2]);
            LED_CLOCK_VALUE_POS4 = GetDigitValueOfChar(pucCurChars[3]);
        }break;

        case DISP_POS_LOWER_TIME:
        {
            LED_LOW_TIME_VALUE_POS1 = GetDigitValueOfChar(pucCurChars[0]);
            LED_LOW_TIME_VALUE_POS2 = GetDigitValueOfChar(pucCurChars[1]);
            LED_LOW_TIME_VALUE_POS3 = GetDigitValueOfChar(pucCurChars[2]);
            LED_LOW_TIME_VALUE_POS4 = GetDigitValueOfChar(pucCurChars[3]);
        }break;

        case DISP_POS_UPPER_TEMP:
        {
            LED_UP_TEMP_VALUE_POS1 = GetDigitValueOfChar(pucCurChars[1]);
            LED_UP_TEMP_VALUE_POS2 = GetDigitValueOfChar(pucCurChars[2]);
            LED_UP_TEMP_VALUE_POS3 = GetDigitValueOfChar(pucCurChars[3]);
        }break;

        case DISP_POS_LOWER_TEMP:
        {
            LED_LOW_TEMP_VALUE_POS1 = GetDigitValueOfChar(pucCurChars[1]);
            LED_LOW_TEMP_VALUE_POS2 = GetDigitValueOfChar(pucCurChars[2]);
            LED_LOW_TEMP_VALUE_POS3 = GetDigitValueOfChar(pucCurChars[3]);
        }break;

        default:
        {
            ;
        }break;
    }
}

/**
 * DispScrollerStop
 * void, UInt08
 * eaz.org.171121
**/
static void DispScrollerStop(UInt08 ucScrollerIndex)
{
    UInt08 pucCurChars[4];

    // show current string's first four chars continuously
    CopySubString((UInt08 *)pucCurChars, \
                  (UInt08 *)c_xsStrings[s_xsScroller[ucScrollerIndex].eStrId].pucStr, 0, 4);
    UpdateStringValueToLedBuff(pucCurChars, s_xsScroller[ucScrollerIndex].eDispPos);

    s_xsScroller[ucScrollerIndex].eStrId = DISP_STR_NULL;
    s_xsScroller[ucScrollerIndex].ucCurStep = 0;
    s_xsScroller[ucScrollerIndex].ucCurHeadCharPos = 0;
    s_xsScroller[ucScrollerIndex].bScrollerActiveFlag = false;

    // if there's also a scroller actived, donot stop a timer
    if((!s_xsScroller[0].bScrollerActiveFlag)
        && (!s_xsScroller[1].bScrollerActiveFlag)
        && (!s_xsScroller[2].bScrollerActiveFlag))
    {
        StopTimer(TIMER_DISP_SCROLL);
    }
}

/**
 * DispScrollerStart
 * void, DISP_POSITION, DISP_STR
 * eaz.org.171121
 * 1. if a scroller is already actived, and a new disp task comes to this one, do not need to stop;
 * 2. if a scroller is already actived, and a new disp task comes to other one, do not need to stop this.
**/
static void DispScrollerStart(DISP_POSITION eDispPos, DISP_STR eStrId)
{
    UInt08 pucCurChars[4];
    UInt08 ucScrollerIndex;

    if(s_xsScroller[0].eDispPos == eDispPos)
    {
        ucScrollerIndex = 0;
    }
    else if(s_xsScroller[1].eDispPos == eDispPos)
    {
        ucScrollerIndex = 1;
    }
    else
    {
        ucScrollerIndex = 2;
    }

    // show the first four spaces
    UpdateStringValueToLedBuff(pucCurChars, eDispPos);

    // if there's already a scroller actived, donot need to start a timer
    if(!s_xsScroller[0].bScrollerActiveFlag
        && !s_xsScroller[1].bScrollerActiveFlag
        && !s_xsScroller[2].bScrollerActiveFlag)
    {
        StartTimer(TIMER_DISP_SCROLL, 300);
    }
	
    s_xsScroller[ucScrollerIndex].eStrId = eStrId;
    s_xsScroller[ucScrollerIndex].ucCurStep = 1;
    s_xsScroller[ucScrollerIndex].ucCurHeadCharPos = 0;
    s_xsScroller[ucScrollerIndex].bScrollerActiveFlag = true;
}

/**
 * DispScrollOneCharacter
 * void, void
 * eaz.org.171121
**/
static void DispScrollOneCharacter(void)
{
    UInt08 ucCounter;

    for(ucCounter=0; ucCounter < 3; ucCounter++)
    {
        UInt08 pucCurChars[4] = {0xFF};

        if(!s_xsScroller[ucCounter].bScrollerActiveFlag)
        {
            continue;
        }

        // get value
        CopySubString(&pucCurChars[4 - s_xsScroller[ucCounter].ucCurStep],
                       (UInt08 *)c_xsStrings[s_xsScroller[ucCounter].eStrId].pucStr,
                       s_xsScroller[ucCounter].ucCurHeadCharPos,
                       s_xsScroller[ucCounter].ucCurStep);
        if(pucCurChars[0] != '\0')
        {
            s_xsScroller[ucCounter].ucCurStep
                    = s_xsScroller[ucCounter].ucCurStep >= 4
                        ? 4 : s_xsScroller[ucCounter].ucCurStep + 1;
            s_xsScroller[ucCounter].ucCurHeadCharPos
                    = s_xsScroller[ucCounter].ucCurStep < 4
                        ? 0 : s_xsScroller[ucCounter].ucCurHeadCharPos + 1;
        }
        else
        {
			if(s_xsScroller[ucCounter].ucCurStep == 4)
			{
				s_xsScroller[ucCounter].ucCurStep ++;
			}
			else
			{
                DispScrollerStop(ucCounter);
				continue;
			}
        }

        UpdateStringValueToLedBuff(&pucCurChars[0], s_xsScroller[ucCounter].eDispPos);
    }
}

/**
 * DispBlinkSet
 * void, DISP_POSITION
 * eaz.org.171121
**/
static void DispBlinkSet(DISP_POSITION eDispPos, BLINK_STATE eBlinkState)
{
    switch(eDispPos)
    {
        case DISP_POS_MAIN_TIME:
        {
            LED_CLOCK_VALUE_POS1_BLINK_FLAG = eBlinkState;
            LED_CLOCK_VALUE_POS2_BLINK_FLAG = eBlinkState;
            LED_CLOCK_VALUE_POS3_BLINK_FLAG = eBlinkState;
            LED_CLOCK_VALUE_POS4_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME:
        {
            LED_UP_TIME_VALUE_POS1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_VALUE_POS2_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_VALUE_POS3_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_VALUE_POS4_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME:
        {
            LED_LOW_TIME_VALUE_POS1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_VALUE_POS2_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_VALUE_POS3_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_VALUE_POS4_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TEMP:
        {
            LED_UP_TEMP_VALUE_POS1_BLINK_FLAG = eBlinkState;
            LED_UP_TEMP_VALUE_POS2_BLINK_FLAG = eBlinkState;
            LED_UP_TEMP_VALUE_POS3_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TEMP:
        {
            LED_LOW_TEMP_VALUE_POS1_BLINK_FLAG = eBlinkState;
            LED_LOW_TEMP_VALUE_POS2_BLINK_FLAG = eBlinkState;
            LED_LOW_TEMP_VALUE_POS3_BLINK_FLAG = eBlinkState;
        }break;

        // Main Icons
        case DISP_POS_MAIN_LOCK_ICON:
        {
            LED_LOCK_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOCK_ICON2_BLINK_FLAG = eBlinkState;
        }break;
		
        case DISP_POS_MAIN_TIME_ICON_DP:
        {
            LED_CLOCK_DP1_BLINK_FLAG = eBlinkState;
            LED_CLOCK_DP2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_MAIN_TIME_ICON_AM:
        {
            LED_CLOCK_FORMAT_AM_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_MAIN_TIME_ICON_PM:
        {
            LED_CLOCK_FORMAT_PM_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_MAIN_TIME_ICON_HR:
        {
            LED_CLOCK_FORMAT_HR_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_MAIN_TIME_ICON_MIN:
        {
            LED_CLOCK_FORMAT_MIN_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_MAIN_TIME_ICON_SEC:
        {
            LED_CLOCK_FORMAT_SEC_ICON_BLINK_FLAG = eBlinkState;
        }break;

        // Upper Icons
        case DISP_POS_UPPER_FAN_ICON:
        {
            LED_UP_FAN_OUTLINE_UP1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_UP2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_UP3_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_RIGHT1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_RIGHT2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_RIGHT3_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_DOWN1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_DOWN2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_DOWN3_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_LEFT1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_LEFT2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_LEFT3_BLINK_FLAG = eBlinkState;

            LED_UP_FAN_INNER_UP1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_UP2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_DOWN1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_DOWN2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_RIGHT2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_OUTLINE_RIGHT3_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_ICON2_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_ICON3_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_ICON4_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_ICON5_BLINK_FLAG = eBlinkState;
            LED_UP_FAN_INNER_ICON6_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_BAKE:
        {
            LED_UP_MODE_BAKE_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_BAKE_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_BROIL:
        {
            LED_UP_MODE_BROIL_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_BROIL_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_HI:
        {
            LED_UP_MODE_HI_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_LO:
        {
            LED_UP_MODE_LOW_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_PROOF:
        {
            LED_UP_MODE_PROOF_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_PROOF_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_WARM:
        {
            LED_UP_MODE_WARM_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_WARM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_SELF:
        {
            LED_UP_MODE_SELF_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_SELF_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_STEAM:
        {
            LED_UP_MODE_STEAM_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_STEAM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_MODE_ICON_CLEAN:
        {
            LED_UP_MODE_CLEAN_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_MODE_CLEAN_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_AM:
        {
            LED_UP_TIME_FORMAT_AM_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_FORMAT_AM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_PM:
        {
            LED_UP_TIME_FORMAT_PM_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_FORMAT_PM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_DLY:
        {
            LED_UP_TIME_FORMAT_DELY_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_FORMAT_DELY_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_HR:
        {
            LED_UP_TIME_FORMAT_HR_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_FORMAT_HR_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_MIN:
        {
            LED_UP_TIME_FORMAT_MIN_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_FORMAT_MIN_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_SEC:
        {
            LED_UP_TIME_FORMAT_SEC_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_FORMAT_SEC_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TIME_ICON_DP:
        {
            LED_UP_TIME_DP1_BLINK_FLAG = eBlinkState;
            LED_UP_TIME_DP2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TEMP_ICON_F:
        {
            LED_UP_TEMP_FORMAT_F_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TEMP_FORMAT_F_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_UPPER_TEMP_ICON_C:
        {
            LED_UP_TEMP_FORMAT_C_ICON1_BLINK_FLAG = eBlinkState;
            LED_UP_TEMP_FORMAT_C_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        // Lower Icons
        case DISP_POS_LOWER_FAN_ICON:
        {
            LED_LOW_FAN_OUTLINE_UP1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_UP2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_UP3_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_RIGHT1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_RIGHT2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_RIGHT3_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_DOWN1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_DOWN2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_DOWN3_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_LEFT1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_LEFT2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_LEFT3_BLINK_FLAG = eBlinkState;

            LED_LOW_FAN_INNER_UP1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_UP2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_DOWN1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_DOWN2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_RIGHT2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_OUTLINE_RIGHT3_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_ICON2_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_ICON3_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_ICON4_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_ICON5_BLINK_FLAG = eBlinkState;
            LED_LOW_FAN_INNER_ICON6_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_BAKE:
        {
            LED_LOW_MODE_BAKE_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_BAKE_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_BROIL:
        {
            LED_LOW_MODE_BROIL_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_BROIL_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_HI:
        {
            LED_LOW_MODE_HI_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_LO:
        {
            LED_LOW_MODE_LOW_ICON_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_PROOF:
        {
            LED_LOW_MODE_PROOF_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_PROOF_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_WARM:
        {
            LED_LOW_MODE_WARM_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_WARM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_SELF:
        {
            LED_LOW_MODE_SELF_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_SELF_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_STEAM:
        {
            LED_LOW_MODE_STEAM_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_STEAM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_MODE_ICON_CLEAN:
        {
            LED_LOW_MODE_CLEAN_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_MODE_CLEAN_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_AM:
        {
            LED_LOW_TIME_FORMAT_AM_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_FORMAT_AM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_PM:
        {
            LED_LOW_TIME_FORMAT_PM_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_FORMAT_PM_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_DLY:
        {
            LED_LOW_TIME_FORMAT_DELY_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_FORMAT_DELY_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_HR:
        {
            LED_LOW_TIME_FORMAT_HR_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_FORMAT_HR_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_MIN:
        {
            LED_LOW_TIME_FORMAT_MIN_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_FORMAT_MIN_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_SEC:
        {
            LED_LOW_TIME_FORMAT_SEC_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_FORMAT_SEC_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TIME_ICON_DP:
        {
            LED_LOW_TIME_DP1_BLINK_FLAG = eBlinkState;
            LED_LOW_TIME_DP2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TEMP_ICON_F:
        {
            LED_LOW_TEMP_FORMAT_F_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TEMP_FORMAT_F_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        case DISP_POS_LOWER_TEMP_ICON_C:
        {
            LED_LOW_TEMP_FORMAT_C_ICON1_BLINK_FLAG = eBlinkState;
            LED_LOW_TEMP_FORMAT_C_ICON2_BLINK_FLAG = eBlinkState;
        }break;

        default:
        {
            ;
        }break;
    }
}

/**
 * DispBlinkControl
 * void, DISP_POSITION, BLINK_STATE
 * eaz.org.171121
 * if there's a blink element already, donot restart a timer.
**/
static void DispBlinkControl(DISP_POSITION eDispPos, BLINK_STATE eBlinkState)
{
    if(eBlinkState == BLINK_STATE_YES && (!TimerStartState(TIMER_DISP_BLINK)))
    {
        StartTimer(TIMER_DISP_BLINK, 300);
    }

    DispBlinkSet(eDispPos, eBlinkState);
}

/**
 * DispControl
 * void, DISP_POSITION, UInt08
 * eaz.org.171205
 * Display or hide icon or digits.
**/
static void DispControl(DISP_POSITION eDispPos, UInt08 ucLedSwitch)
{
    switch(eDispPos)
    {
        case DISP_POS_MAIN_TIME:
        {
            LED_CLOCK_VALUE_POS1 = ucLedSwitch;
            LED_CLOCK_VALUE_POS2 = ucLedSwitch;
            LED_CLOCK_VALUE_POS3 = ucLedSwitch;
            LED_CLOCK_VALUE_POS4 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME:
        {
            LED_UP_TIME_VALUE_POS1 = ucLedSwitch;
            LED_UP_TIME_VALUE_POS2 = ucLedSwitch;
            LED_UP_TIME_VALUE_POS3 = ucLedSwitch;
            LED_UP_TIME_VALUE_POS4 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME:
        {
            LED_LOW_TIME_VALUE_POS1 = ucLedSwitch;
            LED_LOW_TIME_VALUE_POS2 = ucLedSwitch;
            LED_LOW_TIME_VALUE_POS3 = ucLedSwitch;
            LED_LOW_TIME_VALUE_POS4 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TEMP:
        {
            LED_UP_TEMP_VALUE_POS1 = ucLedSwitch;
            LED_UP_TEMP_VALUE_POS2 = ucLedSwitch;
            LED_UP_TEMP_VALUE_POS3 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TEMP:
        {
            LED_LOW_TEMP_VALUE_POS1 = ucLedSwitch;
            LED_LOW_TEMP_VALUE_POS2 = ucLedSwitch;
            LED_LOW_TEMP_VALUE_POS3 = ucLedSwitch;
        }break;

        // Main Icons
        case DISP_POS_MAIN_LOCK_ICON:
        {
            LED_LOCK_ICON1 = ucLedSwitch;
            LED_LOCK_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_MAIN_TIME_ICON_DP:
        {
            LED_CLOCK_DP1 = ucLedSwitch;
            LED_CLOCK_DP2 = ucLedSwitch;
        }break;

        case DISP_POS_MAIN_TIME_ICON_AM:
        {
            LED_CLOCK_FORMAT_AM_ICON = ucLedSwitch;
        }break;

        case DISP_POS_MAIN_TIME_ICON_PM:
        {
            LED_CLOCK_FORMAT_PM_ICON = ucLedSwitch;
        }break;

        case DISP_POS_MAIN_TIME_ICON_HR:
        {
            LED_CLOCK_FORMAT_HR_ICON = ucLedSwitch;
        }break;

        case DISP_POS_MAIN_TIME_ICON_MIN:
        {
            LED_CLOCK_FORMAT_MIN_ICON = ucLedSwitch;
        }break;

        case DISP_POS_MAIN_TIME_ICON_SEC:
        {
            LED_CLOCK_FORMAT_SEC_ICON = ucLedSwitch;
        }break;

        // Upper Icons
        case DISP_POS_UPPER_FAN_ICON:
        {
            LED_UP_FAN_OUTLINE_UP1 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_UP2 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_UP3 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_RIGHT1 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_RIGHT2 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_RIGHT3 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_DOWN1 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_DOWN2 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_DOWN3 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_LEFT1 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_LEFT2 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_LEFT3 = ucLedSwitch;

            LED_UP_FAN_INNER_UP1 = ucLedSwitch;
            LED_UP_FAN_INNER_UP2 = ucLedSwitch;
            LED_UP_FAN_INNER_DOWN1 = ucLedSwitch;
            LED_UP_FAN_INNER_DOWN2 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_RIGHT2 = ucLedSwitch;
            LED_UP_FAN_OUTLINE_RIGHT3 = ucLedSwitch;
            LED_UP_FAN_INNER_ICON1 = ucLedSwitch;
            LED_UP_FAN_INNER_ICON2 = ucLedSwitch;
            LED_UP_FAN_INNER_ICON3 = ucLedSwitch;
            LED_UP_FAN_INNER_ICON4 = ucLedSwitch;
            LED_UP_FAN_INNER_ICON5 = ucLedSwitch;
            LED_UP_FAN_INNER_ICON6 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_BAKE:
        {
            LED_UP_MODE_BAKE_ICON1 = ucLedSwitch;
            LED_UP_MODE_BAKE_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_BROIL:
        {
            LED_UP_MODE_BROIL_ICON1 = ucLedSwitch;
            LED_UP_MODE_BROIL_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_HI:
        {
            LED_UP_MODE_HI_ICON = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_LO:
        {
            LED_UP_MODE_LOW_ICON = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_PROOF:
        {
            LED_UP_MODE_PROOF_ICON1 = ucLedSwitch;
            LED_UP_MODE_PROOF_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_WARM:
        {
            LED_UP_MODE_WARM_ICON1 = ucLedSwitch;
            LED_UP_MODE_WARM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_SELF:
        {
            LED_UP_MODE_SELF_ICON1 = ucLedSwitch;
            LED_UP_MODE_SELF_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_STEAM:
        {
            LED_UP_MODE_STEAM_ICON1 = ucLedSwitch;
            LED_UP_MODE_STEAM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_MODE_ICON_CLEAN:
        {
            LED_UP_MODE_CLEAN_ICON1 = ucLedSwitch;
            LED_UP_MODE_CLEAN_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_AM:
        {
            LED_UP_TIME_FORMAT_AM_ICON1 = ucLedSwitch;
            LED_UP_TIME_FORMAT_AM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_PM:
        {
            LED_UP_TIME_FORMAT_PM_ICON1 = ucLedSwitch;
            LED_UP_TIME_FORMAT_PM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_DLY:
        {
            LED_UP_TIME_FORMAT_DELY_ICON1 = ucLedSwitch;
            LED_UP_TIME_FORMAT_DELY_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_HR:
        {
            LED_UP_TIME_FORMAT_HR_ICON1 = ucLedSwitch;
            LED_UP_TIME_FORMAT_HR_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_MIN:
        {
            LED_UP_TIME_FORMAT_MIN_ICON1 = ucLedSwitch;
            LED_UP_TIME_FORMAT_MIN_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_SEC:
        {
            LED_UP_TIME_FORMAT_SEC_ICON1 = ucLedSwitch;
            LED_UP_TIME_FORMAT_SEC_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TIME_ICON_DP:
        {
            LED_UP_TIME_DP1 = ucLedSwitch;
            LED_UP_TIME_DP2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TEMP_ICON_F:
        {
            LED_UP_TEMP_FORMAT_F_ICON1 = ucLedSwitch;
            LED_UP_TEMP_FORMAT_F_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_UPPER_TEMP_ICON_C:
        {
            LED_UP_TEMP_FORMAT_C_ICON1 = ucLedSwitch;
            LED_UP_TEMP_FORMAT_C_ICON2 = ucLedSwitch;
        }break;

        // Lower Icons
        case DISP_POS_LOWER_FAN_ICON:
        {
            LED_LOW_FAN_OUTLINE_UP1 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_UP2 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_UP3 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_RIGHT1 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_RIGHT2 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_RIGHT3 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_DOWN1 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_DOWN2 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_DOWN3 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_LEFT1 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_LEFT2 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_LEFT3 = ucLedSwitch;

            LED_LOW_FAN_INNER_UP1 = ucLedSwitch;
            LED_LOW_FAN_INNER_UP2 = ucLedSwitch;
            LED_LOW_FAN_INNER_DOWN1 = ucLedSwitch;
            LED_LOW_FAN_INNER_DOWN2 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_RIGHT2 = ucLedSwitch;
            LED_LOW_FAN_OUTLINE_RIGHT3 = ucLedSwitch;
            LED_LOW_FAN_INNER_ICON1 = ucLedSwitch;
            LED_LOW_FAN_INNER_ICON2 = ucLedSwitch;
            LED_LOW_FAN_INNER_ICON3 = ucLedSwitch;
            LED_LOW_FAN_INNER_ICON4 = ucLedSwitch;
            LED_LOW_FAN_INNER_ICON5 = ucLedSwitch;
            LED_LOW_FAN_INNER_ICON6 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_BAKE:
        {
            LED_LOW_MODE_BAKE_ICON1 = ucLedSwitch;
            LED_LOW_MODE_BAKE_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_BROIL:
        {
            LED_LOW_MODE_BROIL_ICON1 = ucLedSwitch;
            LED_LOW_MODE_BROIL_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_HI:
        {
            LED_LOW_MODE_HI_ICON = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_LO:
        {
            LED_LOW_MODE_LOW_ICON = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_PROOF:
        {
            LED_LOW_MODE_PROOF_ICON1 = ucLedSwitch;
            LED_LOW_MODE_PROOF_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_WARM:
        {
            LED_LOW_MODE_WARM_ICON1 = ucLedSwitch;
            LED_LOW_MODE_WARM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_SELF:
        {
            LED_LOW_MODE_SELF_ICON1 = ucLedSwitch;
            LED_LOW_MODE_SELF_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_STEAM:
        {
            LED_LOW_MODE_STEAM_ICON1 = ucLedSwitch;
            LED_LOW_MODE_STEAM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_MODE_ICON_CLEAN:
        {
            LED_LOW_MODE_CLEAN_ICON1 = ucLedSwitch;
            LED_LOW_MODE_CLEAN_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_AM:
        {
            LED_LOW_TIME_FORMAT_AM_ICON1 = ucLedSwitch;
            LED_LOW_TIME_FORMAT_AM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_PM:
        {
            LED_LOW_TIME_FORMAT_PM_ICON1 = ucLedSwitch;
            LED_LOW_TIME_FORMAT_PM_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_DLY:
        {
            LED_LOW_TIME_FORMAT_DELY_ICON1 = ucLedSwitch;
            LED_LOW_TIME_FORMAT_DELY_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_HR:
        {
            LED_LOW_TIME_FORMAT_HR_ICON1 = ucLedSwitch;
            LED_LOW_TIME_FORMAT_HR_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_MIN:
        {
            LED_LOW_TIME_FORMAT_MIN_ICON1 = ucLedSwitch;
            LED_LOW_TIME_FORMAT_MIN_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_SEC:
        {
            LED_LOW_TIME_FORMAT_SEC_ICON1 = ucLedSwitch;
            LED_LOW_TIME_FORMAT_SEC_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TIME_ICON_DP:
        {
            LED_LOW_TIME_DP1 = ucLedSwitch;
            LED_LOW_TIME_DP2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TEMP_ICON_F:
        {
            LED_LOW_TEMP_FORMAT_F_ICON1 = ucLedSwitch;
            LED_LOW_TEMP_FORMAT_F_ICON2 = ucLedSwitch;
        }break;

        case DISP_POS_LOWER_TEMP_ICON_C:
        {
            LED_LOW_TEMP_FORMAT_C_ICON1 = ucLedSwitch;
            LED_LOW_TEMP_FORMAT_C_ICON2 = ucLedSwitch;
        }break;

        default:
        {
            ;
        }break;
    }
}

/**
 * Disp
 * void, DISP_POSITION, BLINK_STATE
 * eaz.org.171205
 * Display icon and set blinkstate.
**/
void Disp(DISP_POSITION eDispPos, BLINK_STATE eBlinkState)
{
    DispControl(eDispPos, LED_ON);
    DispBlinkControl(eDispPos, eBlinkState);
}

/**
 * Hide
 * void, DISP_POSITION
 * eaz.org.171205
 * hide icon/digits and clear blinkstate and scroll state.
**/
void Hide(DISP_POSITION eDispPos)
{
	UInt08 ucScrollerIndex = 4;
	
	switch(eDispPos)
	{
		case DISP_POS_MAIN_TIME:
		{
			ucScrollerIndex = 0;
		}break;
		
		case DISP_POS_UPPER_TIME:
		{
			ucScrollerIndex = 1;
		}break;
		
		case DISP_POS_LOWER_TIME:
		{
			ucScrollerIndex = 2;
		}break;
		
		default:
			break;
	}
	
	if(ucScrollerIndex <= 2)
	{
		DispScrollerStop(ucScrollerIndex);
	}
	
    DispControl(eDispPos, LED_OFF);
    DispBlinkControl(eDispPos, BLINK_STATE_NO);
}

/**
 * DispNumbers
 * void, UInt16, HEAD_ZERO_STATE, DISP_POSITION, BLINK_STATE
 * eaz.org.171120
 * TODO:head_zero not process this time
**/
void DispNumbers(Int16 iValue, HEAD_ZERO_STATE eHeadZeroState, DISP_POSITION eDispPos, BLINK_STATE eBlinkState)
{
    UInt08 pucDispChar[4];
    Number2Chars(iValue, pucDispChar, eHeadZeroState);
    DispBlinkControl(eDispPos, eBlinkState);
    UpdateStringValueToLedBuff(pucDispChar, eDispPos);
}

/**
 * DispString
 * void, UInt16, DISP_POSITION, BLINK_STATE, SCROLL_STATE
 * eaz.org.171120
**/
void DispString(DISP_STR eStrId, DISP_POSITION eDispPos, BLINK_STATE eBlinkState, SCROLL_STATE eScrollState)
{
    UInt08 pucDispChar[4];
    DispBlinkControl(eDispPos, eBlinkState);
    if(eScrollState == SCROLL_STATE_YES)
    {
        DispScrollerStart(eDispPos, eStrId);
    }
    else
    {
        CopySubString(pucDispChar, (UInt08 *)c_xsStrings[eStrId].pucStr, 0, 4);
        UpdateStringValueToLedBuff(pucDispChar, eDispPos);
    }
}

/**
 * DispRefresh
 * void, void
 * eaz.org.171121
**/
void DispRefresh(void)
{
    if(TimeOut(TIMER_DISP_SCROLL))
    {
        DispScrollOneCharacter();
    }

    if(TimeOut(TIMER_DISP_BLINK))
    {
        SwitchLedBlinkState();
    }

    if(TimeOut(TIMER_DISP_REFRESH))
    {
        LedChipsDataRefresh();
    }
}

