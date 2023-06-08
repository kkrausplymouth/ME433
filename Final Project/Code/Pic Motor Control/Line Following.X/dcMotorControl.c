#include "dcMotorControl.h"
#include <xc.h>


void setupMotors(){

    //Timer Setup
    T2CONbits.TCKPS = 1;      // Timer2 prescaler N=1 (1:1)
    PR2 = 2399;              // period = (PR4+1) * N * (1/48000000) = 50 kHz
    TMR2 = 0;                 // initial TMR2 count is 0
    T2CONbits.ON = 1;         // turn on Timer2


    //motor1 (Right):

    //PWM Output
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;

    //Direction Bit
    TRISBbits.TRISB15 = 0;
    LATBbits.LATB15 = 0;

    //OC1 Setup
    RPA0Rbits.RPA0R = 0b0101; // Set A0 to OC1
    OC1CONbits.OCM = 0b110;   // PWM mode without fault pin; other OC3CON bits are defaults
    OC1CONbits.OCTSEL = 0;    // Use timer2  CHECK THIS <-<-<-<-<-
    OC1RS = 0;                // duty cycle = OC3RS/(PR2+1) = 75%
    OC1R = 0;                 // initialize before turning OC2 on; afterward it is read-only
    OC1CONbits.ON = 1;        // turn on OC1


    //motor2 (left):

    //PWM Output
    TRISBbits.TRISB1 = 0;
    LATBbits.LATB1 = 0;

    //Direction Bit
    TRISBbits.TRISB14 = 0;
    LATBbits.LATB14 = 0;

    //OC2 Setup
    RPB1Rbits.RPB1R = 0b0101; // Set B1 to OC2
    OC2CONbits.OCM = 0b110;   // PWM mode without fault pin; other OC3CON bits are defaults
    OC2CONbits.OCTSEL = 0;    // Use timer2  CHECK THIS <-<-<-<-<-
    OC2RS = 0;                // duty cycle = OC3RS/(PR2+1) = 75%
    OC2R = 0;                 // initialize before turning OC2 on; afterward it is read-only
    OC2CONbits.ON = 1;        // turn on OC2

    
}

//Set speeds of left and right motors as a percentage of full speed; negative for reverse
void setMotors(int Left_speed, int Right_speed){

    //MAY HAVE TO FLIP WHICH MOTOR IS WHICH

    //Left Motor:

    if (Left_speed >= 100){
        Left_speed = 99;
    } else if (Left_speed <= -100){
        Left_speed = -99;
    }

    if (Left_speed >= 0){
        OC1RS = 24*Left_speed;
        LATBbits.LATB15 = 0;
    } else {
        OC1RS = -24*Left_speed;
        LATBbits.LATB15 = 1;
    }

    //Right Motor:

    if (Right_speed >= 100){
        Right_speed = 99;
    } else if (Right_speed <= -100){
        Right_speed = -99;
    }

    if (Right_speed >= 0){
        OC2RS = 24*Right_speed;
        LATBbits.LATB14 = 1;
    } else {
        OC2RS = -24*Right_speed;
        LATBbits.LATB14 = 0;
    }

}

//set robot speed (0-100), turning speed (0-100), and direction to turn (0 - left, 1 - right)
void setRobotControl(int speed, int turnDirection, float turnSpeed){
    int right = speed;
    int left = speed;

    if (turnDirection == 0){
        left -= turnSpeed/2;
        right += turnSpeed/2;
    } else {
        left += turnSpeed/2;
        right -= turnSpeed/2;
    }

    setMotors(left, right);

}

void calculateControl(int location, float sd){
    int speed = 75 - sd*CONTROL_SCALER;
    int turnDir, turnspeed;

    if (location < 30){
        turnDir = 1; //turn right
        turnspeed = (30 - location)*TURN_SCALER;
    } else if (location >= 30){
        turnDir = 0;
        turnspeed = (location - 30)*TURN_SCALER;
    }

    setRobotControl(speed, turnDir, turnspeed);
    




}