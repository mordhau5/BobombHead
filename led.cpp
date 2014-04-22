#include "LED.h"

byte pinID;

LED::LED(){
}

LED::~LED(){
}

//Public functions
byte getPinID(){
	return pinID;
}

void LED::setup(byte id){
	currVal = 0;
	pinID = id;
}