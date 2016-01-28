//
///*
// * File:   main.c
// * Author: JuanDesktop
// *
// * Created on January 5, 2016, 10:41 PM
// */
//
//
#include <xc.h>
#include <pic18f242.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <plib/pwm.h>

#define CENTERED_DUTY1 100
#define CENTERED_DUTY2 100
#define PERIOD 1779
#define DUTY1_DELTA 1
#define DUTY2_DELTA 1
#define DUTY1_MAX 100
#define DUTY2_MAX 100
#define DUTY1_MIN 0
#define DUTY2_MIN 0
#define HIGH (1)
#define LOW (0)

static unsigned int dutyCycle1Counter = 0;
static unsigned int periodCounter = 0;
static unsigned int dutyCycle2Counter = 0;
static unsigned int dutyCycle1 = CENTERED_DUTY1;
static unsigned int dutyCycle2 = CENTERED_DUTY2;
static bool pressReleaseFlag1 = 0;
static bool pressReleaseFlag2 = 0;
static bool pressReleaseFlag3 = 0;
static bool pressReleaseFlag4 = 0;
static bool pressReleaseFlagCenter = 0;
static bool pressReleaseFlagRelay = 0;

void ConfigureTimers(void)
{
/*1 us Timer0 interrupt*/
  T0CON	 = 0xC8;
  TMR0L	 = 0xFB;
  GIE	 = 1;
  TMR0IE	 = 1;
}

void InitializeOutputPins(void)
{
    TRISCbits.RC3 = 0 ;   //   configure RC3 as output
    TRISCbits.RC4 = 0 ;   //   configure RC4 as output
    TRISCbits.RC5 = 0 ;   //   configure RC5 as output
}

void IntializeInputPins(void)
{
    TRISBbits.RB0 = 1 ;   //   configure RB0 as input
    TRISBbits.RB1 = 1 ;   //   configure RB1 as input
    TRISBbits.RB2 = 1 ;   //   configure RB2 as input
    TRISBbits.RB3 = 1 ;   //   configure RB3 as input
    TRISBbits.RB4 = 1 ;   //   configure RB4 as input
    TRISBbits.RB5 = 1 ;   //   configure RB5 as input
    TRISBbits.RB6 = 1 ;   //   configure RB6 as input
    TRISBbits.RB7 = 1 ;   //   configure RB7 as input

    INTCON2bits.nRBPU = 0; //Set all B input ports to pull ups
}

void WriteMotor1Output(char output)
{
    RC3 = output;
}

void WriteMotor2Output(char output)
{
    RC4 = output;
}

void ToggleRelayOutput(void)
{
    RC5 ^= 1;
}

void main(void) {

    InitializeOutputPins();
    IntializeInputPins();
    ConfigureTimers();

    while(1)
    {
    }
    
    return;
}

void IncrementMotor1DutyCycle(void)
{
    if(dutyCycle1 < DUTY1_MAX)
    {
      dutyCycle1 += DUTY1_DELTA;
    }
}

void DecrementMotor1DutyCycle(void)
{
    if(dutyCycle1 > DUTY1_MIN)
    {
      dutyCycle1 -= DUTY1_DELTA;
    }
}

void IncrementMotor2DutyCycle(void)
{
    if(dutyCycle2 < DUTY2_MAX)
    {
      dutyCycle2 += DUTY2_DELTA;
    }
}

void DecrementMotor2DutyCycle(void)
{
    if(dutyCycle2 > DUTY2_MIN)
    {
      dutyCycle2 -= DUTY2_DELTA;
    }
}

void CenterMotors(void)
{
    dutyCycle1 = CENTERED_DUTY1;
    dutyCycle2 = CENTERED_DUTY2;
}

void MonitorMotor1IncrementButton(void)
{
    if((RB0 == 0) && (pressReleaseFlag1 == 0))
    {
      IncrementMotor1DutyCycle();
      pressReleaseFlag1 = 1;
    }
    else if((RB0 == 1) && (pressReleaseFlag1 == 1))
    {
       pressReleaseFlag1 = 0;
    }  
}

void MonitorMotor1DecrementButton(void)
{
    if((RB1 == 0) && (pressReleaseFlag2 == 0))
    {
      DecrementMotor1DutyCycle();
      pressReleaseFlag2 = 1;
    }
    else if((RB1 == 1) && (pressReleaseFlag2 == 1))
    {
       pressReleaseFlag2 = 0;
    }  
}

void MonitorMotor2IncrementButton(void)
{
    if((RB2 == 0) && (pressReleaseFlag3 == 0))
    {
      IncrementMotor2DutyCycle();
      pressReleaseFlag3 = 1;
    }
    else if((RB2 == 1) && (pressReleaseFlag3 == 1))
    {
       pressReleaseFlag3 = 0;
    }  
}

void MonitorMotor2DecrementButton(void)
{
    if((RB3 == 0) && (pressReleaseFlag4 == 0))
    {
      DecrementMotor2DutyCycle();
      pressReleaseFlag4 = 1;
    }
    else if((RB3 == 1) && (pressReleaseFlag4 == 1))
    {
       pressReleaseFlag4 = 0;
    }  
}

void MonitorCenteringButton(void)
{
    if((RB4 == 0) && (pressReleaseFlagCenter == 0))
    {
      CenterMotors();
      pressReleaseFlagCenter = 1;
    }
    else if((RB4 == 1) && (pressReleaseFlagCenter == 1))
    {
       pressReleaseFlagCenter = 0;
    }  
}

void MonitorMotor1Buttons(void)
{
    MonitorMotor1IncrementButton();
    MonitorMotor1DecrementButton();
}

void MonitorMotor2Buttons(void)
{
    MonitorMotor2IncrementButton();
    MonitorMotor2DecrementButton();
}

void MonitorRelayButton(void)
{
    if((RB5 == 0) && (pressReleaseFlagRelay == 0))
    {
      ToggleRelayOutput();
      pressReleaseFlagRelay = 1;
    }
    else if((RB5 == 1) && (pressReleaseFlagRelay == 1))
    {
       pressReleaseFlagRelay = 0;
    }  
}

void interrupt Interrupt(){
  
  MonitorMotor1Buttons();
  MonitorMotor2Buttons();
  MonitorCenteringButton();
  MonitorRelayButton();
    
  if (TMR0IF){ 
    if(dutyCycle1Counter == dutyCycle1)
    {
        WriteMotor1Output(LOW);
    }
    
    if(dutyCycle2Counter == dutyCycle2)
    {
        WriteMotor2Output(LOW);
    }
    
    if(periodCounter == PERIOD)
    {
        WriteMotor1Output(HIGH);
        WriteMotor2Output(HIGH);
        dutyCycle1Counter =0;
        dutyCycle2Counter =0;
        periodCounter=0;
    }
       dutyCycle1Counter++;
       dutyCycle2Counter++;
       periodCounter++;
   ToggleRelayOutput();
    TMR0IF = 0;
    TMR0L	 = 0xFB;
  }
} 

