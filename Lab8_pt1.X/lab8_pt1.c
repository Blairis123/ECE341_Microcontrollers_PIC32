/*
 This is a single byte read/write
 *  MasterWriteI2C2() & MasterReadI2C2() are blocking
 *  no IdleI2C2 is needed but StartI2C2() is
 */

#include <plib.h>
#include "CerebotMX7cK.h"

char BusyI2C2(void)
{
    return(I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RSEN || I2C2CONbits.RCEN || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT);
}// Returns non zero if I2C controller is busy

#define Fsck 400000
#define BRG_VAL ((FPB/2/Fsck)-2)
#define TEST_VAL 0xE7   //Should be arrow pointing to the right of screen

int main(void)
{
    unsigned char SlaveAddress, i2cbyte, write_err=0;
    char i2cdata[10]={0,0,0,0,0,0,0,0,0,0};
    int datasz, index;
    
    //Init resources
    Cerebot_mx7cK_setup();
    OpenI2C2( I2C_EN, BRG_VAL);// Enable I2C
    if (BusyI2C2()) return(1);// I2C bus not ready
    
    //Create I2C frame
    SlaveAddress = 0x50;
    i2cdata[0] = ((SlaveAddress <<1) | 0);// Control Byte
    i2cdata[1] = 0x03;// Memory address (MSB)
    i2cdata[2] = 0x41;// Memory address (LSB)
    i2cdata[3] = TEST_VAL;// Single Data Byte
    
    //Send I2C Frame to EEPROM ---- WRITE!
    datasz = 4;
    index = 0;
    StartI2C2();// Non-blocking
    IdleI2C2();
    while(datasz--)
        write_err |= MasterWriteI2C2(i2cdata[index++]);
    StopI2C2();
    IdleI2C2();
    
    //Main should never return anything but ....
    if(write_err) return(1);// SOME PROBLEM DURING WRITE
    
    // Poll EEPROM for write completion
    StartI2C2();
    IdleI2C2();
    while(MasterWriteI2C2(i2cdata[0]))
    {
        RestartI2C2();// try restart instead of stop if NO ack
        IdleI2C2();
    }
    
    // Read from EEPROM
    datasz = 3;
    index = 0;
    StartI2C2();
    IdleI2C2();
    while(datasz--)
        MasterWriteI2C2(i2cdata[index++]);// didn't check for ACK
    RestartI2C2();// Restart & reverse I2c bus direction
    IdleI2C2();
    MasterWriteI2C2( (SlaveAddress << 1) | 1);
    i2cbyte = MasterReadI2C2();
    NotAckI2C2();// End read with NO ACK
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
    
    // Check data for match!
    if(i2cbyte == TEST_VAL)
    {
        PORTSetBits(IOPORT_G, LED4);
    }
    else
    {
        PORTSetBits(IOPORT_G, LED1);
    }
    
    // Program should get stuck here
    while(1);
    
    //This should never happen
    return(1);
}