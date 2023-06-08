#include "servo.h"

#include <xc.h>

void servoSetupB(int pin, int timerNum, int OCNum){

    TRISBSET = 1<<pin;
    LATBSET = 1<<pin;

    if (pin == 0){
        RPB0Rbits.RPB0R = 0b0101; // Set B0 to OC1
    }

    if (timerNum == 3){
        T3CONbits.TCKPS = 4;      // Timer3 prescaler N=4 (1:16)
        PR3 = 59999;              // period = (PR2+1) * N * (1/48000000) = 50 kHz
        TMR3 = 0;                 // initial TMR3 count is 0

        T3CONbits.ON = 1;         // turn on Timer3
    }

    if (OCNum == 3){
        OC3CONbits.OCM = 0b110;   // PWM mode without fault pin; other OC3CON bits are defaults
        OC3CONbits.OCTSEL = 1;    // Use timer3  CHECK THIS <-<-<-<-<-
        OC3RS = 45000;                // duty cycle = OC3RS/(PR3+1) = 75%
        OC3R = 45000;                 // initialize before turning OC3 on; afterward it is read-only
        OC3CONbits.ON = 1;        // turn on OC3
    }



}

void servoSetAngle(int angle, int OCNum){
    if (angle > 180){
        angle = 180;
    } else if (angle < 0){
        angle = 0;
    }
    int dc = ((STARTRANGE + (((float) angle)/180)*(ENDRANGE - STARTRANGE))/20)*59999;

    if (OCNum == 3){
        OC1RS = dc;
    }


}