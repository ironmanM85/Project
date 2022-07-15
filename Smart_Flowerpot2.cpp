#if 1

#include <DHT.h>
#include <Arduino.h>
#include <Emotion_Farm.h>
#include <LiquidCrystal_I2C.h>
#include "util/SuperLoop.h"

#define DHTPIN		A0
#define DHTTYPE		DHT11

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);

class Temp_Humid : public SuperLoop{
	int h;
	int t;
public:
	Temp_Humid() : SuperLoop(6000){
		pinMode(DHTPIN,INPUT);
	}

	void job() override{
		readDht();
	}

	void readDht(){
		lcd.clear();
		h = dht.readHumidity();
		t = dht.readTemperature();

		Serial.println(String("Humid : ") + h + String(" Temp : ") + t);

		lcd.setCursor(1, 0);
		lcd.write(0);
		lcd.print(String(" ") + t);
		lcd.write(1);

		lcd.setCursor(9, 0);
		lcd.write(2);
		lcd.print(String(" ") + h + String("%"));
	}
};

void setup(){
	Serial.begin(115200);

	lcd.init();
	lcd.backlight();
	lcd.clear();

	pinMode(DHTPIN,INPUT);

	lcd.createChar(0, temp);
	lcd.createChar(1, C);
	lcd.createChar(2, humi);
	lcd.createChar(4, water);
	lcd.createChar(5, good);
	lcd.createChar(6, bad);

	lcd.setCursor(2, 0);
	lcd.print("IoT Project");
	lcd.setCursor(0, 1);
	lcd.print("Smart Flowerpot!");

	lcd.clear();
}

Temp_Humid temp_humid;
void loop(){
	temp_humid.loop();

}

#endif
