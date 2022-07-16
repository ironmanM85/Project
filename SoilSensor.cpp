#if 0

#include <Arduino.h>
#include "util/Emotion_Farm.h"
#include "util/LiquidCrystal_I2C.h"
#include "util/SuperLoop.h"

#define Soil 		A1

LiquidCrystal_I2C lcd(0x27,16,2);

int LED[3] = {2,3,4};

<<<<<<< HEAD
=======
class Moisture : public SuperLoop{
	int value;
public:
	Moisture(): SuperLoop(1000){
		pinMode(Soil,INPUT);
		for(int i = 0; i < 3; i++)
			pinMode(LED[i],OUTPUT);
	}

	void job() override{
		readSoil();
	}

	void readSoil(){
		lcd.clear();
		value = map(analogRead(Soil), 1023, 200, 0, 100);
		Serial.print(String("Soil value = ") + analogRead(Soil));
		Serial.println(String(" Soil map value = ") + value);
		lcd.setCursor(1, 1);
		lcd.write(4);
		lcd.print(String(" ") + value + String("%"));
		lcd.setCursor(9, 1);

		if(value < 25)
			On();
		else
			Off();
	}

	void On(){
		for(int i = 0; i < 3; i++){
			lcd.write(6);
			lcd.print(" ");
		}
		digitalWrite(LED[0],HIGH);
		digitalWrite(LED[1],LOW);
	}

	void Off(){
		for(int i = 0; i < 3; i++){
			lcd.write(5);
			lcd.print(" ");
		}
		digitalWrite(LED[0],LOW);
		digitalWrite(LED[1],OUTPUT);
	}
};

>>>>>>> ABH
void setup(){
	Serial.begin(115200);

	lcd.init();
	lcd.backlight();
	lcd.clear();

<<<<<<< HEAD
	pinMode(Soil,INPUT);
	for(int i = 0; i < 3; i++)
		pinMode(LED[i],OUTPUT);

=======
>>>>>>> ABH
	lcd.createChar(4, water);
	lcd.createChar(5, good);
	lcd.createChar(6, bad);
}

Moisture moi;

void loop(){
<<<<<<< HEAD
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
			digitalWrite(LED[0],HIGH);
			digitalWrite(LED[1],LOW);
		}
	} else{
		for(int i = 0; i < 3; i++){
			lcd.write(5);
			lcd.print(" ");
			digitalWrite(LED[0],LOW);
			digitalWrite(LED[1],OUTPUT);
		}
	}

	delay(1000);
=======
	moi.loop();
>>>>>>> ABH
}

#endif
