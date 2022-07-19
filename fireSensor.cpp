#if 1

#include <Arduino.h>
#include <Firmata.h>

#define flame	A0
#define Buzzer	5
#define LED		6

void setup() {
	Serial.begin(115200);

	pinMode(flame,INPUT);
	pinMode(Buzzer,OUTPUT);
	pinMode(LED, OUTPUT);
}

void loop() {
	int	val = 0;
	val = analogRead(flame);
	Serial.println(val);
	if(val < 50) {
		digitalWrite(Buzzer,LOW);
		digitalWrite(LED, LOW);
	}else{
		digitalWrite(Buzzer, HIGH);
		digitalWrite(LED, HIGH);
	}
}

#endif
