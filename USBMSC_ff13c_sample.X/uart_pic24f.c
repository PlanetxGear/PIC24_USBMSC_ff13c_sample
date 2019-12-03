/*------------------------------------------------------------------------/
/  UART control module for PIC24F                          (C)ChaN, 2010
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2010, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include <p24FJ64GB002.h>
#include "uart_pic24f.h"
#include "mcc_generated_files/mcc.h"

#define BUFFER_SIZE 128


static volatile int TxRun;		/* Tx running flag */
static volatile struct {
	int		ri, wi, ct;			/* Read index, Write index, Data counter */
	unsigned char	buff[BUFFER_SIZE];	/* FIFO buffer */
} TxFifo, RxFifo;




/* UART1 Rx interrupt ISR */

//void __attribute__((interrupt, auto_psv)) _U1RXInterrupt (void)
void Interrupt_U1RX (void)
{
	unsigned char d;
	int i;


	d = (unsigned char)U1RXREG;	/* Get received data */
	_U1RXIF = 0;				/* Clear Rx interrupt flag */
	i = RxFifo.ct;				/* Number of bytes in the FIFO */
	if (i < BUFFER_SIZE) {		/* Skip if FIFO is full */
		RxFifo.ct = ++i;
		i = RxFifo.wi;
		RxFifo.buff[i++] = d;	/* Store data into the FIFO */
		RxFifo.wi = i % BUFFER_SIZE;	/* Next write ptr */
	}
}



/* UART1 Tx interrupt ISR */

//void __attribute__((interrupt, auto_psv)) _U1TXInterrupt (void)
void  Interrupt_U1TX (void)
{
	int i;

    if(TxFifo.ct == 0)
    {
        IEC0bits.U1TXIE = 0;
        return;
    }


    IFS0bits.U1TXIF = 0; /* Clear Tx interrupt flag */

    while(!(U1STAbits.UTXBF == 1))
    {
    	i = TxFifo.ct;		/* Number of data in the FIFO */
        if (i) {			/* If any data is available, pop a byte and send it. */
            TxFifo.ct = --i;
            i = TxFifo.ri;
            U1TXREG = TxFifo.buff[i++];		/* Send a byte */
            TxFifo.ri = i % BUFFER_SIZE;	/* Next read ptr */
        } else {			/* No data in the Tx FIFO */
            TxRun = 0;		/* Stop transmission sequense */
            return;
        }
    }
}



/* Check number of bytes in the Rx FIFO */

int uart_test (void)
{
	return RxFifo.ct;	/* Returns number of bytes in the Rx FIFO */
}



/* Get a byte from Rx FIFO */

unsigned char uart_getc (void)
{
	unsigned char d;
	int i;


	while (!RxFifo.ct) ;		/* Wait while Rx FIFO empty */

	i = RxFifo.ri;				/* Get a byte from Rx FIFO */
	d = RxFifo.buff[i++];
	RxFifo.ri = i % BUFFER_SIZE;
	INTERRUPT_GlobalDisable();
	RxFifo.ct--;
	INTERRUPT_GlobalEnable();

	return d;
}



/* Put a byte into Tx FIFO */

void uart_putc (unsigned char d)
{
	int i;


	while (TxFifo.ct >= BUFFER_SIZE) ;	/* Wait while Tx FIFO is full */

	i = TxFifo.wi;		/* Put a data into the Tx FIFO */
	TxFifo.buff[i++] = d;
	TxFifo.wi = i % BUFFER_SIZE;
	INTERRUPT_GlobalDisable();
	TxFifo.ct++;
//	if (!TxRun) {		/* If transmission sequense is not running, start the tx sequense */
	if (TxFifo.ct) {		/* If transmission sequense is not running, start the tx sequense */
		TxRun = 1;
//		_U1TXIF = 1;	/* Force trigger Tx interrupt */
        IEC0bits.U1TXIE = 1;    /* Force trigger Tx interrupt */
	}
	INTERRUPT_GlobalEnable();
}



/* Initialize UART module */

void uart_init (unsigned long bps)
{
	/* Disable UART1 Tx/Rx interrupts */
	_U1RXIE = 0;
	_U1TXIE = 0;

	/* Initialize UART1 */
//	U1BRG = FCY / 16 / bps - 1;
	_UARTEN = 1;
	_UTXEN = 1;

	/* Clear Tx/Rx FIFOs */
	TxFifo.ri = 0; TxFifo.wi = 0; TxFifo.ct = 0;
	RxFifo.ri = 0; RxFifo.wi = 0; RxFifo.ct = 0;
	TxRun = 0;

	/* Enable UART Tx/Rx interruptrs */
	_U1RXIE = 1;
	_U1TXIE = 1;
}


