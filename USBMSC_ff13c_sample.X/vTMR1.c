/***************************************************************************//**
 * @file vTMR1.c
 * @brief	wait timer for any process.
 *			1000Hz 1msec interval.
 *			use TMR1.
 * @author hiroshi murakami
 * @date	20161225
 *
 * This software is released under the MIT License, see LICENSE.md
 ******************************************************************************/
#define _vTMR1_C

#include "mcc_generated_files/mcc.h"
#include "vTMR1.h"
#include "integer.h"
#include "ff_timer.h"

//    static volatile
volatile unsigned int uiTMR001;             // used for USB status
volatile unsigned int uiTMR007;             // used for LED Blink


//******************************************************************************
//	Function Name :Intrupt TIMER1
//	Outline		:タイマ1割り込み処理  1000Hz 1msec interval
//	Input		:
//	Output		:
//******************************************************************************
void Intrrupt_TMR1(void)
{
	if (uiTMR001 > 0) --uiTMR001; // 1000Hz decrement timer with zero stopped
	if (uiTMR007 > 0) --uiTMR007; // 1000Hz decrement timer with zero stopped

    ff13_T1Interrupt(); //from ff13 sample 20191112
}

void vTMR1_init(void)
{
    uiTMR001 = 0;
    uiTMR007 = 0;
}


