#include<LPC21XX.H>		

#define SCL 0x10
#define SDA 0x40

#define CCLK 60000000
#define PCLK CCLK/4
#define I2C_SPEED 100000
#define LOADVAL ((PCLK/I2C_SPEED)/2)

#define AA_BIT 2
#define SI_BIT 3
#define STO_BIT 4
#define STA_BIT 5
#define I2C_EN 6

void I2C_INIT(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_RESTART(void);
void I2C_WRITE(u8);
u8 I2C_NACK(void);
u8 I2C_MASTERACK(void);

/* Initialize I2C */
void I2C_INIT(void)
{
    PINSEL0 |= SCL | SDA;      // Enable I2C pins
    I2SCLL = LOADVAL;          // Set clock low time
    I2SCLH = LOADVAL;          // Set clock high time
    I2CONSET = 1 << I2C_EN;    // Enable I2C
}

/* Start condition */
void I2C_START(void)
{
    I2CONSET = 1 << STA_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    I2CONCLR = 1 << STA_BIT;
}

/* Stop condition */
void I2C_STOP(void)
{
    I2CONSET = 1 << STO_BIT;
    I2CONCLR = 1 << SI_BIT;
}

/* Restart condition */
void I2C_RESTART(void)
{
    I2CONSET = 1 << STA_BIT;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    I2CONCLR = 1 << STA_BIT;
}

/* Write data on I2C bus */
void I2C_WRITE(u8 dat)
{
    I2DAT = dat;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
}

/* Read byte with NACK */
u8 I2C_NACK(void)
{
    I2CONSET = 0x00;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONCLR >> SI_BIT) & 1) == 0);
    return I2DAT;
}

/* Read byte with ACK */
u8 I2C_MASTERACK(void)
{
    I2CONSET = 1 << AA_BIT;
    I2CONCLR = 1 << SI_BIT;
    while(((I2CONSET >> SI_BIT) & 1) == 0);
    I2CONCLR = 1 << AA_BIT;
    return I2DAT;
}
