#include "draw.h"

void drawChar(unsigned char acsii_char, unsigned char x, unsigned char y){
    for (int j = 0; j<5; j++){
        char col = ASCII_std[acsii_char - 0x20][j];
        for (int i=0; i<8; i++){
            ssd1306_drawPixel(x+j, y+i, (col>>i)&0b1);
        }
    }
    ssd1306_update();
}

void drawString(char* m, char x, char y, unsigned char spacing){
    int i = 0;
    while (m[i] != '\0'){
        drawChar(m[i], x + (5*i + spacing), y);
        i++;
    }
    ssd1306_update();
}