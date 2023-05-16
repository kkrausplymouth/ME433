#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "draw.h"
#include "font.h"
#include "IOextend.h"
#include "ws2812b.h"
#include <stdio.h>
#include <string.h>

#define NUM_RGBS 8

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
    ws2812b_setup(); //rgb lights setup

    
    int initial_colors[8] = {30, 60, 90, 120, 150, 180, 210, 240};
    //int update_colors[8] = {10, 3, 8, 6, 1, 2, 6, 4}; //random
    int update_colors[8] = {1, 1, 1, 1, 1, 1, 1, 1}; //rainbow
    int cur_colors[8] = {30, 60, 90, 120, 150, 180, 210, 240};
    
    wsColor c[NUM_RGBS];
    for (int i=0; i<NUM_RGBS; i++){
        c[i] = HSBtoRGB(initial_colors[i], 1, .3);
    }
    


    int i = 0;
    while (1) {
        for (int j = 0; j<NUM_RGBS; j++){
            int hue = cur_colors[j] + update_colors[j];
            if (hue > 360){
                hue = hue - 360;
            }
            cur_colors[j] = hue;
            c[j] = HSBtoRGB(hue, 1, .5);
        }
        ws2812b_setColor(c, NUM_RGBS);
        delay(.01);
        //sprintf(m, "%d", i);
        //drawString(m, 70, 15, 1);
        i += 1;
        if (i == 360){
            i = 0;
        }
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

