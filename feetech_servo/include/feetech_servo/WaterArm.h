//---------------------------------------------------------------------------------------------------------------------
//  
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2020 Javier Cebolla Verdugo (ceravedi) theceravedi@gmail.com
//---------------------------------------------------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
//  and associated documentation files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify, merge, publish, distribute,
//  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all copies or substantial
//  portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
//  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//---------------------------------------------------------------------------------------------------------------------

#ifndef WATER_ARM_H_
#define WATER_ARM_H_

#include <iostream>
#include <thread>
#include <chrono>
#include <string>


#include <feetech_servo/Joystick.h>
#include <feetech_servo/SCServo.h>

class WaterArm{
public:
    WaterArm();

    bool init();
    bool initJoystick();
    bool servoCalibration(int idJoint_);
    bool moveJoint(int idJoint_,int position_); 
    bool setSpeedAndTime(int idJoint_,int speed_,int time_);
    bool keyboardMode();
    bool joystickMode(); 
    
 public:
    bool fin=0;
    bool finJoystick=0;
    bool finKeyboard=0;

private:

    Joystick joysticks;
    JoystickEvent joysticksEvent;
    int character; // tecla de caracter 
    int idElbow_, idShoulder_;
    int positionActualShoulder_,positionActualElbow_;
    int maxPositionElbow_, minPositionElbow_;
    int maxPositionShoulder_, minPositionShoulder_;
    int speedElbow_,timeElbow_;
    int speedShoulder_,timeShoulder_;
    int shoot;
    SCServo *servoDriver_;
};


#endif
