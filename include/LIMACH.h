#pragma once

#include "Arduino.h"

void Button(void);
int readADC(bool whichADC, int dither);
int voltage2PWM(int voltage, int dither);
int calculateDither(int dither);
void writePWM9123(bool state);
void writePWM9145_46(bool state, bool output);