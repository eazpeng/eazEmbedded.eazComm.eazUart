/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         SimuTimer.h
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171120
* Brief:
*************************************************************************/
#ifndef _SIMU_TIMER_H_
#define _SIMU_TIMER_H_

#include "stdbool.h"

// data structure
// Just add a enum-value here if you need.
typedef enum
{
    TIMER_BASE,
    TIMER_SENSORS,
    TIMER_SUM
}TIMER_ID;

// interface
extern void SimuTimerHandler(void);
extern void StartTimer(TIMER_ID eTimerId, unsigned int uiTimeMs);
extern void StopTimer(TIMER_ID eTimerId);
extern bool TimeOut(TIMER_ID eTimerId);

#endif //_SIMU_TIMER_H_
