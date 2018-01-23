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
    bool            bTimeUpFlag;
    bool            bTimeActiveFlag;
}SimuTimer, *PSimuTimer;
static SimuTimer s_xsTimers[TIMER_SUM];

/**
 * StartTimer
 * void, TIMER_ID, unsigned int
 * eaz.org.171122
 * Start or restart a counting cycle
**/
void StartTimer(TIMER_ID eTimerId, unsigned int uiTimeMs)
{
    s_xsTimers[eTimerId].uiValueBak = uiTimeMs;
    s_xsTimers[eTimerId].uiValue = 0;
    s_xsTimers[eTimerId].bTimeUpFlag = false;
    s_xsTimers[eTimerId].bTimeActiveFlag = true;
}

/**
 * TimeOut
 * bool, TIMER_ID
 * eaz.org.171122
 * Return the timer's time-out state, and recount.
**/
bool TimeOut(TIMER_ID eTimerId)
{
    if(s_xsTimers[eTimerId].bTimeUpFlag && s_xsTimers[eTimerId].bTimeActiveFlag)
    {
        s_xsTimers[eTimerId].uiValue = 0;
        s_xsTimers[eTimerId].bTimeUpFlag = false;

        return true;
    }

    return false;
}

/**
 * StopTimer
 * void, TIMER_ID
 * eaz.org.171122
 * Stop counting in the handler.
**/
void StopTimer(TIMER_ID eTimerId)
{
    s_xsTimers[eTimerId].bTimeActiveFlag = false;
    s_xsTimers[eTimerId].uiValueBak = 0;
    s_xsTimers[eTimerId].uiValue = 0;
    s_xsTimers[eTimerId].bTimeUpFlag = false;
}

/**
 * SimuTimerHandler
 * void, void
 * eaz.org.171122
 * Drop this to a 1ms timer-base ISR.
**/
void SimuTimerHandler(void)
{
    unsigned char ucTimerIndex;

    for(ucTimerIndex = 0; ucTimerIndex<TIMER_SUM; ucTimerIndex ++)
    {
        if(s_xsTimers[ucTimerIndex].bTimeActiveFlag
           && (!s_xsTimers[ucTimerIndex].bTimeUpFlag))
        {
            s_xsTimers[ucTimerIndex].uiValue++;

            if(s_xsTimers[ucTimerIndex].uiValue
               >= s_xsTimers[ucTimerIndex].uiValueBak)
            {
                s_xsTimers[ucTimerIndex].bTimeUpFlag = true;
            }
        }
    }
}
