#if 1

#include <Arduino.h>
#include <Firmata.h>

int Buzzer = 5;

void setup(){
	pinMode(Buzzer,OUTPUT);
	Serial.begin(115200);
}

void loop(){
	digitalWrite(Buzzer,HIGH);
	delay(1000);
	digitalWrite(Buzzer,LOW);
	delay(1000);
}

#endif
