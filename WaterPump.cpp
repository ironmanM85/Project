#if 0

#include <Arduino.h>
#include "util/SuperLoop.h"

#define Soil 		A1
#define PumpA		9

class WaterPump : public SuperLoop{

public:
	WaterPump() : SuperLoop(3000){
		pinMode(PumpA, OUTPUT);
	}

	void job() override{
		int value = analogRead(Soil);

		Serial.println(String("Soil value = ") + value);

		if(value > 800){
			pumpOn();
		}

		else{
			pumpOff();
		}
	}

	void pumpOn(){
		analogWrite(PumpA, 255);
	}

	void pumpOff(){
		analogWrite(PumpA, 0);
	}

};

void setup(){
	Serial.begin(115200);

	pinMode(Soil, INPUT);
}

WaterPump waterpump;
void loop(){
	waterpump.loop();
}

#endif
