#include "nu32dip.h" // constants, functions for startup and UART
#include <math.h>

#define ARYLENGTH 100

void blink(int, int); // blink the LEDs function

void delay(float seconds){
  int time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT()-time < seconds*24000000){}
}

void generate_sine_wave(double* ary, int len, float amp){
    for (int i=0; i<len; i++) {
        ary[i] = (double) amp*sin((double) ((double) i/len)*2*3.14159265);
    }
}

int main(void) {
  char message[100];
  double sinewave[ARYLENGTH];
  generate_sine_wave(sinewave, ARYLENGTH, 3.3);
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  
  while (1) {
      if (NU32DIP_USER == 0){
          for (int i=0; i<ARYLENGTH; i++){
              sprintf(message, "%lf\r\n", sinewave[i]);
              NU32DIP_WriteUART1(message);
              delay(.01);
          }
      }
  }
}

// blink the LEDs
void blink(int iterations, int time_ms){
	int i;
	unsigned int t;
	for (i=0; i< iterations; i++){
		NU32DIP_GREEN = 0; // on
		NU32DIP_YELLOW = 1; // off
		t = _CP0_GET_COUNT(); // should really check for overflow here
		// the core timer ticks at half the SYSCLK, so 24000000 times per second
		// so each millisecond is 24000 ticks
		// wait half in each delay
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
		
		NU32DIP_GREEN = 1; // off
		NU32DIP_YELLOW = 0; // on
		t = _CP0_GET_COUNT(); // should really check for overflow here
		while(_CP0_GET_COUNT() < t + 12000*time_ms){}
	}
}
		
