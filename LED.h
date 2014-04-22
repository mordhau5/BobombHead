#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED{
public:
	LED();
	~LED();
	void setup(byte);
	byte getPinID();

private:
	byte pinID;
	byte currVal;
};

#endif