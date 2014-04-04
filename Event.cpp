#include "EVENT.h"

/*
****EVENT OBJECT****
An event is a (linear) instruction for a specific device. It could be changing the analog value on a led,
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

[Goal Value, Step Instruction, Timeline(start time), Time b/w Ops, referenceID for a Device, head event, tail event]
*/

EVENT::EVENT(){
    //pinMode(LED_PIN, OUTPUT); //make that pin an OUTPUT
}

//<<destructor>>
EVENT::~EVENT(){/*nothing to destruct*/}

void EVENT::nothing(){
	//does nothing
}

String EVENT::toString(){
	//returns this object as a string
	return "";
}