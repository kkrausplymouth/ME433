#include "picoCam.h"
#include <xc.h>

void setupPicoCamera(){

    UART2_Startup();
}

int getLineValue(){
    // in while(1)
    int com = 0;
    // uart2_flag() is 1 when uart2 has rx a message and sprintf'd it into a value
    while(!get_uart2_flag()){};
    set_uart2_flag(0); // set the flag to 0 to be ready for the next message
    com = get_uart2_value();

    return com;
    
}