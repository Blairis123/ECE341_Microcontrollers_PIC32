#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project2.h"

#define COUNTS_PER_MS 8890   // Initial guess
/* Function prototypes */
void system_init(void);
void sw_msDelay(unsigned int);
void hw_msDelay(unsigned int);


int main()
{
    int mS = 1;   /* Set total delay time ? change as needed */
    system_init(); /* Setup system Hardware. */
    while(1)
    {
        LATBINV = LEDB; /* Toggle LEDB each delay period */
        /* Run with only one of the two following statements uncommented */
        //sw_msDelay(1000);  /* Software only delay */
        hw_msDelay(1000);   /* Hardware-assisted delay */
    }
    return 0; /* Returning a value is expected but this statement
               * should never execute */
}

// LEDA is defined in CerebotMX7cK.h  
void sw_msDelay(unsigned int mS)
{
    int i;
    while(mS --)
    {
        for (i = 0; i< COUNTS_PER_MS; i++)  // 1 ms delay loop
        {
            //do nothing
        }
        LATBINV = LEDA; // Toggle LEDA each ms for instrumentation
    }       
}                       //Software breakpoint

/*hw_msDelay Function Description ******************************************
* SYNTAX:         void hw_msDelay(unsigned int mS);
* DESCRIPTION: This is a millisecond delay function uses the core time
*   to set the base millisecond delay period. Delay periods
*   of zero are permitted. LEDA is toggled each millisecond.
* KEYWORDS:  delay, ms, milliseconds, software delay, core timer
* PARAMETER1: mS - the total number of milliseconds to delay
* RETURN VALUE: None:
* END DESCRIPTION *********************************************************/
void hw_msDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();   //Read core timer count -- SW breakpoint
    tWait = (CORE_MS_TICK_RATE * mS);    //Time to wait
    while((ReadCoreTimer() - tStart) < tWait);  //Empty loop, whit for time
    LATBINV = LEDA;
}

/* initialize_system Function Description *************************************
 * SYNTAX:          void initialize_system();
 * PARAMETER1:      No Parameters
 * KEYWORDS:        initialize
 * DESCRIPTION:     Sets Registers to default (0) to prevent non zero values 
 *                  from popping up.
 * RETURN VALUE:    None
 * END DESCRIPTION ************************************************************/
void system_init(void)
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);/* Set PmodSTEP LEDs outputs */
    LATBCLR = SM_LEDS;    /* Turn off LEDA through LEDH */
}