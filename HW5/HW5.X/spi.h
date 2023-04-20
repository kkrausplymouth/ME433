#ifndef SPI__H__
#define SPI__H__

#ifndef NU32DIP__H__
#include "nu32dip.h"
#endif

#define CS LATBbits.LATB10

void initSPI();

unsigned char spi_io(unsigned char o);

#endif // SPI__H__