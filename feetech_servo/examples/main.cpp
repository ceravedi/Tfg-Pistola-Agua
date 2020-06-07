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
	int modo=0;
	bool verifyInitServo=0;
	bool verifyInitJoystick=0;
	bool fin=0;
	bool *finKeyboard=0;
	bool finJoystick=0;
	bool finAutomatic=0;	
	WaterArm arm;
	verifyInitServo=arm.init();
	verifyInitJoystick=arm.initJoystick();
	std::cout <<"Volvemos a main.cpp\n";
	if(verifyInitServo==true){
		std::cout << "Se ha realizado correctamente init\n";
		arm.servoCalibration(1);
		arm.servoCalibration(2);
			while(fin==0){
				std::cout <<"Elige modo:\n 1 para modo teclado \n 2 para modo joystick\n 3 para modo automático ";
				std::cin >> modo;
				if(modo==1 ){
					while(finKeyboard==0){
						arm.keyboardMode();
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}
				}
				if(modo==2){
					while(finJoystick==0){
						arm.joystickMode();
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}


				}

				


			}
	}
	else {
		std::cout << "No se ha realizado correctamente init\n";
	}
	
 
	return 0;
}
	