#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "draw.h"
#include "font.h"
#include "IOextend.h"
#include "ws2812b.h"
#include "servo.h"
#include "dcMotorControl.h"
#include "picoCam.h"
#include <stdio.h>
#include <string.h>




void blink(int, int); // blink the LEDs function

void setupAll();

int main(void) {

    //initialization
    NU32DIP_GREEN = 1;
    NU32DIP_YELLOW = 0;
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    i2c_master_setup(); //initialize i2c
    ws2812b_setup(); //rgb lights setup
    servoSetupB(0, 3, 3); //set up servo motor
    setupMotors();
    setupPicoCamera();
    NU32DIP_GREEN = 0;
    NU32DIP_YELLOW = 1;
    
    //Variables we need
    char m[100];


    servoSetAngle(135, 3);
    setMotors(50, 50);
    
    int L;
    while(1){
        NU32DIP_YELLOW = 0;
        L = getLineValue();
        if (L < 0){
            L = 0;
        }
        if (L > 60){
            L = 60;
        }

        sprintf(m,"%d\r\n",L);
        NU32DIP_WriteUART1(m);
    }
}

// blink the LEDs
void blink(int iterations, int time_ms) {
    int i;
    unsigned int t;
    for (i = 0; i < iterations; i++) {
        NU32DIP_GREEN = !NU32DIP_GREEN; // on
        NU32DIP_YELLOW = !NU32DIP_YELLOW; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        // the core timer ticks at half the SYSCLK, so 24000000 times per second
        // so each millisecond is 24000 ticks
        // wait half in each delay
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }

        NU32DIP_GREEN = !NU32DIP_GREEN; // on
        NU32DIP_YELLOW = !NU32DIP_YELLOW; // off
        t = _CP0_GET_COUNT(); // should really check for overflow here
        while (_CP0_GET_COUNT() < t + 12000 * time_ms) {
        }
    }
}

void setupAll(){
    NU32DIP_GREEN = 0;
    NU32DIP_YELLOW = 0;
    delay(1);
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    i2c_master_setup();
    init_mpu6050();
    ssd1306_setup();
    ssd1306_clear(); 
    chip_setup(0b11111110); //setup IO extender
    toggle_LED(0, 1); //turn off LED
    ws2812b_setup(); //rgb lights setup
    servoSetupB(0, 3, 3);
    NU32DIP_GREEN = 0;
    NU32DIP_YELLOW = 1;
}

