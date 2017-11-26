/*************************************************************************
* Copyright:    EAZ LAB.
* File:         Dirt.c
* Layer:        Application.base
* Author:       eaz
* Version:      0.0.171110
*************************************************************************/
// common include
#include "system.h"

// modulation include
#include "Dirt.h"
#include "DirtComm.h"

/**
 * GetDirtSensorParas
 * void, PDirtSensorParas
 * eaz.org.171110
 * Read value and process errors.
**/
void GetDirtSensorParas(PDirtSensorParas pDirtSensorParas)
{
	if(pDirtSensorParas->ucDirtSensorEnableFlag)
	{
        DIRT_COMM_STATE xdata eDirtRevState;
		static unsigned char xdata s_ucDirtRevCheckErrorCounter = 0;
		static unsigned char xdata s_ucDirtRevTimeOutErrorCounter = 0;
		static unsigned char xdata s_ucDirtModuleErrorCounter = 0;

        SimuDirtUartInit();
        eDirtRevState = GetDirtValue(&pDirtSensorParas->ucDirtValue);
        if(DIRT_COMM_REV_NO_DATA == eDirtRevState)
        {
            s_ucDirtRevTimeOutErrorCounter ++;
        }
        else if(DIRT_COMM_REV_CHECK_FAIL == eDirtRevState)
        {
            s_ucDirtRevCheckErrorCounter ++;
        }
        else
        {
            pDirtSensorParas->ucDirtErrorFlag = 0;
            s_ucDirtModuleErrorCounter = 0;
            s_ucDirtRevCheckErrorCounter = 0;
            s_ucDirtRevTimeOutErrorCounter = 0;
        }

        if(s_ucDirtRevCheckErrorCounter > 10 \
            || s_ucDirtRevTimeOutErrorCounter > 200)
        {
            s_ucDirtModuleErrorCounter ++;
            CheckDirtValue();
            s_ucDirtRevCheckErrorCounter = 0;
            s_ucDirtRevTimeOutErrorCounter = 0;
        }

        if(s_ucDirtModuleErrorCounter > 5)
        {
            pDirtSensorParas->ucDirtValue = 0xFF;
            pDirtSensorParas->ucDirtErrorFlag = 1;
        }
	}
}