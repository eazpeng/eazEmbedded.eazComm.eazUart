/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         Led.h
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171118
* Brief:        for AiP1820 LED
*************************************************************************/
#ifndef _LED_H_
#define _LED_H_

#include "EazTypes.h"

#define	LED_ON      (0xFF)
#define LED_OFF     (0x00)
#define BLINK_ON    (0x00)
#define BLINK_OFF   (0xFF)

// Brightness Class
#define LED_BRIGHTNESS_CLASS_H		(0x06)
#define LED_BRIGHTNESS_CLASS_M		(0x03)
#define LED_BRIGHTNESS_CLASS_L		(0x01)

// data structure
// W->White, B->Blue, R->Red, D->Digit.
// D05-07, D16-18 are also blue.
/* use this part should care about the endian of the mcu */
/* cortex-m0plus is small-endian, use big-endian should change the bits-order */
#pragma pack(push,1)
typedef struct tagLedMap
{
    union{
        unsigned char ucDatas[5][7];

        struct{
            union{
                unsigned char ucDatas[7];
                struct{
                    // GR1-1
                    unsigned char W01   : 1;
                    unsigned char W02   : 1;
                    unsigned char W03   : 1;
                    unsigned char W04   : 1;
                    unsigned char W05   : 1;
                    unsigned char W06   : 1;
                    unsigned char W07   : 1;
                    unsigned char W08   : 1;
                    // GR1-2
                    unsigned char W09   : 1;
                    unsigned char W10   : 1;
                    unsigned char W11   : 1;
                    unsigned char W12   : 1;
                    unsigned char B01   : 1;
                    unsigned char B02   : 1;
                    unsigned char B03   : 1;
                    unsigned char B04   : 1;
                    // GR1-3
                    unsigned char B05   : 1;
                    unsigned char B06   : 1;
                    unsigned char B07   : 1;
                    unsigned char B08   : 1;
                    unsigned char B09   : 1;
                    unsigned char B10   : 1;
                    unsigned char W13   : 1;
                    unsigned char W14   : 1;
                    // GR1-4
                    unsigned char W15   : 1;
                    unsigned char W16   : 1;
                    unsigned char W17   : 1;
                    unsigned char W18   : 1;
                    unsigned char W19   : 1;
                    unsigned char W20   : 1;
                    unsigned char W21   : 1;
                    unsigned char W22   : 1;
                    // GR1-5
                    unsigned char W23   : 1;
                    unsigned char W24   : 1;
                    unsigned char W25   : 1;
                    unsigned char W26   : 1;
                    unsigned char W27   : 1;
                    unsigned char W28   : 1;
                    unsigned char W29   : 1;
                    unsigned char W30   : 1;
                    // GR1-6
                    unsigned char D01   : 7;
                    unsigned char NC1   : 1;
                    // GR1-7
                    unsigned char D02   : 7;
                    unsigned char NC2   : 1;
                }Segment;
            }U1;

            union{
                unsigned char ucDatas[7];
                struct{
                    // GR2-1
                    unsigned char D03   : 7;
                    unsigned char W31   : 1;
                    // GR2-2
                    unsigned char D04   : 7;
                    unsigned char W32   : 1;
                    // GR2-3
                    unsigned char D05   : 7;
                    unsigned char W33   : 1;
                    // GR2-4
                    unsigned char D06   : 7;
                    unsigned char W34   : 1;
                    // GR2-5
                    unsigned char D07   : 7;
                    unsigned char W35   : 1;
                    // GR2-6
                    unsigned char W36   : 1;
                    unsigned char W37   : 1;
                    unsigned char W38   : 1;
                    unsigned char W39   : 1;
                    unsigned char W40   : 1;
                    unsigned char B11   : 1;
                    unsigned char B12   : 1;
                    unsigned char B13   : 1;
                    // GR2-7
                    unsigned char W41   : 1;
                    unsigned char W42   : 1;
                    unsigned char W43   : 1;
                    unsigned char W44   : 1;
                    unsigned char W45   : 1;
                    unsigned char W46   : 1;
                    unsigned char NC1   : 1;
                    unsigned char B14   : 1;
                }Segment;
            }U2;

            union{
                unsigned char ucDatas[7];
                struct{
                    // GR3-1
                    unsigned char D08   : 7;
                    unsigned char W47   : 1;
                    // GR3-2
                    unsigned char D09   : 7;
                    unsigned char W48   : 1;
                    // GR3-3
                    unsigned char D10   : 7;
                    unsigned char S01   : 1;
                    // GR3-4
                    unsigned char D11   : 7;
                    unsigned char S02   : 1;
                    // GR3-5
                    unsigned char W49   : 1;
                    unsigned char W50   : 1;
                    unsigned char W51   : 1;
                    unsigned char W52   : 1;
                    unsigned char W53   : 1;
                    unsigned char W54   : 1;
                    unsigned char W55   : 1;
                    unsigned char NC1   : 1;
                    // GR3-6
                    unsigned char DP1   : 1;
                    unsigned char DP2   : 1;
                    unsigned char DP3   : 1;
                    unsigned char DP4   : 1;
                    unsigned char DP6   : 1;
                    unsigned char DP7   : 1;
                    unsigned char NC2   : 1;
                    unsigned char NC3   : 1;
                    // GR3-7
                    unsigned char R01   : 1;
                    unsigned char R02   : 1;
                    unsigned char NC4   : 1;
                    unsigned char NC5   : 1;
                    unsigned char NC6   : 1;
                    unsigned char NC7   : 1;
                    unsigned char NC8   : 1;
                    unsigned char NC9   : 1;
                }Segment;
            }U3;

            union{
                unsigned char ucDatas[7];
                struct{
                    // GR4-1
                    unsigned char W56   : 1;
                    unsigned char W57   : 1;
                    unsigned char W58   : 1;
                    unsigned char W59   : 1;
                    unsigned char W60   : 1;
                    unsigned char W61   : 1;
                    unsigned char W62   : 1;
                    unsigned char W63   : 1;
                    // GR4-2
                    unsigned char W64   : 1;
                    unsigned char W65   : 1;
                    unsigned char W66   : 1;
                    unsigned char W67   : 1;
                    unsigned char B15   : 1;
                    unsigned char B16   : 1;
                    unsigned char B17   : 1;
                    unsigned char B18   : 1;
                    // GR4-3
                    unsigned char B19   : 1;
                    unsigned char B20   : 1;
                    unsigned char B21   : 1;
                    unsigned char B22   : 1;
                    unsigned char B23   : 1;
                    unsigned char B24   : 1;
                    unsigned char W68   : 1;
                    unsigned char W69   : 1;
                    // GR4-4
                    unsigned char W70   : 1;
                    unsigned char W71   : 1;
                    unsigned char W72   : 1;
                    unsigned char W73   : 1;
                    unsigned char W74   : 1;
                    unsigned char W75   : 1;
                    unsigned char W76   : 1;
                    unsigned char W77   : 1;
                    // GR4-5
                    unsigned char W78   : 1;
                    unsigned char W79   : 1;
                    unsigned char W80   : 1;
                    unsigned char W81   : 1;
                    unsigned char W82   : 1;
                    unsigned char W83   : 1;
                    unsigned char W84   : 1;
                    unsigned char W85   : 1;
                    // GR4-6
                    unsigned char D12   : 7;
                    unsigned char NC1   : 1;
                    // GR4-7
                    unsigned char D13   : 7;
                    unsigned char NC2   : 1;
                }Segment;
            }U4;

            union{
                unsigned char ucDatas[7];
                struct{
                    // GR5-1
                    unsigned char D14   : 7;
                    unsigned char W86   : 1;
                    // GR5-2
                    unsigned char D15   : 7;
                    unsigned char W87   : 1;
                    // GR5-3
                    unsigned char D16   : 7;
                    unsigned char W88   : 1;
                    // GR5-4
                    unsigned char D17   : 7;
                    unsigned char W89   : 1;
                    // GR5-5
                    unsigned char D18   : 7;
                    unsigned char W90   : 1;
                    // GR5-6
                    unsigned char W91   : 1;
                    unsigned char W92   : 1;
                    unsigned char W93   : 1;
                    unsigned char W94   : 1;
                    unsigned char W95   : 1;
                    unsigned char B25   : 1;
                    unsigned char B26   : 1;
                    unsigned char B27   : 1;
                    // GR5-7
                    unsigned char NC1   : 1;
                    unsigned char NC2   : 1;
                    unsigned char NC3   : 1;
                    unsigned char NC4   : 1;
                    unsigned char NC5   : 1;
                    unsigned char NC6   : 1;
                    unsigned char NC7   : 1;
                    unsigned char B28   : 1;
                }Segment;
            }U5;
        }xsChips;
    }AllSegs;
}LedMap, *PLedMap;
#pragma pack(pop)

extern LedMap   g_xsLedMap;
extern LedMap   g_xsLedMapBlinkFlags;

// micros
// - upper oven ralated
// -- fan animation ralated
#define LED_UP_FAN_OUTLINE_UP1          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W01
#define LED_UP_FAN_OUTLINE_UP2          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W02
#define LED_UP_FAN_OUTLINE_UP3          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W03
#define LED_UP_FAN_OUTLINE_RIGHT1       		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W04
#define LED_UP_FAN_OUTLINE_RIGHT2       		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W05
#define LED_UP_FAN_OUTLINE_RIGHT3       		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W06
#define LED_UP_FAN_OUTLINE_DOWN1        		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W07
#define LED_UP_FAN_OUTLINE_DOWN2        		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W08
#define LED_UP_FAN_OUTLINE_DOWN3        		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W09
#define LED_UP_FAN_OUTLINE_LEFT1        		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W10
#define LED_UP_FAN_OUTLINE_LEFT2        		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W11
#define LED_UP_FAN_OUTLINE_LEFT3        		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W12
#define LED_UP_FAN_INNER_UP1            		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B05
#define LED_UP_FAN_INNER_UP2            		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B06
#define LED_UP_FAN_INNER_DOWN1          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B09
#define LED_UP_FAN_INNER_DOWN2          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B10
#define LED_UP_FAN_INNER_ICON1          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B01
#define LED_UP_FAN_INNER_ICON2          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B02
#define LED_UP_FAN_INNER_ICON3          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B03
#define LED_UP_FAN_INNER_ICON4          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B04
#define LED_UP_FAN_INNER_ICON5          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B07
#define LED_UP_FAN_INNER_ICON6          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.B08
// -- baking mode ralated               		    
#define LED_UP_MODE_BAKE_ICON1          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W13
#define LED_UP_MODE_BAKE_ICON2          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W14
#define LED_UP_MODE_BROIL_ICON1         		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W21
#define LED_UP_MODE_BROIL_ICON2         		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W22
#define LED_UP_MODE_HI_ICON             		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W23
#define LED_UP_MODE_LOW_ICON            		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W24
#define LED_UP_MODE_PROOF_ICON1         		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W25
#define LED_UP_MODE_PROOF_ICON2         		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W26
#define LED_UP_MODE_WARM_ICON1          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W27
#define LED_UP_MODE_WARM_ICON2          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W28
#define LED_UP_MODE_SELF_ICON1          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W35
#define LED_UP_MODE_SELF_ICON2          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W36
#define LED_UP_MODE_STEAM_ICON1         		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W37
#define LED_UP_MODE_STEAM_ICON2         		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W38
#define LED_UP_MODE_CLEAN_ICON1         		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W39
#define LED_UP_MODE_CLEAN_ICON2         		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W40
// -- time icon                         		    
#define LED_UP_TIME_FORMAT_AM_ICON1     		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W15
#define LED_UP_TIME_FORMAT_AM_ICON2     		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W16
#define LED_UP_TIME_FORMAT_PM_ICON1     		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W17
#define LED_UP_TIME_FORMAT_PM_ICON2     		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W18
#define LED_UP_TIME_FORMAT_DELY_ICON1   		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W19
#define LED_UP_TIME_FORMAT_DELY_ICON2   		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W20
#define LED_UP_TIME_FORMAT_HR_ICON1     		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W29
#define LED_UP_TIME_FORMAT_HR_ICON2     		    g_xsLedMap.AllSegs.xsChips.U1.Segment.W30
#define LED_UP_TIME_FORMAT_MIN_ICON1    		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W31
#define LED_UP_TIME_FORMAT_MIN_ICON2    		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W32
#define LED_UP_TIME_FORMAT_SEC_ICON1    		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W33
#define LED_UP_TIME_FORMAT_SEC_ICON2    		    g_xsLedMap.AllSegs.xsChips.U2.Segment.W34
// -- time value                        		    
#define LED_UP_TIME_VALUE_POS1          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.D01
#define LED_UP_TIME_VALUE_POS2          		    g_xsLedMap.AllSegs.xsChips.U1.Segment.D02
#define LED_UP_TIME_VALUE_POS3          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.D03
#define LED_UP_TIME_VALUE_POS4          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.D04
#define LED_UP_TIME_DP1                 		    g_xsLedMap.AllSegs.xsChips.U3.Segment.DP1
#define LED_UP_TIME_DP2                 		    g_xsLedMap.AllSegs.xsChips.U3.Segment.DP2
// -- temperature format                		    
#define LED_UP_TEMP_FORMAT_F_ICON1      		    g_xsLedMap.AllSegs.xsChips.U2.Segment.B11
#define LED_UP_TEMP_FORMAT_F_ICON2      		    g_xsLedMap.AllSegs.xsChips.U2.Segment.B12
#define LED_UP_TEMP_FORMAT_C_ICON1      		    g_xsLedMap.AllSegs.xsChips.U2.Segment.B13
#define LED_UP_TEMP_FORMAT_C_ICON2      		    g_xsLedMap.AllSegs.xsChips.U2.Segment.B14
// -- temperature value                 		    
#define LED_UP_TEMP_VALUE_POS1          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.D05
#define LED_UP_TEMP_VALUE_POS2          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.D06
#define LED_UP_TEMP_VALUE_POS3          		    g_xsLedMap.AllSegs.xsChips.U2.Segment.D07
// - main area                          		    
// -- clock value                       		    
#define LED_CLOCK_VALUE_POS1            		    g_xsLedMap.AllSegs.xsChips.U3.Segment.D08
#define LED_CLOCK_VALUE_POS2            		    g_xsLedMap.AllSegs.xsChips.U3.Segment.D09
#define LED_CLOCK_VALUE_POS3            		    g_xsLedMap.AllSegs.xsChips.U3.Segment.D10
#define LED_CLOCK_VALUE_POS4            		    g_xsLedMap.AllSegs.xsChips.U3.Segment.D11
#define LED_CLOCK_DP1                   		    g_xsLedMap.AllSegs.xsChips.U3.Segment.DP3
#define LED_CLOCK_DP2                   		    g_xsLedMap.AllSegs.xsChips.U3.Segment.DP4
// -- clock format                      		    
#define LED_CLOCK_FORMAT_AM_ICON        		    g_xsLedMap.AllSegs.xsChips.U3.Segment.W47
#define LED_CLOCK_FORMAT_PM_ICON        		    g_xsLedMap.AllSegs.xsChips.U3.Segment.W48
#define LED_CLOCK_FORMAT_HR_ICON        		    g_xsLedMap.AllSegs.xsChips.U3.Segment.W49
#define LED_CLOCK_FORMAT_MIN_ICON       		    g_xsLedMap.AllSegs.xsChips.U3.Segment.W50
#define LED_CLOCK_FORMAT_SEC_ICON       		    g_xsLedMap.AllSegs.xsChips.U3.Segment.W51
// -- lock icon                         		    
#define LED_LOCK_ICON1                  		    g_xsLedMap.AllSegs.xsChips.U3.Segment.R01
#define LED_LOCK_ICON2                  		    g_xsLedMap.AllSegs.xsChips.U3.Segment.R02
// - lower oven ralated                 		    
// -- fan animation ralated             		    
#define LED_LOW_FAN_OUTLINE_UP1         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W56
#define LED_LOW_FAN_OUTLINE_UP2         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W57
#define LED_LOW_FAN_OUTLINE_UP3         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W58
#define LED_LOW_FAN_OUTLINE_RIGHT1      		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W59
#define LED_LOW_FAN_OUTLINE_RIGHT2      		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W60
#define LED_LOW_FAN_OUTLINE_RIGHT3      		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W61
#define LED_LOW_FAN_OUTLINE_DOWN1       		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W62
#define LED_LOW_FAN_OUTLINE_DOWN2       		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W63
#define LED_LOW_FAN_OUTLINE_DOWN3       		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W64
#define LED_LOW_FAN_OUTLINE_LEFT1       		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W65
#define LED_LOW_FAN_OUTLINE_LEFT2       		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W66
#define LED_LOW_FAN_OUTLINE_LEFT3       		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W67
#define LED_LOW_FAN_INNER_UP1           		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B19
#define LED_LOW_FAN_INNER_UP2           		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B20
#define LED_LOW_FAN_INNER_DOWN1         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B23
#define LED_LOW_FAN_INNER_DOWN2         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B24
#define LED_LOW_FAN_INNER_ICON1         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B15
#define LED_LOW_FAN_INNER_ICON2         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B16
#define LED_LOW_FAN_INNER_ICON3         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B17
#define LED_LOW_FAN_INNER_ICON4         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B18
#define LED_LOW_FAN_INNER_ICON5         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B21
#define LED_LOW_FAN_INNER_ICON6         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.B22
// -- baking mode ralated               		    
#define LED_LOW_MODE_BAKE_ICON1         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W68
#define LED_LOW_MODE_BAKE_ICON2         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W69
#define LED_LOW_MODE_BROIL_ICON1        		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W76
#define LED_LOW_MODE_BROIL_ICON2        		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W77
#define LED_LOW_MODE_HI_ICON            		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W78
#define LED_LOW_MODE_LOW_ICON           		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W79
#define LED_LOW_MODE_PROOF_ICON1        		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W80
#define LED_LOW_MODE_PROOF_ICON2        		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W81
#define LED_LOW_MODE_WARM_ICON1         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W82
#define LED_LOW_MODE_WARM_ICON2         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W83
#define LED_LOW_MODE_SELF_ICON1         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W90
#define LED_LOW_MODE_SELF_ICON2         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W91
#define LED_LOW_MODE_STEAM_ICON1        		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W92
#define LED_LOW_MODE_STEAM_ICON2        		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W93
#define LED_LOW_MODE_CLEAN_ICON1        		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W94
#define LED_LOW_MODE_CLEAN_ICON2        		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W95
// -- time icon                         		    
#define LED_LOW_TIME_FORMAT_AM_ICON1    		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W70
#define LED_LOW_TIME_FORMAT_AM_ICON2    		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W71
#define LED_LOW_TIME_FORMAT_PM_ICON1    		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W72
#define LED_LOW_TIME_FORMAT_PM_ICON2    		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W73
#define LED_LOW_TIME_FORMAT_DELY_ICON1  		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W74
#define LED_LOW_TIME_FORMAT_DELY_ICON2  		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W75
#define LED_LOW_TIME_FORMAT_HR_ICON1    		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W84
#define LED_LOW_TIME_FORMAT_HR_ICON2    		    g_xsLedMap.AllSegs.xsChips.U4.Segment.W85
#define LED_LOW_TIME_FORMAT_MIN_ICON1   		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W86
#define LED_LOW_TIME_FORMAT_MIN_ICON2   		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W87
#define LED_LOW_TIME_FORMAT_SEC_ICON1   		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W88
#define LED_LOW_TIME_FORMAT_SEC_ICON2   		    g_xsLedMap.AllSegs.xsChips.U5.Segment.W89
// -- time value                        		    
#define LED_LOW_TIME_VALUE_POS1         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.D12
#define LED_LOW_TIME_VALUE_POS2         		    g_xsLedMap.AllSegs.xsChips.U4.Segment.D13
#define LED_LOW_TIME_VALUE_POS3         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.D14
#define LED_LOW_TIME_VALUE_POS4         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.D15
#define LED_LOW_TIME_DP1                		    g_xsLedMap.AllSegs.xsChips.U3.Segment.DP6
#define LED_LOW_TIME_DP2                		    g_xsLedMap.AllSegs.xsChips.U3.Segment.DP7
// -- temperature format                		    
#define LED_LOW_TEMP_FORMAT_F_ICON1     		    g_xsLedMap.AllSegs.xsChips.U5.Segment.B25
#define LED_LOW_TEMP_FORMAT_F_ICON2     		    g_xsLedMap.AllSegs.xsChips.U5.Segment.B26
#define LED_LOW_TEMP_FORMAT_C_ICON1     		    g_xsLedMap.AllSegs.xsChips.U5.Segment.B27
#define LED_LOW_TEMP_FORMAT_C_ICON2     		    g_xsLedMap.AllSegs.xsChips.U5.Segment.B28
// -- temperature value                 		    
#define LED_LOW_TEMP_VALUE_POS1         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.D16
#define LED_LOW_TEMP_VALUE_POS2         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.D17
#define LED_LOW_TEMP_VALUE_POS3         		    g_xsLedMap.AllSegs.xsChips.U5.Segment.D18

// blinck micros
// - upper oven ralated
// -- fan animation ralated
#define LED_UP_FAN_OUTLINE_UP1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W01
#define LED_UP_FAN_OUTLINE_UP2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W02
#define LED_UP_FAN_OUTLINE_UP3_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W03
#define LED_UP_FAN_OUTLINE_RIGHT1_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W04
#define LED_UP_FAN_OUTLINE_RIGHT2_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W05
#define LED_UP_FAN_OUTLINE_RIGHT3_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W06
#define LED_UP_FAN_OUTLINE_DOWN1_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W07
#define LED_UP_FAN_OUTLINE_DOWN2_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W08
#define LED_UP_FAN_OUTLINE_DOWN3_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W09
#define LED_UP_FAN_OUTLINE_LEFT1_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W10
#define LED_UP_FAN_OUTLINE_LEFT2_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W11
#define LED_UP_FAN_OUTLINE_LEFT3_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W12
#define LED_UP_FAN_INNER_UP1_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B05
#define LED_UP_FAN_INNER_UP2_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B06
#define LED_UP_FAN_INNER_DOWN1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B09
#define LED_UP_FAN_INNER_DOWN2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B10
#define LED_UP_FAN_INNER_ICON1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B01
#define LED_UP_FAN_INNER_ICON2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B02
#define LED_UP_FAN_INNER_ICON3_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B03
#define LED_UP_FAN_INNER_ICON4_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B04
#define LED_UP_FAN_INNER_ICON5_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B07
#define LED_UP_FAN_INNER_ICON6_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.B08
// -- baking mode ralated                           
#define LED_UP_MODE_BAKE_ICON1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W13
#define LED_UP_MODE_BAKE_ICON2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W14
#define LED_UP_MODE_BROIL_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W21
#define LED_UP_MODE_BROIL_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W22
#define LED_UP_MODE_HI_ICON_BLINK_FLAG              g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W23
#define LED_UP_MODE_LOW_ICON_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W24
#define LED_UP_MODE_PROOF_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W25
#define LED_UP_MODE_PROOF_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W26
#define LED_UP_MODE_WARM_ICON1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W27
#define LED_UP_MODE_WARM_ICON2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W28
#define LED_UP_MODE_SELF_ICON1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W35
#define LED_UP_MODE_SELF_ICON2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W36
#define LED_UP_MODE_STEAM_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W37
#define LED_UP_MODE_STEAM_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W38
#define LED_UP_MODE_CLEAN_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W39
#define LED_UP_MODE_CLEAN_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W40
// -- time icon                                     
#define LED_UP_TIME_FORMAT_AM_ICON1_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W15
#define LED_UP_TIME_FORMAT_AM_ICON2_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W16
#define LED_UP_TIME_FORMAT_PM_ICON1_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W17
#define LED_UP_TIME_FORMAT_PM_ICON2_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W18
#define LED_UP_TIME_FORMAT_DELY_ICON1_BLINK_FLAG    g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W19
#define LED_UP_TIME_FORMAT_DELY_ICON2_BLINK_FLAG    g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W20
#define LED_UP_TIME_FORMAT_HR_ICON1_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W29
#define LED_UP_TIME_FORMAT_HR_ICON2_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.W30
#define LED_UP_TIME_FORMAT_MIN_ICON1_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W31
#define LED_UP_TIME_FORMAT_MIN_ICON2_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W32
#define LED_UP_TIME_FORMAT_SEC_ICON1_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W33
#define LED_UP_TIME_FORMAT_SEC_ICON2_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.W34
// -- time value                                    
#define LED_UP_TIME_VALUE_POS1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.D01
#define LED_UP_TIME_VALUE_POS2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U1.Segment.D02
#define LED_UP_TIME_VALUE_POS3_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.D03
#define LED_UP_TIME_VALUE_POS4_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.D04
#define LED_UP_TIME_DP1_BLINK_FLAG                  g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.DP1
#define LED_UP_TIME_DP2_BLINK_FLAG                  g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.DP2
// -- temperature format                            
#define LED_UP_TEMP_FORMAT_F_ICON1_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.B11
#define LED_UP_TEMP_FORMAT_F_ICON2_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.B12
#define LED_UP_TEMP_FORMAT_C_ICON1_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.B13
#define LED_UP_TEMP_FORMAT_C_ICON2_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.B14
// -- temperature value                             
#define LED_UP_TEMP_VALUE_POS1_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.D05
#define LED_UP_TEMP_VALUE_POS2_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.D06
#define LED_UP_TEMP_VALUE_POS3_BLINK_FLAG           g_xsLedMapBlinkFlags.AllSegs.xsChips.U2.Segment.D07
// - main area                                      
// -- clock value                                   
#define LED_CLOCK_VALUE_POS1_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.D08
#define LED_CLOCK_VALUE_POS2_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.D09
#define LED_CLOCK_VALUE_POS3_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.D10
#define LED_CLOCK_VALUE_POS4_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.D11
#define LED_CLOCK_DP1_BLINK_FLAG                    g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.DP3
#define LED_CLOCK_DP2_BLINK_FLAG                    g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.DP4
// -- clock format                                  
#define LED_CLOCK_FORMAT_AM_ICON_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.W47
#define LED_CLOCK_FORMAT_PM_ICON_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.W48
#define LED_CLOCK_FORMAT_HR_ICON_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.W49
#define LED_CLOCK_FORMAT_MIN_ICON_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.W50
#define LED_CLOCK_FORMAT_SEC_ICON_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.W51
// -- lock icon                                     
#define LED_LOCK_ICON1_BLINK_FLAG                   g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.R01
#define LED_LOCK_ICON2_BLINK_FLAG                   g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.R02
// - lower oven ralated                             
// -- fan animation ralated                         
#define LED_LOW_FAN_OUTLINE_UP1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W56
#define LED_LOW_FAN_OUTLINE_UP2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W57
#define LED_LOW_FAN_OUTLINE_UP3_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W58
#define LED_LOW_FAN_OUTLINE_RIGHT1_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W59
#define LED_LOW_FAN_OUTLINE_RIGHT2_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W60
#define LED_LOW_FAN_OUTLINE_RIGHT3_BLINK_FLAG       g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W61
#define LED_LOW_FAN_OUTLINE_DOWN1_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W62
#define LED_LOW_FAN_OUTLINE_DOWN2_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W63
#define LED_LOW_FAN_OUTLINE_DOWN3_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W64
#define LED_LOW_FAN_OUTLINE_LEFT1_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W65
#define LED_LOW_FAN_OUTLINE_LEFT2_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W66
#define LED_LOW_FAN_OUTLINE_LEFT3_BLINK_FLAG        g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W67
#define LED_LOW_FAN_INNER_UP1_BLINK_FLAG            g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B19
#define LED_LOW_FAN_INNER_UP2_BLINK_FLAG            g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B20
#define LED_LOW_FAN_INNER_DOWN1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B23
#define LED_LOW_FAN_INNER_DOWN2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B24
#define LED_LOW_FAN_INNER_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B15
#define LED_LOW_FAN_INNER_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B16
#define LED_LOW_FAN_INNER_ICON3_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B17
#define LED_LOW_FAN_INNER_ICON4_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B18
#define LED_LOW_FAN_INNER_ICON5_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B21
#define LED_LOW_FAN_INNER_ICON6_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.B22
// -- baking mode ralated                           
#define LED_LOW_MODE_BAKE_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W68
#define LED_LOW_MODE_BAKE_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W69
#define LED_LOW_MODE_BROIL_ICON1_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W76
#define LED_LOW_MODE_BROIL_ICON2_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W77
#define LED_LOW_MODE_HI_ICON_BLINK_FLAG             g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W78
#define LED_LOW_MODE_LOW_ICON_BLINK_FLAG            g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W79
#define LED_LOW_MODE_PROOF_ICON1_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W80
#define LED_LOW_MODE_PROOF_ICON2_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W81
#define LED_LOW_MODE_WARM_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W82
#define LED_LOW_MODE_WARM_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W83
#define LED_LOW_MODE_SELF_ICON1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W90
#define LED_LOW_MODE_SELF_ICON2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W91
#define LED_LOW_MODE_STEAM_ICON1_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W92
#define LED_LOW_MODE_STEAM_ICON2_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W93
#define LED_LOW_MODE_CLEAN_ICON1_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W94
#define LED_LOW_MODE_CLEAN_ICON2_BLINK_FLAG         g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W95
// -- time icon                                     
#define LED_LOW_TIME_FORMAT_AM_ICON1_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W70
#define LED_LOW_TIME_FORMAT_AM_ICON2_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W71
#define LED_LOW_TIME_FORMAT_PM_ICON1_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W72
#define LED_LOW_TIME_FORMAT_PM_ICON2_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W73
#define LED_LOW_TIME_FORMAT_DELY_ICON1_BLINK_FLAG   g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W74
#define LED_LOW_TIME_FORMAT_DELY_ICON2_BLINK_FLAG   g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W75
#define LED_LOW_TIME_FORMAT_HR_ICON1_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W84
#define LED_LOW_TIME_FORMAT_HR_ICON2_BLINK_FLAG     g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.W85
#define LED_LOW_TIME_FORMAT_MIN_ICON1_BLINK_FLAG    g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W86
#define LED_LOW_TIME_FORMAT_MIN_ICON2_BLINK_FLAG    g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W87
#define LED_LOW_TIME_FORMAT_SEC_ICON1_BLINK_FLAG    g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W88
#define LED_LOW_TIME_FORMAT_SEC_ICON2_BLINK_FLAG    g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.W89
// -- time value                                    
#define LED_LOW_TIME_VALUE_POS1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.D12
#define LED_LOW_TIME_VALUE_POS2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U4.Segment.D13
#define LED_LOW_TIME_VALUE_POS3_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.D14
#define LED_LOW_TIME_VALUE_POS4_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.D15
#define LED_LOW_TIME_DP1_BLINK_FLAG                 g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.DP6
#define LED_LOW_TIME_DP2_BLINK_FLAG                 g_xsLedMapBlinkFlags.AllSegs.xsChips.U3.Segment.DP7
// -- temperature format                            
#define LED_LOW_TEMP_FORMAT_F_ICON1_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.B25
#define LED_LOW_TEMP_FORMAT_F_ICON2_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.B26
#define LED_LOW_TEMP_FORMAT_C_ICON1_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.B27
#define LED_LOW_TEMP_FORMAT_C_ICON2_BLINK_FLAG      g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.B28
// -- temperature value                             
#define LED_LOW_TEMP_VALUE_POS1_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.D16
#define LED_LOW_TEMP_VALUE_POS2_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.D17
#define LED_LOW_TEMP_VALUE_POS3_BLINK_FLAG          g_xsLedMapBlinkFlags.AllSegs.xsChips.U5.Segment.D18

// interface
extern void LedChipsInit(void);
extern void LedChipsDataRefresh(void);
extern void SwitchLedBlinkState(void);
extern void SetAllLedSegmentsOn(void);
extern void SetAllLedSegmentsOff(void);
extern bool LedBlinkState(void);
extern void SetLedBrightness(UInt08 ucBrightnessClass);
extern UInt08 GetDigitValueOfChar(UInt08 ucChar);

#endif  //_LED_H_
