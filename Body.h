#ifndef BODY_H
#define BODY_H

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h" 

class BODY {
public:
	BODY();
	~BODY();
	void nothing();
	void doOps();
};

#endif
/*
Body

Fuse fuse
Key key
Mouth mouth
Face face
Brain brain

while{
	event = brain.wait_for_input(timePeriod)
	push_event_to_peripheral(event)
}*/