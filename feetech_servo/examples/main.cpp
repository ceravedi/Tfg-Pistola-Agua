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

#include <feetech_servo/SCServo.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>

int main(int _argc, char **_argv) {		
	std::cout << "Write serial port (default: /dev/ttyUSB0), type 1 for default: ";
	std::string portUSB;
	std::cin >> portUSB;
	if(portUSB == "1"){
		portUSB = "/dev/ttyUSB0";
	}

	std::cout << "Write ID of Servo Feetech you want to calibrate: ";
	int idServo;
	std::cin >> idServo;

	SCServo *servoDriver;
	servoDriver =  new SCServo(portUSB);

	if(servoDriver->isConnected()){
		servoDriver->EnableTorque(idServo, 1);
		int pos = 500;
		servoDriver->WritePos(idServo, pos, 100, 0);
    	}else{
		std::cout << "Servo Driver not connected!" << std::endl;
    	}

	return 0;
}
