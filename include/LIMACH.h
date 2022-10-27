#pragma once

#include "Arduino.h"

void Button(void);
int readADC(bool whichADC, int dither);
int voltage2PWM(int voltage, int dither);
int calculateDither(int dither);
void writePWM9123(bool state);
void writePWM9145_46(bool state, bool output);

#define joyL 10    //FX1056
#define joyR 9     //FX1057
#define button 3   //SW3604
#define MA9136 0   //K1
#define PWM9145 5  //OUT2(U4,Q2)
#define PWM9146 6  //OUT3(U5,Q4)
#define PWM9123 4  //OUT1(U1,Q9)
#define MA9190 1   //K2

unsigned long previousMicrosJoyL = 0;
const long intervalJoyL = 500;

unsigned long previousMicrosJoyR = 0;
const long intervalJoyR = 500;

unsigned long previousMicrosDither = 0;
const long intervalDither = 5000;

unsigned long previousMicrosButton = 0;
const long intervalButton = 10000;

unsigned long previousMicrosPWM9123ON = 0;
const long intervalPWM9123ON = 250;

long intervalPWM9123OFF = 125;

unsigned long previousMicrosPWM9145_46ON = 0;
const long intervalPWM9145_46ON = 250;

long intervalPWM9145_46OFF = 125;