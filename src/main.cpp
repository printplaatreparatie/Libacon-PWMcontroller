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
  static int dither = 0;
  static bool polarity9190 = 0;
  static bool polarity45_46 = 0;
  unsigned long currentMicros = micros();

  if (currentMicros - previousMicrosPWM9123ON >= intervalPWM)
  {
    digitalWrite(PWM9123, HIGH);
    if (polarity9190)
    {
      digitalWrite(MA9190, HIGH);
    }
    else
    {
      digitalWrite(MA9190, LOW);
    }
  }

  if (currentMicros - previousMicrosPWM9145_46ON >= intervalPWM)
  {
    digitalWrite(PWM9145, HIGH);
    digitalWrite(PWM9146, HIGH);
  }

  if (currentMicros - previousMicrosPWM9123ON >= intervalPWM9123OFF)
  {
    digitalWrite(PWM9123, LOW);
  }

  if (currentMicros - previousMicrosPWM9145_46ON >= intervalPWM9145_46OFF)
  {
    if (polarity45_46)
    {
      digitalWrite(PWM9145, LOW);
    }
    else
    {
      digitalWrite(PWM9146, LOW);
    }
  }

  if (currentMicros - previousMicrosDither >= intervalDither)
  {
    dither = calculateDither(dither);
  }

  if (currentMicros - previousMicrosJoyL >= intervalJoyL)
  {
    polarity45_46 = analog2PWM(joyL, dither);
  }

  if (currentMicros - previousMicrosJoyR >= intervalJoyR)
  {
    polarity9190 = analog2PWM(joyR, dither);
  }

  if (currentMicros - previousMicrosButton >= intervalButton)
  {
    Button();
  }
}