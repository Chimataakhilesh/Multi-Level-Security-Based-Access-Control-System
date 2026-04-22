#include<LPC21XX.H>

void UART_INIT(void);
void UART_TX(u8);
u8 UART_RX(void);
void UART_STRING(char *);

void UART_INIT(void)
{
	PINSEL0 |= 0x5;
	U0LCR = 0x83;
	U0DLL = 97;
	U0DLM = 0;
	U0LCR = 0x03;
}
void UART_TX(u8 txbyte)
{
	U0THR = txbyte;
	while(((U0LSR>>5)&1)==0);
}
u8 UART_RX(void)
{
	while((U0LSR&1)==0);
	return U0RBR;
}
void UART_STRING(char *s)
{
	while(*s)
	{
		UART_TX(*s++);
	}
}
