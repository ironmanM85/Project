#if 1

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "util/LiquidCrystal_I2C.h"
#include "util/SuperLoop.h"

#define FLAME	 	A2
#define BUZZER		7

LiquidCrystal_I2C lcd(0x27,16,2);
int LED[3] = { 2, 3, 4 };

class Fire : public SuperLoop{
public:
	int state = 1;

	Fire() : SuperLoop(500){
		pinMode(BUZZER,OUTPUT);
	}

	void job() override{
		Check_Fire();
	}

	void Check_Fire(){
		int fireV = analogRead(FLAME);
		Serial.println(fireV);
		if(fireV < 20) {
			digitalWrite(BUZZER,LOW);
			digitalWrite(LED[2], LOW);
			state = 1;
		}else{
			digitalWrite(BUZZER, HIGH);
			digitalWrite(LED[2], HIGH);
			lcd.clear();
			lcd.print("Fire!!!!");
			state = 0;
		}
	}
};

Fire fire;

void FireThread(void *param){
	for(;;){
		fire.loop();
		delay(500);
	}
}

void setup(){
	Serial.begin(115200);

	for (int i = 0; i < 3; i++)
			pinMode(LED[i], OUTPUT);

	lcd.init();
	lcd.backlight();
	lcd.clear();

	xTaskCreate(FireThread, "Fire", 128, nullptr, 3, nullptr);
}


void loop(){

}

#endif
