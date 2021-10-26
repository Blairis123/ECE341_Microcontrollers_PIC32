#include "LCDlib.h"

//System init prototype
void system_init(void);

char string1[] = "Does Dr J prefer PIC32 or FPGA??";
char string2[] = "Answer: \116\145\151\164\150\145\162\041";

//Used for testing
//char myStr[] = "Hello World! Glad to be here";

int main()
{
    
    system_init(); /* Setup system Hardware. */
    
    //Used for testing
    busyLCD();
    //writeLCD(1, 'A');
    
    //LCD_puts(*myStr);
    
    int wait = 5000;
    while(1)
    {
        /*
        //First part
        writeLCD(0, 0x01); //Clear display
        LCD_puts(string1);
        LCDDelay(wait);
        
        writeLCD(0, 0x01); //Clear display
        LCD_puts(string2);
        LCDDelay(wait);
        */
        //Second part
        LCD_putc('A');
        //LCDDelay(1000);
    }
    return 0; /* Returning a value is expected but this statement
               * should never execute */
}


/* initialize_system Function Description *************************************
 * SYNTAX:          void initialize_system();
 * PARAMETER1:      No Parameters
 * KEYWORDS:        initialize
 * DESCRIPTION:     Sets Registers to default (0) to prevent non zero values 
 *                  from popping up.
 * RETURN VALUE:    None
 * END DESCRIPTION ************************************************************/
void system_init()
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    
    LCDInit(); //Setup LCD, contains the stuff needed for PMP
}

