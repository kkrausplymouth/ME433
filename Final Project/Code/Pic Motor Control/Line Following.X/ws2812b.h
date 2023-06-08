#ifndef WS2812B_H__
#define WS2812B_H__

#include<xc.h> // processor SFR definitions
#ifndef NU32DIP__H__
#include "nu32dip.h"
#endif

#define RGBCONTROL LATBbits.LATB7
#define NUM_RGBS 8

// link three 8bit colors together
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} wsColor; 

void ws2812b_setup();
void ws2812b_setColor(wsColor*,int);
wsColor HSBtoRGB(float hue, float sat, float brightness);
void rainbow();

#endif