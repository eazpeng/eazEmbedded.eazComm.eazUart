/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         SimuTimer.c
* Layer:        Driver.base
* Author:       eaz
* Version:      0.0.171120
* Brief:
*************************************************************************/
#include "SimuTimer.h"

// local data structure
typedef struct tagSimuTimer
{
    unsigned int    uiValue;
    unsigned int    uiValueBak;
    unsigned char   bTimeUpFlag;        // C51 donot support bool type, and bit in struct
    unsigned char   bTimeActiveFlag;    // C51 donot support bool type, and bit in struct
}SimuTimer, *PSimuTimer;
static SimuTimer xdata s_xsTimers[TIMER_SUM];

/**
 * StartTimer
 * void, TIMER_ID, unsigned int
 * eaz.org.171122
 * Start counting.
 * This timer-base is 2ms.
**/
void StartTimer(TIMER_ID eTimerId, unsigned int uiTimeMs)
{
    s_xsTimers[eTimerId].uiValueBak = uiTimeMs >> 1;
    s_xsTimers[eTimerId].uiValue = 0;
    s_xsTimers[eTimerId].bTimeUpFlag = 0;
    s_xsTimers[eTimerId].bTimeActiveFlag = 1;
}

/**
 * TimeOut
 * bool, TIMER_ID
 * eaz.org.171122
 * Return the timer's time-out state, and recount.
**/
bit TimeOut(TIMER_ID eTimerId)
{
    if(s_xsTimers[eTimerId].bTimeUpFlag)
    {
        s_xsTimers[eTimerId].uiValue = 0;
        s_xsTimers[eTimerId].bTimeUpFlag = 0;

        return 1;
    }

    return 0;
}

/**
 * StopTimer
 * void, TIMER_ID
 * eaz.org.171122
 * Stop counting in the handler.
**/
void StopTimer(TIMER_ID eTimerId)
{
    s_xsTimers[eTimerId].bTimeActiveFlag = 0;
    s_xsTimers[eTimerId].uiValueBak = 0;
    s_xsTimers[eTimerId].uiValue = 0;
    s_xsTimers[eTimerId].bTimeUpFlag = 1;
}

/**
 * SimuTimerHandler
 * void, void
 * eaz.org.171122
 * Drop this to a 1ms timer-base ISR.
**/
void SimuTimerHandler(void)
{
    unsigned char xdata ucTimerIndex;

    for(ucTimerIndex = 0; ucTimerIndex<TIMER_SUM; ucTimerIndex ++)
    {
        if(s_xsTimers[ucTimerIndex].bTimeActiveFlag
           && (!s_xsTimers[ucTimerIndex].bTimeUpFlag))
        {
            s_xsTimers[ucTimerIndex].uiValue++;

            if(s_xsTimers[ucTimerIndex].uiValue
               >= s_xsTimers[ucTimerIndex].uiValueBak)
            {
                s_xsTimers[ucTimerIndex].bTimeUpFlag = 1;
            }
        }
    }
}
