#include<LPC21XX.H>
#include "delay_header.h"
#define LCD_D 0xF<<14
#define RS 1<<12
#define E 1<<13

void LCD_INIT(void);
void LCD_COMMAND(u8);
void LCD_DATA(u8);
void LCD_STRING(u8 *);

/* Initialize LCD */
void LCD_INIT(void)
{
    IODIR0 = LCD_D | RS | E;
    LCD_COMMAND(0x01);  // Clear display
    LCD_COMMAND(0x02);  // Return home
    LCD_COMMAND(0x0C);  // Display ON
    LCD_COMMAND(0x28);  // 4-bit mode
}

/* Send command to LCD */
void LCD_COMMAND(u8 cmd)
{
    IOCLR0 = LCD_D;

    IOSET0 = (cmd & 0xF0) << 10;
    IOCLR0 = RS;
    IOSET0 = E;
    delay_ms(2);
    IOCLR0 = E;

    IOCLR0 = LCD_D;
    IOSET0 = (cmd & 0x0F) << 14;
    IOCLR0 = RS;
    IOSET0 = E;
    delay_ms(2);
    IOCLR0 = E;
}

/* Send data to LCD */
void LCD_DATA(u8 d)
{
    IOCLR0 = LCD_D;

    IOSET0 = (d & 0xF0) << 10;
    IOSET0 = RS;
    IOSET0 = E;
    delay_ms(2);
    IOCLR0 = E;

    IOCLR0 = LCD_D;
    IOSET0 = (d & 0x0F) << 14;
    IOSET0 = RS;
    IOSET0 = E;
    delay_ms(2);
    IOCLR0 = E;
}

/* Display string on LCD */
void LCD_STRING(u8 *s)
{
    while(*s)
    {
        LCD_DATA(*s++);
    }
}

