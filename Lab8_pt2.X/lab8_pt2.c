# include <plib.h>
#include "CerebotMX7cK.h"
#include "I2C_EEPROM_LIB.h"
#include "LCDlib.h"


int main(){
    Cerebot_mx7cK_setup(); // initialize
    init_EEPROM();// initalize EEPROM
    LCDInit();
    
    int startTick = 0, stopTick = 0, readTime = 0, writeTime = 0;
    
    // Varibles for legnth, memAddr, and slaveAddr
    const unsigned int length = 129;
    const unsigned int memAddr = 0x0000;
    const unsigned int slaveAddr = 0x50;
    
    // Data being saved to EEPROM is refered to as 'data in'
    // Data being read from the EEPROM is refered to as 'data out'
    // In short, data is from the perspective of the EEPROM
    unsigned char i2cDataIn[length];
    unsigned char i2cDataOut[length];
    
    // Fill i2cDataIn with stuff
    int i;
    int count = 0x01;
    for (i=0; i<length; i++){
        
        // Reset count when it reaches or is above 0xFF
        if(count >= 0xFF){
            count = 0x01;
        }
        
        // Assign stuff to data going to EEPROM
        i2cDataIn[i] = count;
        
    }
    
    // Start timer for writing
    startTick = ReadCoreTimer();
    writeEEPROM(slaveAddr, memAddr, i2cDataIn, length);// Write!
    stopTick = ReadCoreTimer();
    
    //Calculate time to write
    writeTime = ((stopTick-startTick) / CORE_MS_TICK_RATE);
    
    //Start timer for reading
    startTick = ReadCoreTimer();
    readEEPROM(slaveAddr, memAddr, i2cDataOut, length);// Read!
    stopTick =  ReadCoreTimer();
    
    //Calculate time for reading
    readTime = ((stopTick-startTick) / CORE_MS_TICK_RATE);
    
    i = 0;
    int success = 1;
    int len = length;
    // Does each byte in equal each byte out?
    while(len--){ 
        if (i2cDataIn[i] != i2cDataOut[i]){
            success = 1;
        }
        i++;
    }
    
    // Did it pass?
    if(success != 0){
        LCD_puts("Test Pass");
    }
    else {
        LCD_puts("Test Fail");
    }
    
    while(1){};// Sit idle
    
    return 0;// Should never happen
}
