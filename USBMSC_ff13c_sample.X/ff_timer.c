
#define _FF_TIMER_C

#include "mcc_generated_files/mcc.h"
#include "ff_timer.h"
#include "diskio.h"


// ff13 VARIABLES
volatile UINT Timer;	/* 1kHz increment timer */
volatile WORD rtcYear = 2017;
volatile BYTE rtcMon = 5, rtcMday = 14, rtcHour, rtcMin, rtcSec;



/*
 * ff3 subroutine 
 */
// from ff13 sample 20191112
void  ff13_T1Interrupt (void)
{
	static const BYTE samurai[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static UINT div1k;
	BYTE n;

	_T1IF = 0;			/* Clear irq flag */
	Timer++;			/* Performance counter for this module */
//	disk_timerproc();	/* Drive timer procedure of low level disk I/O module */

	/* Real Time Clock */
	if (++div1k >= 1000) {
		div1k = 0;
		if (++rtcSec >= 60) {
			rtcSec = 0;
			if (++rtcMin >= 60) {
				rtcMin = 0;
				if (++rtcHour >= 24) {
					rtcHour = 0;
					n = samurai[rtcMon - 1];
					if ((n == 28) && !(rtcYear & 3)) n++;
					if (++rtcMday > n) {
						rtcMday = 1;
						if (++rtcMon > 12) {
							rtcMon = 1;
							rtcYear++;
						}
					}
				}
			}
		}
	}
}


DWORD get_fattime (void)
{
	DWORD tmr;

    INTERRUPT_GlobalDisable();
	/* Pack date and time into a DWORD variable */
	tmr =	  (((DWORD)rtcYear - 1980) << 25)
			| ((DWORD)rtcMon << 21)
			| ((DWORD)rtcMday << 16)
			| (WORD)(rtcHour << 11)
			| (WORD)(rtcMin << 5)
			| (WORD)(rtcSec >> 1);
    INTERRUPT_GlobalEnable();

	return tmr;
}


///*-----------------------------------------------------------------------*/
///* Device Timer Driven Procedure                                         */
///*-----------------------------------------------------------------------*/
///* This function must be called by timer interrupt in period of 1ms      */
//void disk_timerproc (void)
//{
//	BYTE s;
//	UINT n;
//
//
//	n = Timer1;					/* 1000Hz decrement timer with zero stopped */
//	if (n) Timer1 = --n;
//	n = Timer2;
//	if (n) Timer2 = --n;
//
//
//	/* Update socket status */
//
//	s = Stat;
//	if (MMC_WP) {
//		s |= STA_PROTECT;
//	} else {
//		s &= ~STA_PROTECT;
//	}
//	if (MMC_CD) {
//		s &= ~STA_NODISK;
//	} else {
//		s |= (STA_NODISK | STA_NOINIT);
//	}
//	Stat = s;
//}
