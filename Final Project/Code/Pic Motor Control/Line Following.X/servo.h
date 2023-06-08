#ifndef servo_H__
#define servo_H__

#include "nu32dip.h"

#define STARTRANGE 0.5
#define ENDRANGE 2.5

void servoSetupB(int pin, int timerNum, int OCNum);
void servoSetAngle(int angle, int OCNum);

#endif