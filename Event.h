#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h" 

class EVENT {
public:
	EVENT();
	~EVENT();
	void nothing();
	String toString();
};

#endif