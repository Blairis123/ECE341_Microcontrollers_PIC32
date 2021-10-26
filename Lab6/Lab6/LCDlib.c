//LCDlib.c ------------------------------
#include <plib.h>
#include "CerebotMX7cK.h"
#include "LCDlib.h"




//Writes to the LCD, RS specifies data or instruction
void writeLCD(int addr, char c)
{
    busyLCD();
	PMPSetAddress(addr); // Set LCD RS control
	PMPMasterWrite(c); // initiate write sequence
} // End of writeLCD

//Reads LCD, RS specifies instruction or data
int readLCD(int addr)
{
    busyLCD();
	PMPSetAddress(addr); // Set LCD RS control
	mPMPMasterReadByte(); // initiate dummy read sequence
	return mPMPMasterReadByte();// read actual data
} // End of readLCD

//Polls the busy flag on the LCD controller and waits for desertion
void busyLCD(void)
{
	//Poll busy flag and wait till it goes low
    PMPSetAddress(0); // Set LCD RS control
	mPMPMasterReadByte(); // initiate dummy read sequence
	int busy = mPMPMasterReadByte() & 0x080;
	while(busy != 0)
    {
        mPMPMasterReadByte();
        busy = mPMPMasterReadByte() & 0x080;
    }
}






void LCD_putc(char c) //Makes decisions about control char
{
    int location = readLCD(0);
    location = location & 0x7F; //Read the location of DDRAM
    
    switch(c)
    {
        case '\r':
            if(location < 0x0F)
            {
                writeLCD(0,0x80);
            }
            else if(location > 0x40)
            {
                writeLCD(0, 0xC0);
            }
            break;
        case '\n':
            //Do control thing
            if(location < 0x0F)
            {
                writeLCD(0,0xC0);
            }
            else if(location > 0x40)
            {
                writeLCD(0, 0x80);
            }
            break;
        default:
            
            if(location > 0x4F)
            {
                writeLCD(0, 0x80);
            }
            if((0x0F < location)&&(location < 0x40))
            {
                writeLCD(0, 0xC0);
                //writeLCD(1, 'X');
            }

            writeLCD(1, c);
    }
}



//Writes a string to the LCD
void LCD_puts(char *char_string)
{
	while(*char_string) // Look for end of string NULL character
	{
        busyLCD();
		LCD_putc(*char_string); // Write character to LCD
		char_string++; // Increment string pointer
	}
} //End of LCD_puts


//Delays a given number of milliseconds (Hardware assisted delay)
/*LCDDelay Function Description ******************************************
* SYNTAX:         void LCDDelay(unsigned int mS);
* DESCRIPTION: This is a millisecond delay function uses the core time
*   to set the base millisecond delay period. Delay periods
*   of zero are permitted. LEDA is toggled each millisecond.
* KEYWORDS:  delay, ms, milliseconds, software delay, core timer
* PARAMETER1: mS - the total number of milliseconds to delay
* RETURN VALUE: None:
* END DESCRIPTION *********************************************************/
void LCDDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();   //Read core timer count -- SW breakpoint
    tWait = (CORE_MS_TICK_RATE * mS);    //Time to wait
    while((ReadCoreTimer() - tStart) < tWait);  //Empty loop, whit for time
    LATBINV = LEDA;
}

//LCD controller initialization
//PMP init
void LCDInit(void)
{
    int cfg1 = PMP_ON|PMP_READ_WRITE_EN|PMP_READ_POL_HI|PMP_WRITE_POL_HI;
    int cfg2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 | PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | PMP_WAIT_END_4;
    int cfg3 = PMP_PEN_0; // only PMA0 enabled
    int cfg4 = PMP_INT_OFF; // no interrupts used
    mPMPOpen(cfg1, cfg2, cfg3, cfg4);
    //END PMP init!
    PMPSetAddress(0); // Set LCD RS control
	LCDDelay(20);
	PMPMasterWrite(0x38); //8 bit data, 2 lines
	LCDDelay(50);
	PMPMasterWrite(0x0f); //display on, cursor on, blink cursor on
	LCDDelay(50);
	PMPMasterWrite(0x01); //Clear display
    LCDDelay(5);
}