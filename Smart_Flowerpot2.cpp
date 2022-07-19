#if 1

#include "util/DHT.h"
#include <Arduino.h>
#include "util/Emotion_Farm.h"
#include "util/LiquidCrystal_I2C.h"
#include "util/SuperLoop.h"

#define DHTPIN		A0
#define Soil 		A1
#define PumpA		9
#define DHTTYPE		DHT11

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);

int LED[3] = { 2, 3, 4 };

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
		clear(0,0);
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

	void clear(int col, int row){
		lcd.setCursor(col, row);
		char space[16+1];
		sprintf(space,"%16s", " ");
		lcd.print(space);
	}
};

class WaterPump: public SuperLoop {
	int prestate = 1;
public:
	int state = 0;

	WaterPump() : SuperLoop(3000) {
		pinMode(PumpA, OUTPUT);
	}

	void job() override {
		if(prestate & state){
			pumpOn();
			state = 0;
		}
		else
			pumpOff();
	}

	void pumpOn() {
		analogWrite(PumpA, 255);
	}

	void pumpOff() {
		analogWrite(PumpA, 0);
	}
};

WaterPump waterpump;

class Moisture: public SuperLoop {
	int value = 0;
public:
	Moisture() : SuperLoop(6000) {
		pinMode(Soil, INPUT);
	}

	void job() override {
		readSoil();
	}

	void readSoil() {
		clear(0,1);
		value = map(analogRead(Soil), 1023, 200, 0, 100);
		Serial.print(String("Soil value = ") + analogRead(Soil));
		Serial.println(String(" Soil map value = ") + value);
		lcd.setCursor(1, 1);

		lcd.write(4);
		lcd.print(String(" ") + value + String("%"));
		lcd.setCursor(9, 1);

		if (value < 25)
			On();
		else
			Off();
	}

	void On() {
		waterpump.state = 1;

		for (int i = 0; i < 3; i++) {
			lcd.write(6);
			lcd.print(" ");
		}
		digitalWrite(LED[0], HIGH);
		digitalWrite(LED[1], LOW);
	}

	void Off() {
		waterpump.state = 0;

		for (int i = 0; i < 3; i++) {
			lcd.write(5);
			lcd.print(" ");
		}
		digitalWrite(LED[0], LOW);
		digitalWrite(LED[1], HIGH);
	}

	void clear(int col, int row){
		lcd.setCursor(col, row);
		char space[16+1];
		sprintf(space,"%16s", " ");
		lcd.print(space);
	}
};

void setup(){
	Serial.begin(115200);

	for (int i = 0; i < 3; i++)
			pinMode(LED[i], OUTPUT);

	lcd.init();
	lcd.backlight();
	lcd.clear();

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
}

Temp_Humid temp_humid;
Moisture moi;

void loop(){
	temp_humid.loop();
	moi.loop();
	waterpump.loop();
}

#endif
