/*************************************************************************
* Copyright:    EAZ LAB.
* File:         Sensors.h
* Layer:        Application.ex
* Author:       eaz
* Version:      0.0.1711123
*************************************************************************/
#ifndef	_SENSORS_H_
#define	_SENSORS_H_

// interface
extern void CheckSensors(void);
extern void SensorsInit(void);
extern unsigned char DirtValue();
extern unsigned char DirtErrorFlag();
extern unsigned char DirtSensorEnableFlag();
extern void SetDirtEnableState(bit bEnableState);

#endif	//_SENSORS_H_