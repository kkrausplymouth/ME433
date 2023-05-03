#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "draw.h"
#include "font.h"
#include "IOextend.h"
#include <stdio.h>
#include <string.h>


void blink(int, int); // blink the LEDs function

int main(void) {
    char m[100];
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

    
    //Accelerometer setup check
    unsigned char who; // read whoami
    who = whoami();
    sprintf(m, "%x\n\r", who);
    NU32DIP_WriteUART1(m); // print whoami
    if (who != 0x68){
        while(1){ // if whoami is not 0x68, stuck in loop with LEDs on
            NU32DIP_WriteUART1("Stuck!\r\n");
            blink(5, 100);
        }
    }
    
    char c[100];
    sprintf(c, "+Z Accel: ");
    drawString(c, 5, 5, 2);
    
    sprintf(c, "FPS: ");
    drawString(c, 5, 20, 2);
    
    
    unsigned char d[14]; // char array for the raw data
    float ax, ay, az, gx, gy, gz, t; // floats to store the data
    delay(1);
	// wait to print until you get a newline
    NU32DIP_YELLOW = 1;
    _CP0_SET_COUNT(0);
    while (1) {
		// use core timer for exactly 100Hz loop
        

        // read IMU
        burst_read_mpu6050(d);
		// convert data
        az = conv_zXL(d);
        // print the data
        sprintf(c, "%3.2f", az);
        drawString(c, 55, 5, 1);
        
        
        int FPS = 60/(((float) _CP0_GET_COUNT())/24000000);
        _CP0_SET_COUNT(0);
        sprintf(c, "%d", FPS);
        drawString(c, 30, 20, 1);
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

