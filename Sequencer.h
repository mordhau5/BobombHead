#ifndef Sequencer_H
#define Sequencer_H

#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h" 

class Sequencer {
public:
	Sequencer();
	~Sequencer();
	void doOps();
	void createRandomSequence();

private:
	//byte goalValue;
	//byte* devicePtr;
	//unsigned long startTime;
	//unsigned int opGapTime;
	//Event* head, tail;
	typedef struct event{
		int data;
		event* next;
	}* eventPtr;
	eventPtr head;
	eventPtr curr;
	eventPtr temp;

	int lastEventOnPeripheral(int pid, int startIndex);
	const int nLeds;
	int ledPin[0];
	const float fpms;
	const int rows;
	const int cols;
	int finalEvt;
	float highVal;
	float lowVal;
	float sequence[][0];


};

#endif