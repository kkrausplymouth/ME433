#ifndef IOEXTEND_H
#define	IOEXTEND_H

#ifndef I2C_MASTER_NOINT_H__
#include "i2c_master_noint.h"
#endif

void chip_setup(unsigned char io);

int read_IO(void);

void toggle_LED(unsigned char IO_pin, unsigned char state);

#endif	/* IOEXTEND_H */

