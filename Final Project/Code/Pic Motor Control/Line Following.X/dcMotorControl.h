#ifndef dcMotorControl_H__
#define dcMotorControl_H__

#include "nu32dip.h"

#define CONTROL_SCALER 3
#define TURN_SCALER 5

void setupMotors();

void setMotors(int Left_speed, int Right_Speed);

void setRobotControl(int speed, int turnDirection, int turnSpeed);

void calculateControl(int mean, float sd);

#endif