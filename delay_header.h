#include <LPC21XX.H>

void delay_seconds(u32);
void delay_ms(u32);
void delay_us(u32);

void delay_seconds(u32 seconds)
{
T0PR = 15000000-1;
T0TCR = 0X01;
while(T0TC<seconds);
T0TCR = 0X03;
T0TCR = 0X00;
}
void delay_ms(u32 ms)
{
T0PR = 15000-1;
T0TCR = 0X01;
while(T0TC<ms);
T0TCR = 0X03;
T0TCR = 0X00;
}
void delay_us(u32 us)
{
T0PR = 15-1;
T0TCR = 0X01;
while(T0TC<us);
T0TCR = 0X03;
T0TCR = 0X00;
}
