#include "Arduino.h"
#include "ACS712.h"

bool debug = 1;

bool chargingSlow = 0;
bool chargingFast = 0;

int i = 0;

float I = 0;
float Imin = 0;
float Imax = 0;

int relaisPin = 9;

ACS712 sensor(ACS712_05B, A0);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

	pinMode(2, INPUT_PULLUP);
	pinMode(7, INPUT_PULLUP);
	pinMode(6, INPUT_PULLUP);
	pinMode(11, OUTPUT);
	pinMode(relaisPin, OUTPUT);

	digitalWrite(relaisPin, HIGH);

	if (debug) {
		Serial.begin(9600);
	}

	sensor.calibrate();
	delay(500);
	sensor.calibrate();
	delay(500);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

	if (digitalRead(2) == LOW) {
		digitalWrite(relaisPin, LOW);

		if (digitalRead(7) == LOW) {
			chargingSlow = 1;
			chargingFast = 0;
		} else if (digitalRead(6) == LOW) {
			chargingFast = 1;
			chargingSlow = 0;
		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (chargingSlow) {

		digitalWrite(relaisPin, LOW);

		I = sensor.getCurrentAC() - 0.09;
		I = sensor.getCurrentAC() - 0.09;

		if (I > 0.25) {
			digitalWrite(11, HIGH);
		} else {
			digitalWrite(11, LOW);
		}

		Imin = min(Imin, I);
		Imax = max(Imax, I);

		if (I < Imax - 0.10) {
			delay(200);

			if (i == 1) {
				chargingSlow = 0;
				digitalWrite(relaisPin, HIGH);
				digitalWrite(11, LOW);
				Imin = 0;
				Imax = 0;
				i = 0;
			} else {
				i = 1;
			}
		}

		Serial.print("Slow   ");
		Serial.println(I);
		Serial.print("  ");
		Serial.println(i);

	} else if (chargingFast) {

		digitalWrite(relaisPin, LOW);

		I = sensor.getCurrentAC() - 0.09;

		Imin = min(Imin, I);
		Imax = max(Imax, I);

		if (I > 0.25) {
			digitalWrite(11, HIGH);
		} else {
			digitalWrite(11, LOW);
		}

		if (Imax > 0.25 && I < 0.15) {
			chargingFast = 0;
			digitalWrite(relaisPin, HIGH);
			digitalWrite(11, LOW);
			Imin = 0;
			Imax = 0;
		}

		Serial.print("Fast   ");
		Serial.println(I);

	}

	delay(50);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void charge() {
//	digitalWrite(relaisPin, LOW);
//	if (digitalRead(7) == LOW) {
//		chargingSlow = 1;
//		chargingFast = 0;
//	} else if (digitalRead(6) == LOW) {
//		chargingFast = 1;
//		chargingSlow = 0;
//	}
//}

//void charge() {
//	digitalWrite(relaisPin, LOW);
//
//	if ((millis() - alteZeit) > entprellZeit) {
//		if (digitalRead(7) == LOW) {
//			chargingSlow = 1;
//			chargingFast = 0;
//		} else if (digitalRead(6) == LOW) {
//			chargingFast = 1;
//			chargingSlow = 0;
//
//			alteZeit = millis(); // letzte Schaltzeit merken
//		}
//	}
//}

