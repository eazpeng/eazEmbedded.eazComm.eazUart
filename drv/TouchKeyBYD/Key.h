/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         Key.h
* Layer:        Driver.ex
* Author:       eaz
* Version:      0.0.171120
* Brief:        this driver only for BYD
*************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

/* use this part should care about the endian of the mcu */
/* cortex-m0plus is small-endian, use big-endian should change the bits-order */
#pragma pack(push,1)
typedef struct tagKeyMap
{
    union{
        unsigned short usValue;
        struct{
            unsigned short T_K01: 1;
            unsigned short T_K02: 1;
            unsigned short T_K03: 1;
            unsigned short T_K04: 1;
            unsigned short T_K05: 1;
            unsigned short T_K06: 1;
            unsigned short T_K07: 1;
            unsigned short T_K08: 1;
            unsigned short T_K09: 1;
            unsigned short T_K10: 1;
            unsigned short T_K11: 1;
            unsigned short T_K12: 1;
            unsigned short T_K13: 1;
            unsigned short T_K14: 1;
            unsigned short T_K15: 1;
            unsigned short T_K16: 1;
        }xsSigleKey;
    }xuUpperKeys;

    union{
        unsigned short usValue;
        struct{
            unsigned short T_K01: 1;
            unsigned short T_K02: 1;
            unsigned short T_K03: 1;
            unsigned short T_K04: 1;
            unsigned short T_K05: 1;
            unsigned short T_K06: 1;
            unsigned short T_K07: 1;
            unsigned short T_K08: 1;
            unsigned short T_K09: 1;
            unsigned short T_K10: 1;
            unsigned short T_K11: 1;
            unsigned short T_K12: 1;
            unsigned short T_K13: 1;
            unsigned short T_K14: 1;
            unsigned short T_K15: 1;
            unsigned short T_K16: 1;
        }xsSigleKey;
    }xuLowerKeys;
}KeyMap, *PKeyMap;
#pragma pack(pop)

// global paras
extern KeyMap xsKeyMap;

#define KEY_UPPER_BAKE              xsKeyMap.xuUpperKeys.xsSigleKey.T_K01
#define KEY_UPPER_BROIL             xsKeyMap.xuUpperKeys.xsSigleKey.T_K02
#define KEY_UPPER_PROOF_WARM        xsKeyMap.xuUpperKeys.xsSigleKey.T_K03
#define KEY_MAIN_PAD_1              xsKeyMap.xuUpperKeys.xsSigleKey.T_K04
#define KEY_MAIN_PAD_2              xsKeyMap.xuUpperKeys.xsSigleKey.T_K05
#define KEY_UPPER_CLEAN             xsKeyMap.xuUpperKeys.xsSigleKey.T_K06
#define KEY_UPPER_COOKTIME          xsKeyMap.xuUpperKeys.xsSigleKey.T_K07
#define KEY_UPPER_DELAYSTART        xsKeyMap.xuUpperKeys.xsSigleKey.T_K08
#define KEY_MAIN_PAD_4              xsKeyMap.xuUpperKeys.xsSigleKey.T_K09
#define KEY_MAIN_PAD_5              xsKeyMap.xuUpperKeys.xsSigleKey.T_K10
#define KEY_UPPER_TIMER             xsKeyMap.xuUpperKeys.xsSigleKey.T_K11
#define KEY_UPPER_START_ENTER       xsKeyMap.xuUpperKeys.xsSigleKey.T_K12
#define KEY_UPPER_OFF_CLEAR         xsKeyMap.xuUpperKeys.xsSigleKey.T_K13
#define KEY_MAIN_PAD_7              xsKeyMap.xuUpperKeys.xsSigleKey.T_K14
#define KEY_MAIN_PAD_8              xsKeyMap.xuUpperKeys.xsSigleKey.T_K15
#define KEY_UPPER_REV               xsKeyMap.xuUpperKeys.xsSigleKey.T_K16

#define KEY_LOWER_BAKE              xsKeyMap.xuLowerKeys.xsSigleKey.T_K01
#define KEY_LOWER_BROIL             xsKeyMap.xuLowerKeys.xsSigleKey.T_K02
#define KEY_LOWER_PROOF_WARM        xsKeyMap.xuLowerKeys.xsSigleKey.T_K03
#define KEY_MAIN_PAD_3              xsKeyMap.xuLowerKeys.xsSigleKey.T_K04
#define KEY_MAIN_PAD_PLUS           xsKeyMap.xuLowerKeys.xsSigleKey.T_K05
#define KEY_LOWER_CLEAN             xsKeyMap.xuLowerKeys.xsSigleKey.T_K06
#define KEY_LOWER_COOKTIME          xsKeyMap.xuLowerKeys.xsSigleKey.T_K07
#define KEY_LOWER_DELAYSTART        xsKeyMap.xuLowerKeys.xsSigleKey.T_K08
#define KEY_MAIN_PAD_6              xsKeyMap.xuLowerKeys.xsSigleKey.T_K09
#define KEY_MAIN_PAD_MINUS          xsKeyMap.xuLowerKeys.xsSigleKey.T_K10
#define KEY_LOWER_LIGHTS            xsKeyMap.xuLowerKeys.xsSigleKey.T_K11
#define KEY_LOWER_START_ENTER       xsKeyMap.xuLowerKeys.xsSigleKey.T_K12
#define KEY_LOWER_OFF_CLEAR         xsKeyMap.xuLowerKeys.xsSigleKey.T_K13
#define KEY_MAIN_PAD_9              xsKeyMap.xuLowerKeys.xsSigleKey.T_K14
#define KEY_MAIN_PAD_0              xsKeyMap.xuLowerKeys.xsSigleKey.T_K15
#define KEY_LOWER_REV               xsKeyMap.xuLowerKeys.xsSigleKey.T_K16

typedef enum
{
    KEY_STATE_VOID,
    KEY_RD_OK,
    KEY_RD_FAIL,
}KEY_READ_STATE;

typedef enum
{
    KEY_EVENT_VOID,
    // clicked
    KEY_EVENT_UPPER_BAKE_CLICKED,
    KEY_EVENT_UPPER_BROIL_CLICKED,
    KEY_EVENT_UPPER_PROOF_WARM_CLICKED,
    KEY_EVENT_UPPER_CLEAN_CLICKED,
    KEY_EVENT_UPPER_COOKTIME_CLICKED,
    KEY_EVENT_UPPER_DELAYSTART_CLICKED,
    KEY_EVENT_UPPER_TIMER_CLICKED,
    KEY_EVENT_UPPER_START_ENTER_CLICKED,
    KEY_EVENT_UPPER_OFF_CLEAR_CLICKED,
    KEY_EVENT_NUM_0_CLICKED,
    KEY_EVENT_NUM_1_CLICKED,
    KEY_EVENT_NUM_2_CLICKED,
    KEY_EVENT_NUM_3_CLICKED,
    KEY_EVENT_NUM_4_CLICKED,
    KEY_EVENT_NUM_5_CLICKED,
    KEY_EVENT_NUM_6_CLICKED,
    KEY_EVENT_NUM_7_CLICKED,
    KEY_EVENT_NUM_8_CLICKED,
    KEY_EVENT_NUM_9_CLICKED,
    KEY_EVENT_PLUS_CLICKED,
    KEY_EVENT_MINUS_CLICKED,
    KEY_EVENT_LOWER_BAKE_CLICKED,
    KEY_EVENT_LOWER_BROIL_CLICKED,
    KEY_EVENT_LOWER_PROOF_WARM_CLICKED,
    KEY_EVENT_LOWER_CLEAN_CLICKED,
    KEY_EVENT_LOWER_COOKTIME_CLICKED,
    KEY_EVENT_LOWER_DELAYSTART_CLICKED,
    KEY_EVENT_LOWER_LIGHTS_CLICKED,
    KEY_EVENT_LOWER_START_ENTER_CLICKED,
    KEY_EVENT_LOWER_OFF_CLEAR_CLICKED,
    // double clicked
    KEY_EVENT_UPPER_PROOF_WARM_DOUBLE_CLICKED,
    KEY_EVENT_UPPER_CLEAN_DOUBLE_CLICKED,
    KEY_EVENT_LOWER_PROOF_WARM_DOUBLE_CLICKED,
    KEY_EVENT_LOWER_CLEAN_DOUBLE_CLICKED,
    // long pressed
    KEY_EVENT_UPPER_BAKE_AND_UPPER_BROIL_LONG_PRESSED,
    KEY_EVENT_TIMER_AND_LOWER_DELAY_START_AND_LOWER_COOK_TIME_LONG_PRESSED,
}KEY_EVENT;

typedef struct tagKeyEvent
{
    KEY_EVENT       eUpperKeyEvent;
    KEY_EVENT       eMainPadKeyEvent;
    KEY_EVENT       eLowerKeyEvent;
    KEY_EVENT       eCompareKeyEvent;
}KeyEvent, *PKeyEvent;

extern void KeyProcess(PKeyEvent pxsKeyEvent);

#endif  //_KEY_H_
