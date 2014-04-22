#include <Sequencer.h>
//objects
//BODY body;

//hardware info
int btnPin = 7;
boolean lastBtn;
Sequencer sequencer;
/*const int nLeds = 6;
int ledPin[nLeds] = {11,10,9,6,5,3};
const float fpms = 0.020;
const int rows = 30;
const int cols = 8;
const int finalEvt = rows-1;
float highVal = 255;
float lowVal = 0;
float sequence[rows][cols];*/ //= {
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
}

void loop(){
	boolean btnPushed = digitalRead(btnPin);
	if(btnPushed && !lastBtn){
		delay(5);
		lastBtn = HIGH;
		sequencer.createRandomSequence();
		sequencer.doOps();
	}
	else{
		lastBtn = btnPushed;
	}
}