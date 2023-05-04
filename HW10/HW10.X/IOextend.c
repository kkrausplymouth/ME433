#include "IOextend.h"


void chip_setup(unsigned char io){
    i2c_master_start();
    i2c_master_send(0b01000000);
    i2c_master_send(0x00);
    i2c_master_send(io);
    i2c_master_stop();
}

int read_IO(void){
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
