//---------------------------------------------------------------------------------------------------------------------
//  
//---------------------------------------------------------------------------------------------------------------------
//  Copyright 2020 Manuel Pérez Jiménez (a.k.a. manuoso) manuperezj@gmail.com
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

#include <feetech_servo/WaterArm.h>
#include <feetech_servo/Joystick.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <string>



int main(int _argc, char **_argv) {
	int mode=0;
	bool verifyInitServo=0;
	bool verifyInitJoystick=0;
	int id_,setSpeed_,setTime_;

	WaterArm arm;
	verifyInitServo=arm.init();
	verifyInitJoystick=arm.initJoystick();
	std::cout <<"Volvemos a main.cpp\n";
	if(verifyInitServo==true){
		std::cout << "Se ha realizado correctamente init\n";
		arm.servoCalibration(1);
		arm.servoCalibration(2);
			while(arm.fin==0){
				std::cout <<"Display:\n 1 keyboardMode \n 2 joystickMode\n 3 SetSpeedAndTime \n 4 servoCalibration\n Press any other key to finish program";
				std::cin >> mode;
				switch(mode)
				{

				
					case 1 :
					arm.finKeyboard=0;
					std::cout << "W ->Shoulder Up \n S -> Shoulder Down \n E -> Elbow Up \n D -> Elbow Down \n A -> END";
					while(arm.finKeyboard==0){
						
						arm.keyboardMode();
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}
					break;
					case 2: 
					arm.finJoystick=0;
					std::cout << "Left Joystick UP ->Shoulder Up \n Left Joystick Down -> Shoulder Down \n Right Joystick Up -> Elbow Up \n Right Joystick Down -> Elbow Down \n Press X -> Shoot \nPress other button -> END";
					while(arm.finJoystick==0){
						arm.joystickMode();
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
					break;
					case 3:
					std::cout << "Enter idJoint\n1 idShoulder\n 2 idElbow\n";
					std::cin  >> id_;
					std::cout << "Enter Speed (recommendended between 1000 and 3000)\n";
					std::cin  >> setSpeed_;
					std::cout << "Enter Time (recommended between 0 and 100)";
					std::cin  >> setTime_;
					arm.setSpeedAndTime(id_,setSpeed_,setTime_);
					break;

					case 4:
					arm.servoCalibration(1);
					arm.servoCalibration(2);
					break;

					default :
					arm.fin=1;
					std:: cout <<"Program End";

				}
				
				
				


			}
	}
	else {
		std::cout << "No se ha realizado correctamente init\n";
	}
	
 
	return 0;
}
	