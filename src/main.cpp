#include "LIMACH.h"

void setup()
{
  pinMode(joyL, INPUT);
  pinMode(joyR, INPUT);

  pinMode(MA9136, OUTPUT);
  pinMode(MA9190, OUTPUT);
  pinMode(PWM9123, OUTPUT);
  pinMode(PWM9145, OUTPUT);
  pinMode(PWM9146, OUTPUT);
}

void loop()
{
  unsigned long currentMillis = micros();

  if (currentMillis - previousMicrosPWM9123ON >= intervalPWM9123ON)
  {
  }

  if (currentMillis - previousMicrosPWM9123ON >= intervalPWM9123OFF)
  {
  }

  if (currentMillis - previousMicrosPWM9145_46ON >= intervalPWM9145_46ON)
  {
  }

  if (currentMillis - previousMicrosPWM9145_46ON >= intervalPWM9145_46OFF)
  {
  }

  if (currentMillis - previousMicrosJoyL >= intervalJoyL)
  {
  }

  if (currentMillis - previousMicrosDither >= intervalDither)
  {
  }

  if (currentMillis - previousMicrosJoyR >= intervalJoyR)
  {
  }

  if (currentMillis - previousMicrosButton >= intervalButton)
  {
  }
}