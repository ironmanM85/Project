#if 1

#include <Arduino.h>
#include "util/Emotion_Farm.h"
#include "util/LiquidCrystal_I2C.h"

#define Soil 		A1

LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
	Serial.begin(115200);

	lcd.init();
	lcd.backlight();
	lcd.clear();

	pinMode(Soil,INPUT);

	lcd.createChar(4, water);
	lcd.createChar(5, good);
	lcd.createChar(6, bad);
}

void loop(){
	int value = analogRead(Soil);

	Serial.println(String("Soli = ") + value);

	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.write(4);
	lcd.print(String(" ") + value);

	lcd.setCursor(9, 1);
	if(value > 800){
		for(int i = 0; i < 3; i++){
			lcd.write(6);
			lcd.print(" ");
		}
	} else{
		for(int i = 0; i < 3; i++){
			lcd.write(5);
			lcd.print(" ");
		}
	}
	delay(1000);
}
#endif
