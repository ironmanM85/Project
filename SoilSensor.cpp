#if 0

#include <Arduino.h>

#define Soil 		A1

void setup(){
	Serial.begin(115200);

	pinMode(Soil,INPUT);
}

void loop(){
	int value = analogRead(Soil);

	Serial.println(String("Soli = ") + value);

	delay(1000);
}

#endif
