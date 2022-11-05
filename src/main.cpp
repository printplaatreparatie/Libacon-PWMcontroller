#include "Arduino.h"

void Button(void);                         // read the button on the left joystick and if pushed turn on MA9136
bool analog2PWM(int whichADC, int dither); // convert the analog value from the ADC's(the joysticks) to the correct PWM duty cycle
int calculateDither(int dither);           // update the dither value(this value is added to the PWM dutycycle)

#define joyL A2   // FX1056
#define joyR A7   // FX1057
#define button 3  // SW3604 (button on joyL)
#define MA9136 0  // K1 (turned on by button on joyL)
#define PWM9145 5 // OUT2(U4,Q2)
#define PWM9146 6 // OUT3(U5,Q4)
#define PWM9123 8 // OUT1(U1,Q9)
#define MA9190 1  // K2 (invert hydraulic flow from PWM9123)

int ditherRange = 50; // dither range around the PWM dutycycle(so ditherRange = 10 & PWM = 50 => PWM = 45-55)
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

  TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(COM1B1); //set PWM outputs from clock1(PWM9145&9146) in fast PWM mode
  TCCR1B = _BV(CS11) | _BV(WGM12);                 //and set clock prescaler zo that PWM freq= around 2KHz closest = 3.8KHz

  TCCR0A = _BV(COM0A1) | _BV(WGM00);               //do the same but for clock0 but only for outputA(PWM9123);
  TCCR0B = _BV(CS01);                              //this clock frequency is different therefore PWMfreq = 1.9KHz
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
    OCR1B = 255;
  }
  else
  {
    OCR1B = dutyCycle_4546;
    OCR1A = 255;
  }
}

int calculateDither(int dither)  //ads a triangle waveform to the dutycycle so the valves dont stick when position doesnt change
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
    dither = dither + 5; // dither in steps of 5 because dither freq= loopFreq/(ditherRange/steps) //loopFreq=1.3KHz ditherRange=50
  }
  else
  {
    dither = dither - 5;
  }
  return dither;
}

void Button(void)
{
  static bool MA9136State = false; // give the MA9136 the initial state of OFF
  static int loopsPassed = 0;      // stores how many times this function is called while the button is still being pressed
  if (!MA9136State)
  {
    if (!(digitalRead(button))) // if the button is LOW(pushed)
    {
      loopsPassed++;
      if (loopsPassed == 10) // debounce (if the button has been pressed for n loops(the button is truly pressed)
      {
        MA9136State = true;
        digitalWrite(MA9136, MA9136State);
      }
    }
    else
    {
      loopsPassed = 0; // if the button is not pressed reset the loop counter
    }
  }
}

bool analog2PWM(int whichADC, int dither)  //for more clarity see pictures on trello(libacon volvo)
{
  const long maxValue = 511 - 15; //the joystick doesnt always rest in the same mid position so this is to create a deadzone in which the ADC value is ignored and PWM255 is written
  const long minValue = 0 + 165;  //when the joysticj is fully turned to one side it outputs 0.7V or 4.3 so another deadzone of 0.7V/0.00488V
  const long maxPWM = 255;
  const long minPWM = 0;
  long dutyCycle = 0;

  bool polarity = false;
  long analogValue = (long)analogRead(whichADC);
  if (analogValue >= 512)
  {
    polarity = true;
    analogValue = 511 - (analogValue - 512); //map the value so that instead of 0-1023 it goes from 0-512-0
  }

  if (analogValue < minValue) //ignore the deadzone and just write minPWM
  {
    dutyCycle = minPWM;
  }
  else if (analogValue > maxValue) //ignore the deadzone and just write maxPWM
  {
    dutyCycle = maxPWM;
  }
  else//now the ADCvalue is filtered so that it only can be 165(minvalue)-496(maxvalue)
  {
    //the next formula maps the value 165-496 to a PWM value of 0-255
    //dutycycle = (value/totalvalue*maxPWM)+dithervalue
    dutyCycle = ((((analogValue - minValue) * maxPWM) / (maxValue - minValue))) + ((ditherRange / 2) - dither);
    //because dither gets added and substracted after the value can get out of limit and overflow so check if that happens and limit the value
    if (dutyCycle > 255)
    {
      dutyCycle = 255;
    }
    if (dutyCycle < 0)
    {
      dutyCycle = 0;
    }
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