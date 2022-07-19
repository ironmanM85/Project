#if 1

#include <Arduino.h>
#include <Firmata.h>

int flame = A0;
int Buzzer = 5;
int	val = 0;

void setup() {
//	pinMode(flame,OUTPUT);
	pinMode(Buzzer,OUTPUT);
	Serial.begin(115200);
}

void loop() {
	val = analogRead(flame);
	Serial.println(val);
	if(val < 50) {
		digitalWrite(5,LOW);
		analogWrite(6, 0);
	}else{
		digitalWrite(5, HIGH);
		analogWrite(6, 255);
	}
	delay(1000);
}

#endif
