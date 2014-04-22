#include "Sequencer.H"


Sequencer::Sequencer() : nLeds(6), rows(30), cols(8), fpms(0.020)
{
	int ledPin[6] = {11,10,9,6,5,3};
	float sequence[rows][cols];
	float lowVal = 0;
	float highVal = 255;
	int finalEvt = rows-1;
}

Sequencer::~Sequencer(){}

void Sequencer::doOps(){
	
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

void Sequencer::createRandomSequence(){
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

int Sequencer::lastEventOnPeripheral(int pid, int startIndex){
	int pos = -1;
	for(int i=startIndex-1;i>=0;i--){
		if(sequence[i][7] == pid){
			pos = i;
			i = 0;
		}
	}
	return pos;
}