/*
 *
 * File name:	Project3.h
 * Author:		Richard W. Wall (C)opyright 2011
 * Rev Date:	Aug. 1, 2012
 * Rev Date:	Aug, 28, 2013
 *
****************************************/

#ifndef __PROJECT_3_H__
    #define __PROJECT_3_H__
#endif

//Define variables used
#define COUNTS_PER_MS 8890   // Initial guess

//Enum definitions!
enum stepState{s0_5 = 0x0A, s1 = 0x08, s1_5 = 0x09, s2 = 0x01, s2_5 = 0x05, s3 = 0x04, s3_5 = 0x06, s4 = 0x02};
enum direction{CW = 1, CCW = 2};
enum mode{FS = 1, HS = 2};

/* Function prototypes */
void system_init(void);
void sw_msDelay(unsigned int);
unsigned int read_buttons(void);
void decode_buttons(unsigned int, unsigned int *, unsigned int *, unsigned int *);
unsigned int stepperStateMachine(unsigned int, unsigned int, unsigned int);
void outputToStepper(unsigned int);

/* End of Project2.h */
