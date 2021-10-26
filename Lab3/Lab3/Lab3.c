#include <plib.h>

#include "CerebotMX7cK.h"
#include "Project3.h"


int main()
{
    
    system_init(); /* Setup system Hardware. */
    
    unsigned int btnInput;
    unsigned int delay;
    unsigned int mode;
    unsigned int direction;
    unsigned int stepState = 0x0A;
    
    while(1)
    {
        btnInput = read_buttons();
        decode_buttons(btnInput, &delay, &mode, &direction);
        stepState = stepperStateMachine(direction, stepState, mode);
        outputToStepper(stepState);
        
        sw_msDelay(delay);  //Wait!
    }
    return 0; /* Returning a value is expected but this statement
               * should never execute */
}

/* decode_buttons Function Description ***************************************
 * NOTES:           Refer to Cerebot MX7cK data sheet for button bit positions
 * END DESCRIPTION ************************************************************/
void decode_buttons(unsigned int buttons, unsigned int *delay, unsigned int *mode, unsigned int *direction)
{
    switch(buttons)
    {
        case 0://All of
            *direction = CW;
            *mode = FS;
            *delay = 40;
            break;
            
        case 1://BTN1 on, BTN2 off
            *direction = CW;
            *mode = HS;
            *delay = 20;
            break;
            
        case 2://BTN1 off, BTN2 on
            *direction = CCW;
            *mode = HS;
            *delay = 20;
            break;
            
        case 3://BTN1 on, BTN on
            *direction = CCW;
            *mode = FS;
            *delay = 40;
            break;
            
    }
}

/*
 This function takes a state of the stepper motor and moves it!
 */
void outputToStepper(unsigned int stepState)
{
    unsigned int x = stepState;

    x = PORTB;
    x = x & ~SM_COILS;
    x = x | (stepState<<7);
    LATB = x;
    
    LATBINV = LEDB; // Toggle LEDB for each new stepper movement and used 
                    //for instrumentation
}
/*
 stepperStatemachine function description:
 *  This function takes the current state of the stepper and the direction
 *  that the stepper motor needs to move in.
 */
unsigned int stepperStateMachine(unsigned int direction, unsigned int stepState, unsigned int mode)
{
    switch(stepState)
    {
        case s0_5:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s1_5;
                }
                else
                {
                    stepState = s1;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s3_5;
                }
                else
                {
                    stepState = s4;
                }
            }
            break;

        case s1:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s2;
                }
                else
                {
                    stepState = s1_5;
                }
            }
            else    //CCW
            {
                if(mode = FS)
                {
                    stepState = s4;
                }
                else
                {
                    stepState = s0_5;
                }
            }
            break;
        
        case s1_5:
            if (direction == CW)
            {
                if(mode ==FS)
                {
                    stepState = s2_5;
                }
                else
                {
                    stepState = s2;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s0_5;
                }
                else
                {
                    stepState = s1;
                }
            }
            break;
            
        case s2:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s3;
                }
                else
                {
                    stepState = s2_5;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s1;
                }
                else
                {
                    stepState = s1_5;
                }
            }
            break;
            
        case s2_5:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s3_5;
                }
                else
                {
                    stepState = s3;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s1_5;
                }
                else
                {
                    stepState = s2;
                }
            }
            break;
            
        case s3:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s4;
                }
                else
                {
                    stepState = s3_5;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s2;
                }
                else
                {
                    stepState = s2_5;
                }
            }
            break;
            
        case s3_5:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s0_5;
                }
                else
                {
                    stepState = s4;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s2_5;
                }
                else
                {
                    stepState = s3;
                }
            }
            break;
            
        case s4:
            if (direction == CW)
            {
                if(mode == FS)
                {
                    stepState = s1;
                }
                else
                {
                    stepState = s0_5;
                }
            }
            else    //CCW
            {
                if(mode == FS)
                {
                    stepState = s3;
                }
                else
                {
                    stepState = s3_5;
                }
            }
            break;
    return stepState;
    }
}

// LEDA is defined in CerebotMX7cK.h
/*
 This is a software delay function
 */
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

/* read_buttons Function Description *****************************************
 * SYNTAX:          int read_buttons(void);
 * KEYWORDS:        button, read, inputs
 * DESCRIPTION:     Reads the status of the input buttons.  Button status is
 *                  reported for button bit positions only. All other bits in
 *                  the returned value are set to zero as shown below:
 *
 *      Port G Bit position [15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0]
 *      Port G Bit value    [ 0| 0| 0| 0| 0| 0| 0| 0|B2|B1| 0| 0| 0| 0| 0| 0]
 *
 *      B1 will be 1 if BTN1 is pressed otherwise B1 will be zero
 *      B2 will be 1 if BTN2 is pressed otherwise B2 will be zero
 * END DESCRIPTION ************************************************************/
unsigned int read_buttons(void)
{
    unsigned int x = 0;
    x = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    x = x >> 6; //Shift bits over so the return value will be 0,1,2, or 3
    return x;
}