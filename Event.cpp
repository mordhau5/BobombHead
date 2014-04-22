#include "EVENT.h"

/*
****EVENT OBJECT****
An event is a (linear) instruction for a specific device. It could be changing the analog value on an led,
or moving a Servo to a given position. An event is a time-based concept, and the action it corresponds to 
can take place over any given period.

Events can be chained together in order of the time they start at.

An event only knows the increment values to be given to its device and the device's address NOT how to use 
those values. That is handled by whatever object type corresponds to that hardware (handled above the event). 
Events also do not know when they were last seen by a processor, that should be stored with the device (to save 
memory).

The Goal value for the event is stored but not passed to the device. Theoretically, once the Goal is reached 
this event would be destroyed and the event linking to this event would be hooked up to this event's next event, 
thus completing the link.

****Debating whether or not to store the event runtime or calculate it when it's needed, since it technically
doesn't need to be accessed when inside of event processing (where speed is crucial for timings).****

[completed(bit), Timeline(start time)(unsigned long), Time b/w Ops(unsigned int), Goal Value(byte), referenceID for a Device(byte?), FunctionID(byte), Step Value(byte)]
*/

byte goalValue;
byte* devicePtr;
unsigned long startTime;
unsigned int opGapTime; //may be able to get away with byte here, we'll see. would be the diff between 8.5-12.5s and waaaay more.
//Event* head;
//Event* tail;

EVENT::EVENT(){
}

//<<destructor>>
EVENT::~EVENT(){/*nothing to destruct*/}

void EVENT::nothing(){
	startTime = 56;
}

String EVENT::toString(){
	//returns this object as a string
	return "";
}