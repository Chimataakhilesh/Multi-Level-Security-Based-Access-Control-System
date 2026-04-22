#include<LPC21XX.H>

void eeprom_page_write(u8 SA,u8 WDA,u8 *p,u8 nbytes);
void eeprom_seq_read(u8 SA,u8 RDA,u8 *p,u8 nbytes);

void eeprom_page_write(u8 SA,u8 WDA,u8 *p,u8 nbytes)
{
	u8 i;
	I2C_START();
	I2C_WRITE(SA<<1);
	I2C_WRITE(WDA);
	for(i=0;i<nbytes;i++)
	{
		I2C_WRITE(p[i]);
	}
	I2C_STOP();
	delay_ms(10);
}

void eeprom_seq_read(u8 SA,u8 RDA,u8 *p,u8 nbytes)
{
	u8 i;
	I2C_START();
	I2C_WRITE(SA<<1);
	I2C_WRITE(RDA);
	I2C_RESTART();
	I2C_WRITE((SA<<1)|1);
	for(i=0;i<nbytes-1;i++)
	{
		p[i] = I2C_MASTERACK();
	}
	p[i] = I2C_NACK();
	I2C_STOP();
}
