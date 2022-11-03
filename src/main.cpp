#include "Arduino.h"

void Button(void);                         // read the button on the left joystick and if pushed toggle MA9136
bool analog2PWM(int whichADC, int dither); // convert the analog value from the ADC's(the joysticks) to the correct PWM duty cycle
int calculateDither(int dither);           // update the dither value(this value is added to the PWM dutycycle)

#define joyL A2   // FX1056
#define joyR A6    // FX1057
#define button 3  // SW3604
#define MA9136 0  // K1
#define PWM9145 5 // OUT2(U4,Q2)
#define PWM9146 6 // OUT3(U5,Q4)
#define PWM9123 4 // OUT1(U1,Q9)
#define MA9190 1  // K2

int ditherRange = 10; //dither range in percentage around the PWM dutycycle(so ditherRange = 10 & PWM = 50% => PWM = 45%-55%)

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

  // digitalWrite(MA9136,LOW);
  // digitalWrite(MA9190,LOW);
  // digitalWrite(PWM9123,LOW);
  // digitalWrite(PWM9145,LOW);
  // digitalWrite(PWM9146,LOW);
  // delay(1000);
  // digitalWrite(MA9136,HIGH);
  // digitalWrite(MA9190,HIGH);
  // digitalWrite(PWM9123,HIGH);
  // digitalWrite(PWM9145,HIGH);
  // digitalWrite(PWM9146,HIGH);
  // delay(1000);

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

int calculateDither(int dither){
    if(dither<=ditherRange)
    {
        dither++;
    }
    return dither;
}

void Button(void)
{
    static bool MA9136State = false; // give the MA9136 the initial state of OFF

    if (!(digitalRead(button))) // if the button is LOW(pushed)
    {
        MA9136State = !MA9136State;        // update the state to new value
        digitalWrite(MA9136, MA9136State); // write the updated state to MA9136
    }
}

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