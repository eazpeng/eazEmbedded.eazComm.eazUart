/*************************************************************************
* Copyright:    EAZ LAB.
* File:         Sensors.c
* Layer:        Application.ex
* Author:       eaz
* Version:      0.0.171123
*************************************************************************/
#include "Sensors.h"
#include "Dirt.h"

typedef struct tagSensors
{
	// dirt sensor paras
	DirtSensorParas	xsDirtSensorParas;

	// add other sensors
}Sensors, *PSensors;
static Sensors xdata s_xsSensors;

/**
 * CheckSensors
 * void, void
 * eaz.org.171124
 * All sensors' check-value-function here.
**/
void CheckSensors(void)
{
    GetDirtSensorParas(&s_xsSensors.xsDirtSensorParas);
	
	// add other sensors.
}

/**
 * SetDirtEnableState
 * void, bit
 * eaz.org.171124.
 * Set the Dirt sensor enable state.
**/
void SetDirtEnableState(bit bEnableState)
{
    s_xsSensors.xsDirtSensorParas.ucDirtSensorEnableFlag = bEnableState;
}

/**
 * DirtValue
 * unsigned char, void
 * eaz.org.171124.
 * Return the dirt value.
**/
unsigned char DirtValue(void)
{
    return s_xsSensors.xsDirtSensorParas.ucDirtValue;
}

/**
 * DirtErrorFlag
 * unsigned char, void
 * eaz.org.171124.
 * Return the DirtErrorFlag value.
**/
unsigned char DirtErrorFlag(void)
{
    return s_xsSensors.xsDirtSensorParas.ucDirtErrorFlag;
}

/**
 * DirtSensorEnableFlag
 * unsigned char, void
 * eaz.org.171124.
 * Return the Dirt sensor enable state.
**/
unsigned char DirtSensorEnableFlag()
{
    return s_xsSensors.xsDirtSensorParas.ucDirtSensorEnableFlag;
}