#include <xc.h>
#ifndef NU32DIP__H__
#include "nu32dip.h"
#endif
#ifndef FONT_H__
#include "font.h"
#endif
#ifndef SSD1306_H__
#include "ssd1306.h"
#endif


void drawChar(unsigned char acsii_char, unsigned char x_posn, unsigned char y_posn);

void drawString(char* m, char x, char y, unsigned char spacing);