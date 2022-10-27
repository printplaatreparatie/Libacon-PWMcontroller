#pragma once

#include "Arduino.h"

void Button(void);                         // read the button on the left joystick and if pushed toggle MA9136
bool analog2PWM(int whichADC, int dither); // convert the analog value from the ADC's(the joysticks) to the correct PWM duty cycle
int calculateDither(int dither);           // update the dither value(this value is added to the PWM dutycycle)

#define joyL 10   // FX1056
#define joyR 9    // FX1057
#define button 3  // SW3604
#define MA9136 0  // K1
#define PWM9145 5 // OUT2(U4,Q2)
#define PWM9146 6 // OUT3(U5,Q4)
#define PWM9123 4 // OUT1(U1,Q9)
#define MA9190 1  // K2

unsigned long previousMicrosJoyL = 0;
const long intervalJoyL = 500;

unsigned long previousMicrosJoyR = 0;
const long intervalJoyR = 500;

unsigned long previousMicrosDither = 0;
const long intervalDither = 5000;

unsigned long previousMicrosButton = 0;
const long intervalButton = 10000;

const long intervalPWM = 250;
unsigned long previousMicrosPWM9123ON = 0;
unsigned long previousMicrosPWM9145_46ON = 0;

long intervalPWM9123OFF = intervalPWM;

long intervalPWM9145_46OFF = intervalPWM;