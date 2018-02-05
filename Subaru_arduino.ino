/* Copyright 2012 fitc Jon Robinson
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
* USA.
*
* Project home page: 
* https://github.com/aegis1980/Subaru_arduino
*
*/


const int TEMP_INTERVAL = 5000; // 5 seconds
const int REVERSING_INTERVAL = 1000; //1 sec
unsigned long lastTempTime = 0;
unsigned long lastReverseTime = 0;

const int sensorPin = A5; // The input port
int sensorValue; // Current reading
int outputValue; // The reported reading
int lastValues[3] = { 0,0,0 }; // The last 3 readings

/**
Values for the various hardwart buttons. will depend on what resistors you have
in you button circuity. 
*/

const int R_NO_BUTTON = 1008;

const int R_INFO = 670;
const int R_MENU = 538;
const int R_MAP = 194;
const int R_AV = 403;
const int R_MEDIA = 102;

const int R_RANGE = 25;

/*
char messages sent over serial to the android raspi
*/

const char INFO = 'i';
const char MENU = 'm';
const char MAP = 'p';
const char AV = 'a';
const char MEDIA = 'e';

/* stop multiple presses of same button being sent*/
int old_r= R_NO_BUTTON;

void setup()
{
	Serial.begin(115200);

}

void loop()
{
	int hard_button_press = checkHardButtons();

	if (millis() - lastReverseTime >= REVERSING_INTERVAL) {
		checkReversing();
		lastReverseTime = millis();
	}

	if (millis() - lastTempTime >= TEMP_INTERVAL) {
		checkTempEtc();
		lastTempTime = millis();
	}
}

/*
Hardware buttons from the old system mounted in car.
5 buttons wired to single anaglog pin.
Got this debounce code and the wiring from 
*/
int checkHardButtons() {
	// read the value from the sensor:
	sensorValue = analogRead(sensorPin);
	// Initialise variables for checks
	int i;
	int updateOutput = 1;
	// Loop through previous readings
	for (i = 0; i<3; i++) {
		// If this historic value doesn't match the current reading,
		// we will not update the output value
		if (lastValues[i] != sensorValue) {
			updateOutput = 0;
		}
		// Shift the array elements to make room for new value
		if (i>0) {
			lastValues[(i - 1)] = lastValues[i];
		}
	}
	// Update if needed
	if (updateOutput == 1) {
		outputValue = sensorValue;
	}
	// Append the new value
	lastValues[2] = sensorValue;
	// Debugging output
	//Serial.print(sensorValue);
	//Serial.print(" ");
	//Serial.println(outputValue);

	int  r = R_NO_BUTTON;

	if (outputValue >= R_INFO - R_RANGE && outputValue <= R_INFO + R_RANGE) {
		r = R_INFO;
	}
	else if (outputValue >= R_MENU - R_RANGE && outputValue <= R_MENU + R_RANGE) {
		r = R_MENU;
	}
	else if (outputValue >= R_MAP - R_RANGE && outputValue <= R_MAP + R_RANGE) {
		r = R_MAP;
	}
	else if (outputValue >= R_AV- R_RANGE && outputValue <= R_AV + R_RANGE) {
		r = R_AV;
	}
	else if (outputValue >= R_MEDIA - R_RANGE && outputValue <= R_MEDIA + R_RANGE) {
		r = R_MEDIA;
	}

	if (r != R_NO_BUTTON && r != old_r) {
		char c;
		switch (r) {
		case R_INFO:
			c = INFO;
			break;
		case R_MENU:
			c = MENU;
			break;
		case R_MAP:
			c = MAP;
			break;
		case R_AV:
			c = AV;
			break;
		case R_MEDIA:
			c = MEDIA;
			break;
		}
		// stop multiple presses of same button being sent
		old_r = r;
		Serial.print(c);
		Serial.print("\n");
	}

	return r;

}

/*
TODO
Will check whether in revserse (from old wiring harness) and activate USB camera. 
Deactivate when not in reverse

*/
void checkReversing() {

}

/*
Will check external ambient temp. Will probaly just use a new sensor, 
but there is one in car somewhere (from olf system) which could presumeably be accessed from harness.
*/
void checkTempEtc() {

}