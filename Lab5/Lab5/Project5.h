/*
 *
 * File name:	Project3.h
 * Author:		Richard W. Wall (C)opyright 2011
 * Rev Date:	Aug. 1, 2012
 * Rev Date:	Aug, 28, 2013
 *
****************************************/

#ifndef __PROJECT_5_H__
    #define __PROJECT_5_H__
#endif

//Define variables used
#define T1_PRESCALE 1
#define TOGGLES_PER_SEC 1000
#define T1_TICK (FPB/T1_PRESCALE/TOGGLES_PER_SEC)
#define T1_INTR_RATE 10000 // For 1 ms interrupt rate with T1 clock=10E6

//Enum definitions!
enum stepState{s0_5 = 0x0A, s1 = 0x08, s1_5 = 0x09, s2 = 0x01, s2_5 = 0x05, s3 = 0x04, s3_5 = 0x06, s4 = 0x02};
enum direction{CW = 1, CCW = 2};
enum mode{FS = 1, HS = 2};

/* Function prototypes */
void system_init(void);
int read_buttons(void);
void decode_buttons(unsigned int, unsigned int *, unsigned int *, unsigned int *);
unsigned int stepperStateMachine(unsigned int, unsigned int , unsigned int);
void outputToStepper(unsigned int);
void hw_msDelay(unsigned int);

/* End of Project4.h */
