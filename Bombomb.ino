//#include <BODY.h>

//objects
//BODY body;

//hardware info
int btnPin = 7;
const int nLeds = 6;
int ledPin[nLeds] = {11,10,9,6,5,3};
boolean lastBtn;
const float fpms = 0.020;
const int rows = 30;
const int cols = 8;
const int finalEvt = rows-1;
float highVal = 255;
float lowVal = 0;
float sequence[rows][cols]; //= {
	/*Event: 
	initial value, final value, event runtime(ms)(delta), event begin(ms)(timeline),	<--Given Vals
	step value, time between ops(ms)(delta), last op time(ms)(timeline), 				<--Calced Vals (below)
	ledPin#
	*/
	/*
	{0, 255, 1500, 0, 0, 0, 0, -1}, 
	{0, 50, 1000, 500, 0, 0, 0, -1}, 
	{40, 167, 3050, 1250, 0, 0, 0, -1},
	{167, 0, 3050, 3250, 0, 0, 0, -1},
	{50, 167, 1500, 3950, 0, 0, 0, -1},
	{255, 20, 5050, 4000, 0, 0, 0, -1},
	{255, 0, 2000, 4250, 0, 0, 0, -1},
	{0, 200, 2400, 5250, 0, 0, 0, -1},
	{0, 200, 2400, 5250, 0, 0, 0, -1},
	{0, 200, 2400, 5250, 0, 0, 0, -1},
	{0, 200, 2400, 5250, 0, 0, 0, -1},
	{0, 200, 2400, 5250, 0, 0, 0, -1}*/
	/*
	{0, highVal, 1500, 0, 0, 0, 0, 0}, 
	{0, highVal, 1500, 0, 0, 0, 0, 2}, 
	{0, highVal, 1500, 0, 0, 0, 0, 4},
	{highVal, 0, 1950, 2000, 0, 0, 0, 0},
	{highVal, 0, 1950, 2000, 0, 0, 0, 2},
	{highVal, 0, 1950, 2000, 0, 0, 0, 4},
	{0, highVal, 1950, 3000, 0, 0, 0, 1},
	{0, highVal, 1950, 3000, 0, 0, 0, 3},
	{0, highVal, 1950, 3000, 0, 0, 0, 5},
	{highVal, 0, 500, 5200, 0, 0, 0, 1},
	{highVal, 0, 500, 5200, 0, 0, 0, 3},
	{highVal, 0, 500, 5200, 0, 0, 0, 5}*/

void setup(){
	//give the body declared hardware
	pinMode(btnPin, INPUT);
	for(int i=0;i<nLeds;i++){
		pinMode(ledPin[i], OUTPUT);
	}
	Serial.begin(9600);
	lastBtn = LOW;
}

void loop(){
	boolean btnPushed = digitalRead(btnPin);
	if(btnPushed && !lastBtn){
		delay(5);
		lastBtn = HIGH;
		createRandomSequence();
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
	
	for(int i=0;i<rows;i++){//determine step values and time between ops (so I don't need to for every new test case)
		sequence[i][4] = (sequence[i][1] - sequence[i][0]) * fpms;
		sequence[i][5] = sequence[i][2] * fpms;
	}

	//Show sequence here
	Serial.println("Executing sequence {");
	for(int i=0;i<rows;i++){
		Serial.print("[");
		Serial.print((int)sequence[i][0]);
		for(int j=1;j<cols;j++){
			Serial.print(", ");
			Serial.print((int)sequence[i][j]);
		}
		Serial.println("],");
	}
	Serial.println("}");
	
	
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
		if((float)timeCurr >= sequence[i][3]){
			timeCurr = millis() - timeStart;
			if( (sequence[i][0] != sequence[i][1]) && ( (float)timeCurr - sequence[i][6] >= sequence[i][5] ) ) { 
				/*Serial.print(i);
				Serial.print(" @ ");
				Serial.println(timeCurr);*/
				float rawVal = sequence[i][0] + sequence[i][4];
				if(sequence[i][4] < 0)
					sequence[i][0] = constrain(rawVal,sequence[i][1],sequence[i][0]);
				else
					sequence[i][0] = constrain(rawVal,sequence[i][0],sequence[i][1]);
				analogWrite(ledPin[(int)sequence[i][7]], (int)sequence[i][0]);
				timeCurr = millis() - timeStart;
				sequence[i][6] = (float)timeCurr;
			}
			/*maybe remove event if it's completed? Hopefully in finalized DS*/
		}
		else
			i = -1; //Optimization measure
		if(i == finalEvt) //THIS IS NOT TRUE WHEN EARLIER EVENTS HAVE COMPLETED <-- but this check also might not be helpful
			i=0;
		else
			i++;
	}

	Serial.print("Finished in ");
	Serial.println(millis() - timeStart);

	//zero out sequence and peripherals
	for(int i=0;i<rows;i++){
		int pinNum = sequence[i][7];
		analogWrite(ledPin[pinNum], 0);
		sequence[i][7]=-1;
	}
}

void createRandomSequence(){
	long seed = analogRead(0) * millis(); //Read input on unconnected pin and time
	randomSeed(seed);
	Serial.print("Seed: ");
	Serial.println(seed);
	long timeStart = millis();
	int peripheralID, lastEventOnPID;
	int initValue;
	for(int i=0;i<rows;i++){
		peripheralID = random(nLeds);
		lastEventOnPID = lastEventOnPeripheral(peripheralID, i);
		//Event Initial Value
		if(lastEventOnPID < 0)//No Event found for this LED
			sequence[i][0] = random(highVal);
		else//set it's inital value to the final value of the last event
			sequence[i][0] = sequence[lastEventOnPID][1];
		//Event Final Value
		sequence[i][1] = random(highVal);
		//Event Runtime
		sequence[i][2] = random(500,5000);
		//Event Start Time
		if(lastEventOnPID >= 0)
			sequence[i][3] = max(sequence[lastEventOnPID][2] + sequence[lastEventOnPID][3] + 250,sequence[i-1][3]);
		else if(i > 0)
			sequence[i][3] = sequence[i-1][3];
		else
			sequence[i][3] = 0;
		//Event Peripheral ID
		sequence[i][4] = 0;
		sequence[i][5] = 0;
		sequence[i][6] = 0;
		sequence[i][7] = peripheralID;
	}
	Serial.print("Sequence generated in: ");
	Serial.println(millis() - timeStart);
}

int lastEventOnPeripheral(int pid, int startIndex){
	int pos = -1;
	for(int i=startIndex-1;i>=0;i--){
		if(sequence[i][7] == pid){
			pos = i;
			i = 0;
		}
	}
	return pos;
}