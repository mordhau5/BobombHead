//#include <BODY.h>

//objects
//BODY body;

//hardware info
int btnPin = 7;
const int nLeds = 6;
int ledPin[nLeds] = {11,10,9,6,5,3};
boolean lastBtn;

void setup(){
	//give the body declared hardware
	pinMode(btnPin, INPUT);
	for(int i=0;i<nLeds;i++){
		pinMode(ledPin[i], OUTPUT);
	}
	
	lastBtn = LOW;
}

void loop(){
	boolean btnPushed = digitalRead(btnPin);
	if(btnPushed && !lastBtn){
		delay(5);
		lastBtn = HIGH;
		doOps();
	}
	else{
		lastBtn = btnPushed;
	}
}

/****
GOAL: To execute several simultaneous "Events" to completion on the timeline and framerate they are given.

This is a test to see how well the Arduino will perform given several "Events"(not official DS yet) to
perform operations on.  It will need to be able to switch between Events quickly in order
process them quick enough for real time.
*****/
void doOps(){
	Serial.begin(9600);
	const float fpms = 0.020;
	const int rows = 12;
	const int cols = 8;
	const int finalEvt = rows-1;
	float highVal = 50;
	float lowVal = 0;
	float sequence[rows][cols] = {
		/*Event: 
		initial value, final value, event runtime(ms)(delta), event begin(ms)(timeline),	<--Given Vals
		step value, time between ops(ms)(delta), last op time(ms)(timeline), 				<--Calced Vals (below)
		ledPin#
		*/
		/*{0, 255, 1500, 0, 0, 0, 0, 0}, 
		{0, 50, 1000, 500, 0, 0, 0, 2}, 
		{40, 167, 3050, 1250, 0, 0, 0, 1},
		{167, 0, 3050, 3250, 0, 0, 0, 1},
		{50, 167, 1500, 3950, 0, 0, 0, 2},
		{255, 20, 5050, 4000, 0, 0, 0, 3},
		{255, 0, 2000, 4250, 0, 0, 0, 0},
		{0, 200, 2400, 5250, 0, 0, 0, 4}*/
		{0, highVal, 1500, 0, 0, 0, 0, 0}, 
		{0, highVal, 1500, 0, 0, 0, 0, 2}, 
		{0, highVal, 1500, 0, 0, 0, 0, 4},
		{highVal, 0, 2000, 2000, 0, 0, 0, 0},
		{highVal, 0, 2000, 2000, 0, 0, 0, 2},
		{highVal, 0, 2000, 2000, 0, 0, 0, 4},
		{0, highVal, 2000, 3000, 0, 0, 0, 1},
		{0, highVal, 2000, 3000, 0, 0, 0, 3},
		{0, highVal, 2000, 3000, 0, 0, 0, 5},
		{highVal, 0, 500, 5000, 0, 0, 0, 1},
		{highVal, 0, 500, 5000, 0, 0, 0, 3},
		{highVal, 0, 500, 5000, 0, 0, 0, 5}
	};
	for(int i=0;i<rows;i++){//determine step values and time between ops (so I don't need to for every new test case)
		sequence[i][4] = (sequence[i][1] - sequence[i][0]) * fpms;
		sequence[i][5] = sequence[i][2] * fpms;
	}
	
	long timeCurr, timeStart = millis();
	Serial.print(">Begin @ ");
	Serial.println(timeStart);

	int i = 0, seqGoal = sequence[finalEvt][1];
	while(sequence[finalEvt][0] != seqGoal){  //go through events until last event on sequence timeline has completed
	/*NOT TRUE WHEN EARLIER EVENTS GO LONGER THAN THE LAST EVENT-BEGIN ON TIMELINE
	** ^-> Want to use a linked list type object for events and so traversal and sequence completion will likely be 
	**		determined by the existence of more. 
	**		 ^-> Conditional will be different than one here
	** 		 ^-> Plan test cases accordingly
	*/
		timeCurr = millis() - timeStart;
		if((float)timeCurr >= sequence[i][3]){ //is it time to start this event?
			
			// Serial.print((long)sequence[i][5]);
			// Serial.print(" ");
			// Serial.println(timeCurr);
			/*is this event still running && has the given amount of time passed since this event last had an op ?*/
			timeCurr = millis() - timeStart;
			if( (sequence[i][0] != sequence[i][1]) && ( (float)timeCurr - sequence[i][6] >= sequence[i][5] ) ) { 
				// Serial.print(i);
				/*this would need to change for other types of operations*/
				float rawVal = sequence[i][0] + sequence[i][4];
				if(sequence[i][4] < 0)
					sequence[i][0] = constrain(floor(rawVal),sequence[i][1],sequence[i][0]); //do negative op
				else
					sequence[i][0] = constrain(floor(rawVal),sequence[i][0],sequence[i][1]); //do positive op
				analogWrite(ledPin[(int)sequence[i][7]], sequence[i][0]);
				timeCurr = millis() - timeStart;
				sequence[i][6] = (float)timeCurr; //store op time
				// Serial.println(rawVal);
				// Serial.println(timeCurr/1000.0); //Can't concatenate with FL/DBL by default :(
				//Serial.println(sequence[i][3]);
				 // Serial.print(constrain(sequence[i][0]+sequence[i][3],sequence[i][0],sequence[i][1]));
				// Serial.println(sequence[i][5]);
				  // Serial.println(sequence[i][6]);
				/*Serial commands slow things down immensely!*/
			}
			/*maybe remove event if it's completed? Hopefully in finalized DS*/
		}
		else {
			// Serial.println("so this happened");
			i = -1; //can't start this event in the *ordered* sequence, no sense in going forward.  Optimization measure.
		}
		if(i == finalEvt) //We're at the last event, start back at first one. OH WAIT THIS IS NOT TRUE WHEN EARLIER EVENTS HAVE COMPLETED <-- but this check also might not be helpful
			i=0;
		else
			i++;
	}

	Serial.print("Finished in ");
	Serial.println(millis() - timeStart);
}