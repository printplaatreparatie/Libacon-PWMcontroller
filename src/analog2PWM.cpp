#include "LIMACH.h"

bool analog2PWM(int whichADC, int dither)
{
    const int error = 10;
    const int maxValue = 512 - error;
    const int minValue = 0 + error;
    int delayToTurnOff = intervalPWM;
    bool polarity = false;
    int analogValue = analogRead(whichADC);
    if (analogValue >= 512)
    {
        polarity = true;
        analogValue = 512 - (analogValue - 512);
    }

    if (analogValue < minValue)
    {
        delayToTurnOff = 0;
    }
    else if (analogValue > maxValue)
    {
        delayToTurnOff = intervalPWM;
    }
    else
    {
        analogValue = analogValue - minValue;
        int dutycycle = (((analogValue * 100) / maxValue) + (ditherRange/2)- dither);
        delayToTurnOff = intervalPWM * (dutycycle / 100);
    }

    switch (whichADC)
    {
    case joyL:
        intervalPWM9123OFF = delayToTurnOff;
        break;

    case joyR:
        intervalPWM9145_46OFF = delayToTurnOff;
        break;

    default:
        break;
    }

    return polarity;
}