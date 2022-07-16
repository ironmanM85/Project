#if 1

#include <Arduino.h>

#define Soil 		A1
#define PumpA		9

void setup(){
	Serial.begin(115200);

	pinMode(PumpA, OUTPUT);
	pinMode(Soil, INPUT);
}

void loop(){
	int value = analogRead(Soil);
	Serial.println(String("Soil value = ") + value);

	if(value > 800){
//		digitalWrite(PumpA, HIGH);
		analogWrite(PumpA, 255);
	}

	else{
//		digitalWrite(PumpA, LOW);
		analogWrite(PumpA, 0);
	}

	delay(1000);
}

#endif
