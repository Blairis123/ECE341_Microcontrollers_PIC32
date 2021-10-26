#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project5.h"

unsigned int delay = 1;
unsigned int mode = FS;
unsigned int direction = CW;
unsigned int stepState = 0x0A;
int stepTimer = 1;


int main()
{
    
    system_init(); /* Setup system Hardware. */
    
    while(1)
    {
        
    }
    return 0; /* Returning a value is expected but this statement
               * should never execute */
}
//T1IF!!
void __ISR(_TIMER_1_VECTOR, IPL2) Timer1Handler(void)
{
    int testVar = 0;
    stepTimer--;
    
    if(stepTimer <= 0)
    {
        testVar=1;  //Used for testing
        stepState = stepperStateMachine(direction, stepState, mode);
        outputToStepper(stepState);
        
        stepTimer = delay;//If buttons don't change keep at same speed (delay)
    }
    LATBINV = LEDA; //toggle LED A
    mT1ClearIntFlag();
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1) CNIntHandler(void)
{
    LATBINV = LEDC;
    hw_msDelay(20);
    int btnInput = read_buttons();  //Read and assign value for later use
    
    decode_buttons(btnInput, &delay, &mode, &direction);
    
    /* Required to clear the interrupt flag in the ISR */
    mCNClearIntFlag(); // Macro function
    LATBINV = LEDC;
}

/* decode_buttons Function Description ***************************************
 * NOTES:           Refer to Cerebot MX7cK data sheet for button bit positions
 *             The delay determines the speed at which the stepper motor
 *              rotates at! This information is decoded from user input within
 *              this function.
 * 
 *  CHANGED FROM LAB 4!! Got rid of pointers because using global vars
 * END DESCRIPTION ***********************************************************/
void decode_buttons(unsigned int buttons, unsigned int *delay, unsigned int *mode, unsigned int *direction)
{
    switch(buttons)
    {
        case 0://All of
            *direction = CW;
            *mode = HS;
            *delay = 20;
            break;
            
        case 1://BTN1 on, BTN2 off
            *direction = CW;
            *mode = FS;
            *delay = 40;
            break;
            
        case 2://BTN1 off, BTN2 on
            *direction = CCW;
            *mode = HS;
            *delay = 30;
            break;
            
        case 3://BTN1 on, BTN on
            *direction = CCW;
            *mode = FS;
            *delay = 24;
            break;
            
    }
}

/*
 This function takes a state of the stepper motor and moves it!
 */
void outputToStepper(unsigned int stepState)
{
    LATBINV = LEDB;
    unsigned int x = stepState;

    x = PORTB;
    x = x & ~SM_COILS;
    x = x | (stepState<<7);
    LATB = x;
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

void timer1_interrupt_initialize(void)
{
    //configure Timer 1 with internal clock, 1:1 prescale, PR1 for 1 ms period
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, T1_INTR_RATE-1);
    // set up the timer interrupt with a priority of 2, sub priority 0
    mT1SetIntPriority(2); // Group priority range: 1 to 7
    mT1SetIntSubPriority(0); // Subgroup priority range: 0 to 3
    mT1IntEnable(1); // Enable T1 interrupts
    // Global interrupts must enabled to complete the initialization.
}
/* Initialization of CN peripheral for interrupt level 1 */
void cn_interrupt_initialize(void) // Code that is executed only once
{
    unsigned int dummy; // used to hold PORT read value

    // Enable CN for BTN1 and BTN2
    mCNOpen(CN_ON,(CN8_ENABLE | CN9_ENABLE), 0);
    
    // Set CN interrupts priority level 1 sub priority level 0
    mCNSetIntPriority(1); // Group priority (1 to 7)
    mCNSetIntSubPriority(0); // Subgroup priority (0 to 3)
    
    // read port to clear difference
    dummy = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    
    mCNClearIntFlag(); // Clear CN interrupt flag
    mCNIntEnable(1); // Enable CN interrupts
    // Global interrupts must enabled to complete the initialization.
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
    
    timer1_interrupt_initialize();
    
    cn_interrupt_initialize();
    
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();
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
int read_buttons(void)
{
    unsigned int x = 0;
    x = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    x = x >> 6; //Shift bits over so the return value will be 0,1,2, or 3
    return x;
}

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
}