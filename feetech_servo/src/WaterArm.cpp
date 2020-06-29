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
#include <math.h>

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

	my_serial = new serial::Serial ("/dev/ttyACM0",9600, serial::Timeout::simpleTimeout(1000));
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
		maxAbsPositionElbow_=1300;
		minAbsPositionElbow_=400;
		maxAbsPositionShoulder_=1020;
		minAbsPositionShoulder_=700;
		shoot=0;
		std::cout << "Valores default:\n Speed 3000 \n Time 0 \n minpositionShoulder 500 maxpositionShoulder 900 \n minpositionElbow 100 maxpositionElbow 500\n";
		
    return true;
}

//--------------------------------------------------------------------------------------------------------

bool WaterArm::servoCalibration(int idJoint_){
	
	std::cout <<"Entramos en servoclibration con idJoint:" <<idJoint_<< std::endl;
	if (idJoint_==idShoulder_){
		int pos=800;
		while(pos<maxAbsPositionShoulder_){
		 servoDriver_->WritePos(idShoulder_, pos,speedShoulder_,timeShoulder_);
			 std::cout << "Current position: " << pos << " of servo with id: " << idShoulder_ << std::endl;
			 std::this_thread::sleep_for(std::chrono::milliseconds(200));
			 pos = pos + 50;
		}

		while(pos>minAbsPositionShoulder_){
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
		while(pos<maxAbsPositionElbow_){
		 servoDriver_->WritePos(idElbow_, pos, speedElbow_, timeElbow_);
			 std::cout << "Current position: " << pos << " of servo with id: " << idElbow_ << std::endl;
			 std::this_thread::sleep_for(std::chrono::milliseconds(200));
			 pos = pos + 50;
		}

		while(pos>minAbsPositionElbow_){
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
	if(position_> maxAbsPositionShoulder_){
		position_=maxAbsPositionShoulder_;
		std::cout << "Posición máxima limitada para idJoint:"<< idJoint_ << std::endl;
	} 
	if(position_< minAbsPositionShoulder_){
		position_=minAbsPositionShoulder_;
		std::cout << "Posición mínima limitada para idJoint:" << idJoint_<< std::endl;
	}
	servoDriver_->WritePos(idJoint_,position_,speedShoulder_,timeShoulder_);
	positionActualShoulder_=position_;
}
if(idJoint_==idElbow_){
	if(position_> maxAbsPositionElbow_){
		position_=maxAbsPositionElbow_;
		std::cout << "Posición máxima limitada para idJoint:"<< idJoint_ << std::endl;
	} 
	if(position_< minAbsPositionElbow_){
		position_=minAbsPositionElbow_;
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
     // std::cout << "Button " << int(joysticksEvent.number)  << " is " << joysticksEvent.value << std::endl;
    }
		if(joysticksEvent.number==13 && joysticksEvent.value==1)
		{
			
			test_string="A";
			 my_serial->write(test_string);
		}

		if(joysticksEvent.number==13 && joysticksEvent.value==0)
		{
			
			
			test_string="B";
			my_serial->write(test_string);
		 	
		}
		if(joysticksEvent.number!=13 && joysticksEvent.value==1)
		{
			finJoystick=1;
			std::cout <<"END\n";
		}

	 if (joysticksEvent.isAxis())
    {
     //	std::cout <<"Axis" << int(joysticksEvent.number) <<" is at position" << joysticksEvent.value << std::endl;
	 	if(joysticksEvent.number==1 && joysticksEvent.value>20000){
			 if(positionActualShoulder_<maxAbsPositionShoulder_+50 && positionActualShoulder_>minAbsPositionShoulder_-50){
				positionActualShoulder_=positionActualShoulder_-10;
			 	std::cout <<"DETECTADO JOYSTICK IZQUIERDO HACIA ABAJO(HOMBRO HACIA ABAJO)\n"<< positionActualShoulder_<< std::endl;
				kinematicsLimits(positionActualShoulder_,positionActualElbow_);
				 
				moveJoint(idShoulder_,positionActualShoulder_);
			 }
	 	}
						

		 if(joysticksEvent.number==1 && joysticksEvent.value<-20000 && kinematicsLimits(positionActualShoulder_,positionActualElbow_)==true){
			if(positionActualShoulder_<maxAbsPositionShoulder_+50 && positionActualShoulder_>minAbsPositionShoulder_-50){
				positionActualShoulder_=positionActualShoulder_+10;
				std::cout <<"DETECTADO JOYSTICK IZQUIERDO HACIA ARRIBA(HOMBRO HACIA ARRIBA)\n"<< positionActualShoulder_<< std::endl;
				moveJoint(idShoulder_,positionActualShoulder_);
			}
	 	}


		if(joysticksEvent.number==4 && joysticksEvent.value>20000){
			 if(positionActualElbow_<maxAbsPositionElbow_+50 && positionActualElbow_>minAbsPositionElbow_-50){
			 	positionActualElbow_=positionActualElbow_-10;
				std::cout <<"DETECTADO JOYSTICK DERECHO HACIA ABAJO(CODO HACIA ABAJO)\n"<< positionActualElbow_<< std::endl;
				moveJoint(idElbow_,positionActualElbow_);
				kinematicsLimits(positionActualShoulder_,positionActualElbow_);
			 }
	 	}

		 if(joysticksEvent.number==4 && joysticksEvent.value<-20000 && kinematicsLimits(positionActualShoulder_,positionActualElbow_)==true){
			 if(positionActualElbow_<maxAbsPositionElbow_+50 && positionActualElbow_>minAbsPositionElbow_-50){
				positionActualElbow_=positionActualElbow_+10;
				std::cout <<"DETECTADO JOYSTICK DERECHO HACIA ARRIBA(CODO HACIA ARRIBA)\n"<< positionActualElbow_<< std::endl;
				moveJoint(idElbow_,positionActualElbow_);
			 }

	 	}
    }
  }
		
		
}

bool WaterArm::kinematicsLimits(int positionActualShoulder_, int positionActualElbow_){

double y,x;
double q1_;
double q2_;
double l1_=0.225;
double l2_=0.245;
double factorq1_=(((1000-800)/45)*(180));//Grados a radianes
double factorq2_=((790-390)/80)*(180);//Grados a radianes
q1_=((positionActualShoulder_-1000)*3.141592)/factorq1_;
q2_=((positionActualElbow_-400)*3.141592)/factorq2_;
y=l1_*sin(q1_)+l2_*sin(q1_+q2_);
x=l1_*cos(q1_)+l2_*cos(q1_+q2_);

std::cout<< "X en metros vale:"<< x << std::endl;
std::cout<< "Y en metros vale:"<< y << std::endl;
if (y<0) return true;
if (y>=0) return false;
	
}

//------------------------------------------------------------------------------------

bool WaterArm::inverseKinematics(double x, double y){

double xx,yy;
xx=x/100;//cm a m
yy=y/100;//cm a m
double q1pos_,q1posnew_;
double q1neg_,q1negnew_;
double cosq2_;
double sinq2_;
double q2pos_,q2posnew_;
double q2neg_,q2negnew_;
double alphapos,alphaneg;
double beta;
double l1_=0.225;
double l2_=0.245;
double q1final_,q2final_;
//calculamos el factor de cada servo para pasar de radianes a pasos de servo (factor/180)
double factorq1_=((1000-800)/45)*(180);//radianes a pasos de servo
double factorq2_=((780-390)/80)*(180);//radianes a pasos de servo
cosq2_=(xx*xx+yy*yy-l1_*l1_-l2_*l2_)/(2*l1_*l2_);
std::cout << "cosq2  " << cosq2_ << std::endl;
sinq2_=+sqrt(1-cosq2_*cosq2_);
std::cout << "sinq2  " << sinq2_ << std::endl;
q2pos_=atan(sinq2_/cosq2_);//radianes
std::cout << "q1pos  " << q1pos_ << std::endl;
q2neg_=atan(-sinq2_/cosq2_);//radianes
std::cout << "q1neg  " << q1neg_ << std::endl;
beta=atan(y/x);
std::cout << "beta  " << beta << std::endl;
alphapos=atan((l2_*sinq2_)/(l1_+l2_*cosq2_));
alphaneg=atan((l2_*(-sinq2_))/(l1_+l2_*cosq2_));
std::cout << "alphapos  " << alphapos << std::endl;
std::cout << "alphaneg  " << alphaneg << std::endl;
q1pos_=beta-alphapos;
std::cout << "q2pos_  " << q2pos_ << std::endl;
q1neg_=beta-alphaneg;
std::cout << "q2neg  " << q2neg_ << std::endl;
//calculado tanto q1 como q2, pasamos ambas nuestros 
q1posnew_=1000+(q1pos_*factorq1_)/3.141592;
q1negnew_=1000+(q1neg_*factorq1_)/3.141592;

q2posnew_=400+(q2pos_*factorq2_)/3.141592;
q2negnew_=400+(q2neg_*factorq2_)/3.141592;
std::cout << "Valores: q1positiva:  " << q1posnew_ << "  q1negativa:  " << q1negnew_ << "\n q2positiva:  " << q2posnew_ << " q2negativa:  " << q2negnew_ << std::endl;

if(q1posnew_<=q1negnew_) q1final_=q1posnew_;
else q1final_=q1negnew_;
if(q2posnew_>=q2negnew_) q2final_=q2posnew_;
else q2final_=q2negnew_;
kinematicsLimits(q1final_, q2final_);








return true;

}	