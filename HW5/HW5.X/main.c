#include "nu32dip.h" // constants, functions for startup and UART
#include "spi.h"
#include <math.h>

#define ARYLENGTH 100
#define PI 3.14159265358979323846264338327950


void delay(float seconds)
{
    int time = _CP0_GET_COUNT();
    while ((_CP0_GET_COUNT() - time) < (seconds * 24000000))
    {
    }
}

int main(void)
{
    char message[100];

    //initialize pic and spi
    NU32DIP_Startup();
    initSPI();


    //generate waves
    int sinewave[1024];
    int triwave[1024];
    for (int i = 0; i < 1024; i++) {
        sinewave[i] = (1024/2)*(sin((i * 2 * PI)/(1023/2))) + 1024/2;
        if (i < 1024 / 2) {
            triwave[i] = 2*i;
        } else {
            triwave[i] = (1023*2) - (i * 2);
        }
    } 
    triwave[1023] = 0;
    sinewave[1023] = 1024/2;
    triwave[0] = 0;
    sinewave[0] = 1024/2;
    
    int i = 1, time;
    while (1)
    {
        time = _CP0_GET_COUNT();
        int cursin = sinewave[i];
        int curtri = triwave[i];

        unsigned short sendsin = (0b0111<<12)|(cursin<<2);
        unsigned short sendtri = (0b1111<<12)|(curtri<<2);

        CS = 0;
        spi_io(sendsin>>8);
        spi_io(sendsin);
        CS = 1;
        
        CS = 0;
        spi_io(sendtri>>8);
        spi_io(sendtri);
        CS = 1;

        if (i >= 1023) {
            i = 0;
        } else {
            i+= 2;
        }
 
        while ((_CP0_GET_COUNT() - time) <= 24000000/(1023/2)){
            ;
        }
        
    }
}