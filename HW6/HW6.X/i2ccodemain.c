#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"


void chip_setup(unsigned char io);
void toggle_LED(unsigned char pin, unsigned char state);
int read_IO();

int main(){
    
    char m[100];
    NU32DIP_Startup();
    NU32DIP_GREEN = 1;
    i2c_master_setup();
    // init the i2c pins
    
    // init the chip, GP0 as output, GP7 as input
    unsigned char io = 0b11111110;
    chip_setup(io);
    NU32DIP_GREEN = 0;
    delay(2);
    
    int LED_state = 0;
    toggle_LED(0, !LED_state);
    while(1){
        
        unsigned char c;
        c = read_IO();
        c = !((c>>7)&1);
        if (c){
            delay(.1);
            c = read_IO();
            c = !((c>>7)&1);
            if (c){
                toggle_LED(0, LED_state);
                LED_state = !LED_state;
                delay(.25);
            }
        } 
    }
}

void turnOnGP7(){
    // send start bit (function)

    // send the address of the chip
        // 7 bit address 0b0100(A2)(A1)(A0)(R/W)
            // 0b01000000 (for write)
            // 0b01000001 (for read)

    // send the register name
        // 0x0A for OLAT 
        // 0x0A for OLAT 

    // send the value to turn on GP7
        // (0b10000000) (turn pin on)
        // (0b10000000) (turn pin off)

    // send stop bit (function)
}

void generate_i2c_write(){
    // send start bit
    // send the address of the chip
    // etc.
}

void chip_setup(unsigned char io){
    i2c_master_start();
    i2c_master_send(0b01000000);
    i2c_master_send(0x00);
    i2c_master_send(io);
    i2c_master_stop();
}

int read_IO(){
    unsigned char answer;
    i2c_master_start();
    i2c_master_send(0b01000000); // hardware address and write bit
    i2c_master_send(0x09);  
    i2c_master_restart(); // this line is REALLY important!
    i2c_master_send(0b01000001); // hardware address and read bit
    answer = i2c_master_recv(); // receive a byte from the slave. Should be 0x69 = 105d
    i2c_master_ack(1); // send NACK to slave
    i2c_master_stop();
    return answer;
}

void toggle_LED(unsigned char IO_pin, unsigned char state){
    unsigned char c = state<<IO_pin;
    i2c_master_start();
    i2c_master_send(0b01000000);
    i2c_master_send(0x0A);
    i2c_master_send(c);
    i2c_master_stop();
}

i2c_master_start();
i2c_master_send(0b01000000);
i2c_master_send(0x0A);
i2c_master_send(0b00000001);
i2c_master_stop();