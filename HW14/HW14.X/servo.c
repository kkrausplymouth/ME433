#include "servo.h"

#include <xc.h>

void servoSetupB15(){

    RPB15Rbits.RPB15R = 0b0101; // Set B15 to OC1
    T2CONbits.TCKPS = 4;      // Timer2 prescaler N=4 (1:16)
    PR2 = 59999;              // period = (PR2+1) * N * (1/48000000) = 50 kHz
    TMR2 = 0;                 // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110;   // PWM mode without fault pin; other OC1CON bits are defaults
    OC1CONbits.OCTSEL = 0;    // Use timer2
    OC1RS = 45000;                // duty cycle = OC1RS/(PR2+1) = 75%
    OC1R = 45000;                 // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;         // turn on Timer2
    OC1CONbits.ON = 1;        // turn on OC1
}

void servoSetAngle(int angle){
    if (angle > 180){
        angle = 180;
    } else if (angle < 0){
        angle = 0;
    }

    int dc = ((STARTRANGE + (((float) angle)/180)*(ENDRANGE - STARTRANGE))/20)*59999;
    OC1RS = dc;

}