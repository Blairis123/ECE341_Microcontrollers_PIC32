#include <plib.h>
#include "LCDlib.h"
#include "CerebotMX7cK.h"
#include "I2C_EEPROM_LIB.h"

void init_EEPROM(){
    OpenI2C2(I2C_EN, BRG_VAL);
}

/*
 Function polls EEPROM for page write completion
 * INPUT:
 *      slaveAddr, the address of the slave device to poll
 * OUTPUT:
 *      NA
 */
void wait_I2C_Xfer(int slaveAddr){
    StartI2C2();
    IdleI2C2();
    
    // Continue polling
    while (MasterWriteI2C2((slaveAddr << 1) | 0)){
        RestartI2C2();
        IdleI2C2();
    }
    
    //Final restart
    StopI2C2();
    IdleI2C2();
}

/*
 Input: mem_addr, the memory addr to check
 *      len, the length of data to write
 * 
 * OUTPUT:
 *      0, all good! No problem
 *      1, overflow
 */
int memCheck( int mem_addr, int length){
    if (mem_addr >= 0x800){ // addr larger than memory
        LCD_puts("Addr not valid\nOverflow");
        return 1;
    }
    else if(mem_addr < 0x000){// addr is negative
        LCD_puts("Addr not valid\nUnderflow");
        return 2;
    }
    else if (length < 0){// len is negative
        LCD_puts("Len is negative\n");
        return 3;
    }
    else if(length > 32768){// len won't fit in EEPROM
        LCD_puts("Len > EEPROM\n");
        return 4;
    }
    return 0;
}

/*
 * INPUT:
 *      slaveAddr, addres of the device to read from
 *      mem_addr, address to start reading
 *      i2cData, data buffer to store read values
 *      length, length of data to read
 * OUTPUT:
 *      int, exit status of function
 *      0   :   All good, no errors
 *      1   :   Address overflow
 *      2   :   Address underflow
 *      3   :   Invalid negative length
 *      4   :   Invalid length, length won't fit in EEPROM
 *      5   :   Error while writing
 */
int readEEPROM(int slaveAddr, int mem_addr, char* i2cData, int length){
    
    // Is the memory address valid?
    int mem_error = memCheck(mem_addr, length);
    
    // Check for memory error
    if(mem_error != 0){
        return mem_error;
    }
    
    // Make control, high address, and low address
    unsigned char write_err = 0;
    unsigned int highAddr = ((mem_addr & 0xFF00)>>8);
    unsigned int lowAddr = mem_addr & 0x00FF;
    int i = 0;
    
    StartI2C2();
    IdleI2C2();
    
    // write ctrl byte and mem addr
    write_err |= MasterWriteI2C2((slaveAddr << 1) | 0); //ctrl byte
    write_err |= MasterWriteI2C2(highAddr); // upper mem addr
    write_err |= MasterWriteI2C2(lowAddr); // lower mem addr
    
    if (write_err){
        LCD_puts("Write Error in\nRead EEPROM");
        return 5;
    }
    
    // Restart I2C and write read control byte
    RestartI2C2();
    IdleI2C2();
    write_err |= MasterWriteI2C2((slaveAddr << 1) | 1);// Read command!
    
    // Iterable variable made from length input
    int len = length;
    
    // Read each byte in i2cData
    while (len--){
        
        i2cData[i] = MasterReadI2C2();// Read!
        i++;
        
        if (len>0){
            AckI2C2();
            IdleI2C2();
        }
    }
    
    // Stop reading with a NAK
    NotAckI2C2();
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
    
    //return no error
    return 0;
}

/*
 * INPUT:
 *      slaveAddr, addres of the device to write to
 *      mem_addr, address to start writing
 *      i2cData, data buffer to store values to write
 *      length, length of data to write
 * 
 * OUTPUT:
 *      int, exit status of function
 *      0   :   All good, no errors
 *      1   :   Address overflow
 *      2   :   Address underflow
 *      3   :   Invalid negative length
 *      4   :   Invalid length, length won't fit in EEPROM
 *      5   :   Error while writing
 */
int writeEEPROM(int slaveAddr, int mem_addr, char* i2cData, int length){
    
    // Is the memory address valid?
    int mem_error = memCheck(mem_addr, length);
    
    // Check for return error
    if(mem_error != 0){
        return mem_error;
    }
    
    unsigned int write_err = 0;
    int i = 0;
    int highAddr = (mem_addr & 0xFF00)>>8;// set highAddr
    int lowAddr = mem_addr & 0x00FF;// set lowAddr
    int pagePos = mem_addr;
    
    // Start I2C, write control & addr bytes
    StartI2C2();
    IdleI2C2();
    write_err |= MasterWriteI2C2(((slaveAddr << 1) | 0));//Write control byte
    write_err |= MasterWriteI2C2(highAddr);// Write highAddr
    write_err |= MasterWriteI2C2(lowAddr);// Write lowAddr
    
    // Iterable variable made from length input
    int len = length;
    
    while(len--){
        write_err |= MasterWriteI2C2(i2cData[i++]);
        pagePos++;
        
        if((pagePos % 64)== 0){// check if page boundary
            
            // Stop so EEPROM can do internal write
            StopI2C2();
            IdleI2C2();
            
            // Poll EEPROM for completion
            wait_I2C_Xfer(slaveAddr);
            
            // send ctrl byte as well as the next mem addr
            StartI2C2();
            IdleI2C2();
            write_err |= MasterWriteI2C2(((slaveAddr << 1)|0));//control byte
            write_err |= MasterWriteI2C2((pagePos & 0xFF00)>>8);// high mem_addr
            write_err |= MasterWriteI2C2(pagePos & 0x00FF);// low mem addr
            
            if (write_err){// Something went wrong while writing
                LCD_puts("Write Error in\nWrite EEPROM");
                return 5;
            }
        }
    }
    // Stop I2C activities and idle
    StopI2C2();
    IdleI2C2();
    
    //Wait for EEPROM to do internal page write
    wait_I2C_Xfer(slaveAddr);
    
    // Stop I2C activities and idle
    StopI2C2();
    IdleI2C2();
    
    //Return no error
    return 0;
}

