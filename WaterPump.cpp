#if 1

#include <Arduino.h>

void setup(){
	pinMode(9, OUTPUT);
}

void loop(){
	analogWrite(9,255);
	delay(1000);
	analogWrite(9,0);
	delay(1000);
}

#endif
