/*************************************************************************
* Copyright:    EAZ LAB.
* File:         DirtComm.h
* Layer:        Driver.ex
* Author:       eaz
* Version:      0.0.171110
*************************************************************************/
#ifndef	_DIRT_COMM_H_
#define	_DIRT_COMM_H_

// data structure
typedef enum
{
	DIRT_COMM_NULL,
	DIRT_COMM_REV_OK,
	DIRT_COMM_REV_NO_DATA,
	DIRT_COMM_REV_CHECK_FAIL,
	DIRT_COMM_SND_OK,
	DIRT_COMM_SND_BUSY,
}DIRT_COMM_STATE;

// interface
extern void SimuDirtUartInit(void);
extern void SimuDirtUartCommProcess(void);
extern DIRT_COMM_STATE CheckDirtValue(void);
extern DIRT_COMM_STATE GetDirtValue(unsigned char *pucValue);

#endif	//_DIRT_COMM_H_
