/*************************************************************************
* Copyright:    EAZ LAB.
* File:         Dirt.h
* Layer:        Application.base
* Author:       eaz
* Version:      0.0.171110
*************************************************************************/
#ifndef _DIRT_H_
#define _DIRT_H_

// data structure
typedef struct tagDirtSensorParas
{
	unsigned char	ucDirtSensorEnableFlag;
	unsigned char	ucDirtValue;
	unsigned char	ucDirtErrorFlag;
}DirtSensorParas, *PDirtSensorParas;

// interface
extern void DirtSensorInit(void);
extern void GetDirtSensorParas(PDirtSensorParas pDirtSensorParas);

#endif  //_DIRT_H_