#ifndef _COMMFUNC
#define _COMMFUNC

void uart_init (unsigned long bps);
int uart_test (void);
void uart_putc (unsigned char d);
unsigned char uart_getc (void);
void Interrupt_U1RX(void);    
void Interrupt_U1TX(void);    

#endif

