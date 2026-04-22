#include <LPC21XX.H>

/* EEPROM function prototypes */
void eeprom_page_write(u8 SA, u8 WDA, u8 *p, u8 nbytes);
void eeprom_seq_read(u8 SA, u8 RDA, u8 *p, u8 nbytes);

/* Write data to EEPROM */
void eeprom_page_write(u8 SA, u8 WDA, u8 *p, u8 nbytes)
{
    u8 i;

    I2C_START();
    I2C_WRITE(SA << 1);   // Slave address + write
    I2C_WRITE(WDA);       // Memory address

    for(i = 0; i < nbytes; i++)
    {
        I2C_WRITE(p[i]);  // Write data
    }

    I2C_STOP();
    delay_ms(10);         // Write cycle delay
}

/* Read data from EEPROM */
void eeprom_seq_read(u8 SA, u8 RDA, u8 *p, u8 nbytes)
{
    u8 i;

    I2C_START();
    I2C_WRITE(SA << 1);       // Slave address + write
    I2C_WRITE(RDA);           // Memory address

    I2C_RESTART();
    I2C_WRITE((SA << 1) | 1); // Slave address + read

    for(i = 0; i < nbytes - 1; i++)
    {
        p[i] = I2C_MASTERACK();   // Read with ACK
    }

    p[i] = I2C_NACK();            // Last byte

    I2C_STOP();
}
