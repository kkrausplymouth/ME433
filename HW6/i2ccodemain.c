

int main(){
    // nu32startup
    // init the i2c pins
    // init the chip, GP0 as input, gp7 as output

    while(1){
        // blink the yellow LED

        //blink GP7
            //turn on GP7
            //delay
            //Turn off GP7
            //delay
                //delete all this after debugging

        //r = readfromGP0
        if (r){
            //turn on gp7
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

int read_from_GP0(){
    // send start bit
    // send address with write bit
    // send register you want to read from
    // restart
    // send address with read bit
    unsigned char f = recv();
    // send the ACK bit
    // send the stop bit
    return f&0b00000001;
}