#include "nu32dip.h" // constants, functions for startup and UART

void blink(int, int); // blink the LEDs function

int main(void) {
  char message[100];
  int iterations = 0, time_ms = 0;
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  while (1) {
    NU32DIP_ReadUART1(message, 100); // wait here until get message from computer
    sscanf(message, "%d\n", &iterations);
    sprintf(message, "Times: %d\r\n", iterations);
    NU32DIP_WriteUART1(message);
    
    
    NU32DIP_ReadUART1(message, 100); // wait here until get message from computer
    sscanf(message, "%d\n", &time_ms);
    sprintf(message, "ms per bilnk: %d\r\n", time_ms);
    NU32DIP_WriteUART1(message);
    
    NU32DIP_WriteUART1("Blinking\r\n");
    blink(iterations, time_ms); // 5 times, 500ms each time
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
		
