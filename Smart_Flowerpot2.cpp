#if 1

#include <Arduino.h>
#include "util/DHT.h"
#include <Firmata.h>
#include "util/Emotion_Farm.h"
#include "util/LiquidCrystal_I2C.h"
#include "util/SuperLoop.h"

#define DHTPIN		A0
#define SOIL 		A1
#define FLAME	 	A2
#define BUZZER		5
#define PUMPA		9
#define DHTTYPE		DHT11

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);

int LED[3] = { 2, 3, 4 };

class Fire : public SuperLoop{
public:
	int state = 1;

	Fire() : SuperLoop(300){
		pinMode(BUZZER,OUTPUT);
	}

	void job() override{
		Check_Fire();
	}

	void Check_Fire(){
		int fireV = analogRead(FLAME);
		Serial.println(fireV);
		if(fireV < 15) {
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

class Temp_Humid : public SuperLoop{
	int h = 0;
	int t = 0;
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
		pinMode(PUMPA, OUTPUT);
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
		analogWrite(PUMPA, 255);
	}

	void pumpOff() {
		analogWrite(PUMPA, 0);
	}
};

WaterPump waterpump;

class Moisture: public SuperLoop {
	int value = 0;
public:
	Moisture() : SuperLoop(6000) {
		pinMode(SOIL, INPUT);
	}

	void job() override {
		readSoil();
	}

	void readSoil() {
		clear(0,1);
		value = map(analogRead(SOIL), 1023, 200, 0, 100);
		Serial.print(String("Soil value = ") + analogRead(SOIL));
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
	lcd.print("IoT Project!!");
	lcd.setCursor(0, 1);
	lcd.print("Smart Flowerpot!");
}

Temp_Humid temp_humid;
Moisture moi;
Fire fire;

void loop(){
	fire.loop();

	if(fire.state){
		temp_humid.loop();
		moi.loop();
		waterpump.loop();
	}

	else{
		waterpump.pumpOff();
	}
}

#endif
