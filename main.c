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
#include <htc.h>

#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 20        // Brown-out Reset Voltage bits (VBOR set to 2.0V)

#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

#pragma config CCP2MUX = ON     // CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)

#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)
#pragma config LVP = ON         // Low Voltage ICSP Enable bit (Low Voltage ICSP enabled)

#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)

#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)

#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

#define _XTAL_FREQ 20000000

#define CENTERED_DUTY1 200
#define CENTERED_DUTY2 200
#define PERIOD 2000
#define DUTY1_DELTA 40
#define DUTY2_DELTA 40
#define DUTY1_MAX 400
#define DUTY2_MAX 400
#define DUTY1_MIN 60
#define DUTY2_MIN 60
#define HIGH (1)
#define LOW (0)



static int dutyCycle1Counter = 0;
static int periodCounter = 0;
static int dutyCycle2Counter = 0;
static int dutyCycle1 = CENTERED_DUTY1;
static int dutyCycle2 = CENTERED_DUTY2;
static int pressReleaseFlag1 = 0;
static int pressReleaseFlag2 = 0;
static int pressReleaseFlag3 = 0;
static int pressReleaseFlag4 = 0;
static int pressReleaseFlagCenter = 0;
static int pressReleaseFlagRelay = 0;

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

char ReadInput(char inputNumber)
{
    switch(inputNumber)
    {
        case 1:
            return RA0;
            
        case 2:
            return RA1;
            
        default:
            break;
    }
}

void UpdateDutyCycle1(void)
{
    
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
        
        TMR0IF = 0;
        TMR0L	 = 0xFB;
    }
}