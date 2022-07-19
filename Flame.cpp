#if 0

#include <Arduino.h>
#include <Firmata.h>

int flame = A0;
int Buzzer = 5;
int val = 0;


void setup(){
	pinMode(flame,INPUT);
	pinMode(Buzzer,OUTPUT);
	Serial.begin(115200);
}

void loop(){
	val = analogRead(flame);
	Serial.println(val);
}

#endif
