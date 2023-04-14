#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>

#define ARYLENGTH 100

void blink(int, int); // blink the LEDs function

void delay(float seconds)
{
    int time = _CP0_GET_COUNT();
    while (_CP0_GET_COUNT() - time < seconds * 24000000)
    {
    }
}

int main(void)
{
    char message[100];
    // call init functions
    NU32DIP_Startup();

    // pre calculate all voltages
    while (1)
    {
        // calc voltage for sine wave



        // calc voltage for triangle wave
        // send voltage with spi

        // delay
    }
}

//function to send
void fn(float v, char a_or_b){
    //[a_or_b 1 1 1 [10 bit voltage] 0 0]
    unsigned short t = 0;
    t = 0b111 << 12;
    unsigned char a_or_b; // 0b0 or 0b1 for a or b
    t = t | (a_or_b << 15);
    // stick in the desired voltage shifted by 2

    // send voltage with spi
    CS = 0;
    spi_io(t>>8);
    spi_io(t)
    CS = 1;
}