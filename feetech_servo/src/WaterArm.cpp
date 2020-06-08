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

#include <feetech_servo/WaterArm.h>
#include <feetech_servo/Joystick.h>
#include <string>

WaterArm::WaterArm(){
}

// -------------------------------------------------------------------------------------------------
bool WaterArm::init(){

    

	 std::cout << "Write serial port (default: /dev/ttyUSB0), type 1 for default: ";
	 std::string portUSB;
	 std::cin >> portUSB;
	 if(portUSB == "1"){
		 portUSB = "/dev/ttyUSB0";
	 }
    

	// SCServo *servoDriver_;
	servoDriver_ = new SCServo(portUSB);

   	std::cout << "Write ID of Shoulder Servo: ";
	std::cin >>idShoulder_;

    std::cout << "Write ID of Elbow Servo: ";
	std::cin >>idElbow_;

    std::cout << "idShoulder:" << idShoulder_ << "  idElbow:" << idElbow_ << std::endl;
	std::cout << "Trying to connect both servos\n";

		if(servoDriver_->isConnected()){
		 servoDriver_->EnableTorque(idShoulder_, 1);
		 std::cout << "Shoulder_ connected\n";
		 servoDriver_->EnableTorque(idElbow_,1);
		 std::cout << "Elbow_ connected\n";
		 
     	}else{
		     std::cout << "ServoDriverShoulder not connected!" << std::endl;
			 return false;
    	}


		speedElbow_=1000;
		timeElbow_=0;
		speedShoulder_=1000;
		timeShoulder_=0;
		maxPositionElbow_=1300;
		minPositionElbow_=700;
		maxPositionShoulder_=1200;
		minPositionShoulder_=800;
		std::cout << "Valores default:\n Speed 3000 \n Time 0 \n minpositionShoulder 500 maxpositionShoulder 900 \n minpositionElbow 100 maxpositionElbow 500\n";
		
    return true;
}

//--------------------------------------------------------------------------------------------------------

bool WaterArm::servoCalibration(int idJoint_){
	
	std::cout <<"Entramos en servoclibration con idJoint:" <<idJoint_<< std::endl;
	if (idJoint_==idShoulder_){
		int pos=800;
		while(pos<maxPositionShoulder_){
		 servoDriver_->WritePos(idShoulder_, pos,speedShoulder_,timeShoulder_);
			 std::cout << "Current position: " << pos << " of servo with id: " << idShoulder_ << std::endl;
			 std::this_thread::sleep_for(std::chrono::milliseconds(200));
			 pos = pos + 50;
		}

		while(pos>minPositionShoulder_){
		 servoDriver_->WritePos(idShoulder_, pos, speedShoulder_, timeShoulder_);
			 std::cout << "Current position: " << pos << " of servo with id: " << idShoulder_ << std::endl;
			 std::this_thread::sleep_for(std::chrono::milliseconds(200));
			 pos = pos - 50;
		}

		servoDriver_->WritePos(idShoulder_, 830, speedShoulder_, timeShoulder_);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << "ServoId:"<< idShoulder_ << "Calibrated"<< std::endl;
		positionActualShoulder_=830;

	}

	if (idJoint_==idElbow_){
		int pos =500;
		while(pos<maxPositionElbow_){
		 servoDriver_->WritePos(idElbow_, pos, speedElbow_, timeElbow_);
			 std::cout << "Current position: " << pos << " of servo with id: " << idElbow_ << std::endl;
			 std::this_thread::sleep_for(std::chrono::milliseconds(200));
			 pos = pos + 50;
		}

		while(pos>minPositionElbow_){
		 servoDriver_->WritePos(idElbow_, pos, speedShoulder_, timeShoulder_);
			 std::cout << "Current position: " << pos << " of servo with id: " << idElbow_ << std::endl;
			 std::this_thread::sleep_for(std::chrono::milliseconds(200));
			 pos = pos - 50;
		}

		servoDriver_->WritePos(idElbow_, 730, speedShoulder_, timeShoulder_);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "ServoId:"<< idElbow_ << "Calibrated"<< std::endl;
		positionActualElbow_=730;
		
	}

	if(idJoint_!=idShoulder_ && idJoint_!=idElbow_){
		std::cout << "No Joint calibrated\n";
		return false;
	}


	return true;
}

//----------------------------------------------------------------------------------------------------------

bool WaterArm::moveJoint(int idJoint_,int position_){
if(idJoint_==idShoulder_){
	if(position_> maxPositionShoulder_){
		position_=maxPositionShoulder_;
		std::cout << "Posición máxima limitada para idJoint:"<< idJoint_ << std::endl;
	} 
	if(position_< minPositionShoulder_){
		position_=minPositionShoulder_;
		std::cout << "Posición mínima limitada para idJoint:" << idJoint_<< std::endl;
	}
	servoDriver_->WritePos(idJoint_,position_,speedShoulder_,timeShoulder_);
	positionActualShoulder_=position_;
}
if(idJoint_==idElbow_){
	if(position_> maxPositionElbow_){
		position_=maxPositionElbow_;
		std::cout << "Posición máxima limitada para idJoint:"<< idJoint_ << std::endl;
	} 
	if(position_< minPositionElbow_){
		position_=minPositionElbow_;
		std::cout << "Posición mínima limitada para idJoint:" << idJoint_<< std::endl;
	}

	servoDriver_->WritePos(idJoint_,position_,speedElbow_,timeElbow_);
	positionActualElbow_=position_;
}
if(idJoint_!=idElbow_ && idJoint_!=idShoulder_)
{
	std::cout << "idJoint no coincide con ninguna variable articular\n";
return false;
}

return true;
}

//----------------------------------------------------------------------------------------------------------------

bool WaterArm::setSpeedAndTime(int idJoint_,int speed_,int time_){

	if(idJoint_==idShoulder_){
		speedShoulder_=speed_;
		timeShoulder_=time_;
		std::cout <<"Speed and time change correctly\n";
	}
	if(idJoint_==idElbow_)
	{
		speedElbow_=speed_;
		timeElbow_=time_;
		std::cout <<"Speed and time change correctly\n";

	}
	if(idJoint_!=idShoulder_ && idJoint_!=idElbow_){
		std::cout <<"idJoint_ not found!\n";
		return false;
	}





return true;
}

//-------------------------------------------------------------------------------------------------------------

bool WaterArm::keyboardMode(){
	finKeyboard=0;
	std::cout <<"Estoy en keyboardmode\n";
	character=getchar();
	std::cout <<"getchar saca el valor para character"<<character<<std::endl;
	if(character==119){
		positionActualShoulder_=positionActualShoulder_+30;
		std::cout <<"I detected W\n";
		moveJoint(idShoulder_,positionActualShoulder_);



	}
	if(character==115){
		positionActualShoulder_=positionActualShoulder_-30;
		std::cout <<"I detected S\n";
		moveJoint(idShoulder_,positionActualShoulder_);

	}

	if(character==101){
		positionActualElbow_=positionActualElbow_+30;
		std::cout <<"I detected E\n";
		moveJoint(idElbow_,positionActualElbow_);

	}
	if(character==100){
		positionActualElbow_=positionActualElbow_-30;
		std::cout <<"I detected D\n";
		moveJoint(idElbow_,positionActualElbow_);

	}

		if(character==97){
		std::cout <<"I detected A\n";
		finKeyboard=1;

	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	

return true;
}


//---------------------------------------------------------------------------------------------------------------------------

bool WaterArm::initJoystick(){

	if (!joysticks.isFound())
{
std::cout <<"initJoystick doesn't work.\n";
  // hmm
}
else{
	std::cout << "initJoystick works correctly";
	return false;
}

return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------

bool WaterArm::joystickMode(){

	if (joysticks.sample(&joysticksEvent))
  {
    if (joysticksEvent.isButton())
    {
      std::cout << "Button " << int(joysticksEvent.number)  << " is " << joysticksEvent.value << std::endl;
    }
	if(joysticksEvent.number==0 && joysticksEvent.value==1)
	{
		shoot=1;
		std::cout <<"WATERR!!\n";

	}
	else{
		shoot=0;
	}
	if(joysticksEvent.number!=0 && joysticksEvent.value==1)
	{
		finJoystick=1;
		std::cout <<"END\n";

	}

	 if (joysticksEvent.isAxis())
    {
     	std::cout <<"Axis" << int(joysticksEvent.number) <<" is at position" << joysticksEvent.value << std::endl;
	 	if(joysticksEvent.number==1 && joysticksEvent.value>20000){
			 if(positionActualShoulder_<maxPositionShoulder_+50 && positionActualShoulder_>minPositionShoulder_-50){
				positionActualShoulder_=positionActualShoulder_-10;
			 	std::cout <<"DETECTADO JOYSTICK IZQUIERDO HACIA ABAJO(HOMBRO HACIA ABAJO)\n";
				moveJoint(idShoulder_,positionActualShoulder_);
			 }
	 	}
						

		 if(joysticksEvent.number==1 && joysticksEvent.value<-20000){
			if(positionActualShoulder_<maxPositionShoulder_+50 && positionActualShoulder_>minPositionShoulder_-50){
				positionActualShoulder_=positionActualShoulder_+10;
				std::cout <<"DETECTADO JOYSTICK IZQUIERDO HACIA ARRIBA(HOMBRO HACIA ARRIBA)\n";
				moveJoint(idShoulder_,positionActualShoulder_);
			}
	 	}


		if(joysticksEvent.number==4 && joysticksEvent.value>20000){
			 if(positionActualElbow_<maxPositionElbow_+50 && positionActualElbow_>minPositionElbow_-50){
			 	positionActualElbow_=positionActualElbow_-10;
				std::cout <<"DETECTADO JOYSTICK DERECHO HACIA ABAJO(CODO HACIA ABAJO)\n";
				moveJoint(idElbow_,positionActualElbow_);
			 }
	 	}

		 if(joysticksEvent.number==4 && joysticksEvent.value<-20000){
			 if(positionActualElbow_<maxPositionElbow_+50 && positionActualElbow_>minPositionElbow_-50){
				positionActualElbow_=positionActualElbow_+10;
				std::cout <<"DETECTADO JOYSTICK DERECHO HACIA ARRIBA(CODO HACIA ARRIBA)\n";
				moveJoint(idElbow_,positionActualElbow_);
			 }

	 	}
    }
  }

}