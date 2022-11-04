#include "Arduino.h"

void Button(void);                         // read the button on the left joystick and if pushed toggle MA9136
bool analog2PWM(int whichADC, int dither); // convert the analog value from the ADC's(the joysticks) to the correct PWM duty cycle
int calculateDither(int dither);           // update the dither value(this value is added to the PWM dutycycle)

#define joyL A2   // FX1056
#define joyR A6   // FX1057
#define button 3  // SW3604
#define MA9136 0  // K1
#define PWM9145 5 // OUT2(U4,Q2)
#define PWM9146 6 // OUT3(U5,Q4)
#define PWM9123 8 // OUT1(U1,Q9)
#define MA9190 1  // K2

int ditherRange = 20; // dither range in percentage around the PWM dutycycle(so ditherRange = 10 & PWM = 50% => PWM = 45%-55%)
int dutyCycle_4546 = 0;
int dutyCycle_9123 = 0;

void setup()
{
  pinMode(joyL, INPUT);
  pinMode(joyR, INPUT);

  pinMode(MA9136, OUTPUT);
  pinMode(MA9190, OUTPUT);
  pinMode(PWM9123, OUTPUT);
  pinMode(PWM9145, OUTPUT);
  pinMode(PWM9146, OUTPUT);

	TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(COM1B1);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR0A = _BV(COM0A1) | _BV(WGM00);
  TCCR0B = _BV(CS01);
}

void loop()
{
  static int dither = 0;
  static bool polarity9190 = 0;
  static bool polarity45_46 = 0;
  Button();
  dither = calculateDither(dither);
  polarity9190 = analog2PWM(joyL, dither);
  polarity45_46 = analog2PWM(joyR, dither);

  OCR0A = dutyCycle_9123;
  if (polarity9190)
  {
    digitalWrite(MA9190, HIGH);
  }
  else
  {
    digitalWrite(MA9190, LOW);
  }
  if (polarity45_46)
  {
    OCR1A = dutyCycle_4546;
    //OCR1B = 255;
  }
  else
  {
    OCR1B = dutyCycle_4546;
    //OCR1A = 255;
  }
}

int calculateDither(int dither)
{
  static bool increase = true;
  if (dither >= ditherRange)
  {
    increase = false;
  }
  if (dither <= 0)
  {
    increase = true;
  }

  if (increase)
  {
    dither++;
  }
  else
  {
    dither--;
  }
  return dither;
}

void Button(void)
{
  static bool MA9136State = false; // give the MA9136 the initial state of OFF
  static int loopsPassed = 0;      // count how many times this function is called while the button is still being pressed

  if (!(digitalRead(button))) // if the button is LOW(pushed)
  {
    loopsPassed++;
    if (loopsPassed == 10) // debounce (if the button has been pressed for n loops(the button is truly pressed)
    {
      MA9136State = !MA9136State;
      digitalWrite(MA9136, MA9136State);
    }
  }
  else
  {
    loopsPassed = 0; // if the button is not pressed reset the loop counter
  }
}

bool analog2PWM(int whichADC, int dither)
{
  const long maxValue = 511 - 10;
  const long minValue = 0 + 165;
  const long maxPWM = 255;
  const long minPWM = 0;
  long dutyCycle = 0;

  bool polarity = false;
  long analogValue = (long)analogRead(whichADC);
  if (analogValue >= 512)
  {
    polarity = true;
    analogValue = 511 - (analogValue - 512);
  }

  if (analogValue < minValue)
  {
    dutyCycle = minPWM;
  }
  else if (analogValue > maxValue)
  {
    dutyCycle = maxPWM;
  }
  else
  {
      dutyCycle = (((analogValue-minValue)*maxPWM)/(maxValue-minValue));

  //   // analogValue = analogValue - minValue;
  //   // double dutycyclePercentage = ((analogValue * 100) / 512); //(maxValue - minValue));
  //   // dutyCycle = ((dutycyclePercentage * maxPWM) / 100);

  //   //+ (ditherRange / 2) - dither)
  }
  switch (whichADC)
  {
  case joyL:
    dutyCycle_9123 = dutyCycle;
    break;
  case joyR:
    dutyCycle_4546 = dutyCycle;
    break;
  default:
    break;
  }
  return polarity;
}