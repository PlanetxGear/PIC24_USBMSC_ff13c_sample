/***************************************************************************//**
 * @file ff_timer.h
 ******************************************************************************/
#ifndef _FF_TIMER_H
#define _FF_TIMER_H

#include "integer.h"

/*****************************
 * DEFINES
 *****************************/

/*****************************
 * VARIABLES
 *****************************/
#if defined _FF_TIMER_C
#else
// ff13 VARIABLES
volatile extern UINT Timer;	/* 1kHz increment timer */
volatile extern WORD rtcYear;
volatile extern BYTE rtcMon, rtcMday, rtcHour, rtcMin, rtcSec;


#endif

/*****************************
 * PROTOTYPES
 *****************************/
void  ff13_T1Interrupt (void);  // from ff13 sample 20191112
DWORD get_fattime (void);

#endif
