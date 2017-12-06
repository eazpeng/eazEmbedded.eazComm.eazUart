/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         Key.c
* Layer:        Driver.ex
* Author:       eaz
* Version:      0.0.171120
* Brief:        this driver only for BYD
*************************************************************************/
#include "bsp.h"
#include "I2C.h"
#include "Key.h"
#include "A31G12x_series.h"
#include "A31G12x_pcu.h"

static void SCL1_SET(void);
static void SCL1_CLR(void);
static void SDA1_SET(void);
static void SDA1_CLR(void);
static bool SDA1_VALUE(void);
static void SCL2_SET(void);
static void SCL2_CLR(void);
static void SDA2_SET(void);
static void SDA2_CLR(void);
static bool SDA2_VALUE(void);

#define DEVICE1_ADDR    (0xC0)
#define DEVICE2_ADDR    (0xC0)

static SimuI2C s_xsSimuI2CUpperKey =
{
    DEVICE1_ADDR,

    SCL1_SET,
    SCL1_CLR,
    SDA1_SET,
    SDA1_CLR,
    SDA1_VALUE
};

static SimuI2C s_xsSimuI2CLowerKey =
{
    DEVICE2_ADDR,

    SCL2_SET,
    SCL2_CLR,
    SDA2_SET,
    SDA2_CLR,
    SDA2_VALUE
};

KeyMap xsKeyMap;

typedef struct tagKeyState
{
    union{
        unsigned char   ucValue;
        struct{
            bool        bKeyDownFlag;
            bool        bKeyClickedFlag;
            bool        bKeyClickedFlagBak;
            bool        bKeyDoubleClickedFlag;
            bool        bKeyLongPressFlag;
            bool        bKeyLongPressReleasedFlag;
            bool        bKeyExLongPressFlag;
            bool        bKeyExLongPressReleasedFlag;
        }xsSingleFlag;
    }xuFlags;

    unsigned int    uiKeyPressedCounter;
}KeyState, *PKeyState;
typedef struct tagKeysStates
{
    union{
        KeyState    xsData[2][16];
        struct{
            /* !!! these keys' order must be the same with the KEYMAP structure */
            // upper pad
            KeyState xsKeyUpperBake;
            KeyState xsKeyUpperBroil;
            KeyState xsKeyUpperProofOrWarm;
            KeyState xsKeyMainPadNum1;
            KeyState xsKeyMainPadNum2;
            KeyState xsKeyUpperClean;
            KeyState xsKeyUpperCookTime;
            KeyState xsKeyUpperDelayStart;
            KeyState xsKeyMainPadNum4;
            KeyState xsKeyMainPadNum5;
            KeyState xsKeyUpperTimer;
            KeyState xsKeyUpperStartOrEnter;
            KeyState xsKeyUpperOffOrClear;
            KeyState xsKeyMainPadNum7;
            KeyState xsKeyMainPadNum8;
            // lower pad
            KeyState xsKeyMainPadNum3;
            KeyState xsKeyMainPadPlus;
            KeyState xsKeyLowerBake;
            KeyState xsKeyLowerBroil;
            KeyState xsKeyLowerProofOrWarm;
            KeyState xsKeyMainPadNum6;
            KeyState xsKeyMainPadMinus;
            KeyState xsKeyLowerClean;
            KeyState xsKeyLowerCookTime;
            KeyState xsKeyLowerDelayStart;
            KeyState xsKeyMainPadNum9;
            KeyState xsKeyMainPadNum0;
            KeyState xsKeyLowerLights;
            KeyState xsKeyLowerStartOrEnter;
            KeyState xsKeyLowerOffOrClear;
        }xsSingleKey;
    }xuKeys;
}KeysStates, *PKeysStates;
static KeysStates s_xsKeysStates;

/**
 * ReadKeyValue
 * KEY_READ_STATE, PSimuI2C, unsigned short *
 * eaz.org.171122
**/
static KEY_READ_STATE ReadKeyValue(PSimuI2C pxsSimuI2C, unsigned short *pucKeyValue)
{
    unsigned char ucReadData;

    I2CStart(pxsSimuI2C);
    I2CSendByte(pxsSimuI2C, pxsSimuI2C->ucDeviceAddr | 0x01); // device address and READ bit
    if(I2CWaitACK(pxsSimuI2C) != I2C_ACK)
    {
        I2CStop(pxsSimuI2C);
        return KEY_RD_FAIL;
    }

    I2CReceiveByte(pxsSimuI2C, &ucReadData);
    I2CSendACK(pxsSimuI2C);
    *pucKeyValue = ucReadData;

    I2CReceiveByte(pxsSimuI2C, &ucReadData);
    I2CSendNACK(pxsSimuI2C);
    I2CStop(pxsSimuI2C);

    *pucKeyValue += ucReadData << 8;

    return KEY_RD_OK;
}

/**OK
 * UpdateKeysStates
 * void, unsigned char
 * eaz.org.171122
 * TODO.eaz: Need to measure the execute time & add key press/release filter(maybe?)
**/
static void UpdateKeysStates(unsigned char ucIndex)
{
    unsigned short  usShiftRegister = 0x0001;
    unsigned char   ucCounter;

    for(ucCounter = 0; ucCounter < 16; ucCounter++)
    {
        if(xsKeyMap.xuUpperKeys.usValue & usShiftRegister)
        {
            if(s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyDownFlag)
            {
                if(s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter >= 300)
                {
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyExLongPressFlag = true;
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyLongPressFlag = false;
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter = 300;
                }
                else if(s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter >= 200)
                {
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyLongPressFlag = true;
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter ++;
                }
                else
                {
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter ++;
                }
            }
            else
            {
                s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyDownFlag = true;
                s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter ++;
            }
        }
        // released
        else
        {
            // already down
            if(s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyDownFlag)
            {
                if(s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter >= 300)
                {
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyExLongPressFlag = false;
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyExLongPressReleasedFlag = true;
                }
                else if(s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter >= 200)
                {
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyLongPressFlag = false;
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyLongPressReleasedFlag = true;
                }
                else
                {
                    s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyClickedFlag = true;
                }

                s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].xuFlags.xsSingleFlag.bKeyDownFlag = 0;
                s_xsKeysStates.xuKeys.xsData[ucIndex][ucCounter].uiKeyPressedCounter = 0;
            }
            else
            {
                ;// nothing happened
            }
        }
        usShiftRegister <<= 1;
    }
}

static KEY_EVENT JudgeUpperKeyEvent()
{
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperBake.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperBake.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_BAKE_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperBroil.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperBroil.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_BROIL_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperProofOrWarm.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperProofOrWarm.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_PROOF_WARM_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperClean.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperClean.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_CLEAN_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperCookTime.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperCookTime.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_COOKTIME_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperDelayStart.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperDelayStart.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_DELAYSTART_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperTimer.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperTimer.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_TIMER_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperStartOrEnter.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperStartOrEnter.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_START_ENTER_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperOffOrClear.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyUpperOffOrClear.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_UPPER_OFF_CLEAR_CLICKED;
    }

    return KEY_EVENT_VOID;
}

static KEY_EVENT JudgeLowerKeyEvent()
{
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerBake.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerBake.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_BAKE_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerBroil.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerBroil.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_BROIL_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerProofOrWarm.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerProofOrWarm.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_PROOF_WARM_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerClean.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerClean.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_CLEAN_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerCookTime.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerCookTime.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_COOKTIME_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerDelayStart.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerDelayStart.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_DELAYSTART_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerLights.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerLights.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_LIGHTS_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerStartOrEnter.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerStartOrEnter.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_START_ENTER_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerOffOrClear.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyLowerOffOrClear.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_LOWER_OFF_CLEAR_CLICKED;
    }

    return KEY_EVENT_VOID;
}

static KEY_EVENT JudgeMainPadKeyEvent()
{
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum0.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum0.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_0_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum1.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum1.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_1_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum2.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum2.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_2_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum3.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum3.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_3_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum4.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum4.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_4_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum5.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum5.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_5_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum6.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum6.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_6_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum7.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum7.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_7_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum8.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum8.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_8_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum9.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadNum9.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_NUM_9_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadPlus.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadPlus.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_PLUS_CLICKED;
    }
    if(s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadMinus.xuFlags.xsSingleFlag.bKeyClickedFlag)
    {
        s_xsKeysStates.xuKeys.xsSingleKey.xsKeyMainPadMinus.xuFlags.xsSingleFlag.bKeyClickedFlag = false;
        return KEY_EVENT_MINUS_CLICKED;
    }

    return KEY_EVENT_VOID;
}
static KEY_EVENT JudgeCompareKeyEvent(PKeyEvent pxsKeyEvent)
{
    return KEY_EVENT_VOID;
}

static void JudgeKeyEvent(PKeyEvent pxsKeyEvent)
{
    // upper
    pxsKeyEvent->eUpperKeyEvent = JudgeUpperKeyEvent();
    pxsKeyEvent->eLowerKeyEvent = JudgeLowerKeyEvent();
    pxsKeyEvent->eMainPadKeyEvent = JudgeMainPadKeyEvent();
    pxsKeyEvent->eCompareKeyEvent = JudgeCompareKeyEvent(pxsKeyEvent);
}

/**OK
 * KeyProcess
 * void, void
 * eaz.org.171122
 * if there are several actions, the first valid key action SCANED will be the output KEY_VALUE
 * the other actions, will SCANED sequencely, maybe a little bit delay.
 * Key scan cycle is 10ms
**/
void KeyProcess(PKeyEvent pxsKeyEvent)
{
    ReadKeyValue(&s_xsSimuI2CUpperKey, &xsKeyMap.xuUpperKeys.usValue);
    ReadKeyValue(&s_xsSimuI2CLowerKey, &xsKeyMap.xuLowerKeys.usValue);

    UpdateKeysStates(0);
    UpdateKeysStates(1);

    JudgeKeyEvent(pxsKeyEvent);
}

/**OK
 * SDA1_SET
 * void, void
 * eaz.org.171122
**/
static void SDA1_SET(void)
{
    P_T1_SDA_H;
	P_T1_SDA_O;
}

/**OK
 * SDA1_CLR
 * void, void
 * eaz.org.171122
**/
static void SDA1_CLR(void)
{
    P_T1_SDA_L;
	P_T1_SDA_O;
}

/**OK
 * SCL1_SET
 * void, void
 * eaz.org.171122
**/
static void SCL1_SET(void)
{
    P_T1_SCL_H;
}

/**OK
 * SCL1_CLR
 * void, void
 * eaz.org.171122
**/
static void SCL1_CLR(void)
{
    P_T1_SCL_L;
}

/**OK
 * SDA1_VALUE
 * bool, void
 * eaz.org.171122
**/
static bool SDA1_VALUE(void)
{
    // SetDirect
	P_T1_SDA_I;
    return P_T1_SDA;
}

/**OK
 * SDA2_SET
 * void, void
 * eaz.org.171122
**/
static void SDA2_SET(void)
{
    P_T2_SDA_H;
	P_T2_SDA_O;
}

/**OK
 * SDA2_CLR
 * void, void
 * eaz.org.171122
**/
static void SDA2_CLR(void)
{
    P_T2_SDA_L;
	P_T2_SDA_O;
}

/**OK
 * SCL2_SET
 * void, void
 * eaz.org.171122
**/
static void SCL2_SET(void)
{
    P_T2_SCL_H;
}

/**OK
 * SCL2_CLR
 * void, void
 * eaz.org.171122
**/
static void SCL2_CLR(void)
{
    P_T2_SCL_L;
}

/**OK
 * SDA2_VALUE
 * bool, void
 * eaz.org.171122
**/
static bool SDA2_VALUE(void)
{
    // SetDirect
	P_T2_SDA_I;
    return P_T2_SDA;
}
