#include<LPC21XX.H>
#include<stdio.h>

typedef unsigned int u32;
typedef unsigned char u8;

#include "lcd_header.h"
#include "I2C_header.h"
#include "UART_header.h"
#include "EEPROM_header.h"

#define SA 0x50
#define c0 (IOPIN1 &(1<<21))
#define c1 (IOPIN1 &(1<<22))
#define c2 (IOPIN1 &(1<<25))
#define c3 (IOPIN1 &(1<<26))

#define r0 1<<27
#define r1 1<<28
#define r2 1<<29
#define r3 1<<30
#define LED 1<<17
//#define EN_MOTOR 1<<8
#define DC1 1<<19
#define DC2 1<<18

#define OTP_DIGITS 4
#define MAX_OTP_ATTEMPTS 3

void UART_NUM(unsigned int);
int my_strcmp(char *,char *);
void DCMOTOR(void);
void GSM(unsigned int);

u32 otp;
unsigned int key_lut[4][4] = {{'7','8','9','S'},{'4','5','6','T'},{'1','2','3','A'},{'R','0','E','@'}};
unsigned int keyscan(void)
{
	unsigned char row_val,col_val;
	IODIR1 |= r0|r1|r2|r3; //set direction for rows as output
	while(1)
	{
		IOCLR1 = r0|r1|r2|r3;
		IOSET1 = c0|c1|c2|c3;
		while((c0&&c1&&c2&&c3)==1); //waiting for key press
	
		//test for row 0
		IOCLR1 = r0;
		IOSET1 = r1|r2|r3;
		if((c0&&c1&&c2&&c3)==0)
		{
			row_val=0;
			break;
		}
		//test for row 1
		IOCLR1 = r1;
		IOSET1 = r0|r2|r3;
		if((c0&&c1&&c2&&c3)==0)
		{
			row_val=1;
			break;
		}
		//test for row 2
		IOCLR1 = r2;
		IOSET1 = r0|r1|r3;
		if((c0&&c1&&c2&&c3)==0)
		{
			row_val=2;
			break;
		}
		//test for row 3
		IOCLR1 = r3;
		IOSET1 = r0|r1|r2;
		if((c0&&c1&&c2&&c3)==0)
		{
			row_val=3;
			break;
		}
	}//inner while loop ends
	if(c0==0)
		col_val=0;
	else if(c1==0)
		col_val=1;
	else if(c2==0)
		col_val=2;
	else
		col_val=3;
	delay_ms(150); //to avoid switch bouncing
	while((c0&&c1&&c2&&c3)==0);//waithing for key release
	return key_lut[row_val][col_val];
}
#include <LPC21XX.H>

/* Main function */
int main()
{
    u32 i = 28, entered_otp, attempt;
    u8 pass[9], org[9], digit;

    IODIR1 |= DC1 | DC2 | LED;
    IOSET1 = DC1 | DC2 | LED;

    I2C_INIT();
    LCD_INIT();
    UART_INIT();

    eeprom_page_write(SA, 0x00, "STAR@321", 8);

    T1PR = 1500000;
    T1TCR = 0x01;

    LCD_COMMAND(0x80);
    LCD_STRING("  Multi - Level  ");
    LCD_COMMAND(0xC0);
    LCD_STRING(" Security Based ");
    LCD_COMMAND(0x94);
    LCD_STRING("AccessControl System");
    delay_seconds(2);

    LCD_COMMAND(0x01);

    while(1)
    {
        LCD_COMMAND(0x80);
        LCD_STRING("ENTER PASSWORD");
        LCD_COMMAND(0xC0);

        /* Read password */
        for(i = 0; i < 8; i++)
        {
            pass[i] = keyscan();
            LCD_DATA(pass[i]);
        }
        pass[8] = '\0';

        /* Read stored password */
        eeprom_seq_read(SA, 0x00, org, 8);
        org[8] = '\0';

        if(my_strcmp((char *)pass, (char *)org) == 0)
        {
            LCD_COMMAND(0x01);

            attempt = 0;

            while(attempt < MAX_OTP_ATTEMPTS)
            {
                otp = T1PC % 10000;

                GSM(0);

                LCD_COMMAND(0x80);
                LCD_STRING("Enter OTP:");
                LCD_COMMAND(0xC0);

                entered_otp = 0;

                for(i = 0; i < OTP_DIGITS; i++)
                {
                    digit = keyscan();
                    entered_otp = entered_otp * 10 + (digit - '0');
                    LCD_DATA(digit);
                }

                LCD_COMMAND(0x01);

                if(entered_otp == otp)
                {
                    LCD_COMMAND(0x80);
                    LCD_STRING("ACCESS GRANTED");
                    LCD_COMMAND(0xC0);
                    LCD_STRING("LOCKER OPENED");

                    DCMOTOR();
                    IOSET1 = DC1 | DC2 | LED;
                    break;
                }
                else
                {
                    attempt++;

                    if(attempt < MAX_OTP_ATTEMPTS)
                    {
                        LCD_COMMAND(0xC0);
                        LCD_STRING("WRONG OTP");
                        delay_ms(1000);
                        LCD_COMMAND(0x01);
                    }
                    else
                    {
                        LCD_COMMAND(0xC0);
                        LCD_STRING("ACCESS DENIED");
                        GSM(1);
                    }
                }
            }
        }
        else
        {
            LCD_COMMAND(0xC0);
            LCD_STRING("WRONG PASSWORD");
            delay_ms(1000);
        }

        LCD_COMMAND(0x01);
    }
}

/* Send number over UART */
void UART_NUM(unsigned int n)
{
    int i;
    char buf[6];

    sprintf(buf, "%04u", n);

    for(i = 0; buf[i] != '\0'; i++)
    {
        UART_TX(buf[i]);
    }
}

/* Compare two strings */
int my_strcmp(char *s1, char *s2)
{
    int i;

    for(i = 0; i < 8; i++)
    {
        if(s1[i] > s2[i])
            return 1;
        else if(s1[i] < s2[i])
            return -1;
    }

    return 0;
}

/* DC motor control */
void DCMOTOR(void)
{
    IOSET1 = DC1;
    IOCLR1 = DC2 | LED;
    delay_seconds(3);
}

/* GSM SMS function */
void GSM(unsigned int n)
{
    char msg[60];

    UART_STRING("AT\r");
    delay_ms(1000);

    UART_STRING("AT+CMGF=1\r");
    delay_ms(1000);

    UART_STRING("AT+CMGS=\"+919360055735\"\r");
    delay_ms(2000);

    if(n == 0)
    {
        sprintf(msg, "This is your otp:%04u for locker open", otp);
        UART_STRING(msg);
    }
    else if(n == 1)
    {
        UART_STRING("Unauthorized Access");
    }

    delay_ms(500);

    UART_TX(0x1A);
    delay_ms(1000);
}
