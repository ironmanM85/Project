#if 1

#include <Arduino.h>
#include "util/DHT.h"
#include <Firmata.h>
#include "util/Emotion_Farm.h"
#include "util/LiquidCrystal_I2C.h"
#include "util/SuperLoop.h"

#define DHTPIN		A0
#define SOIL		A1
#define FLAME	 	A2
#define BUZZER		5
#define PUMPA		9
#define DHTTYPE		DHT11
#define COMMAND_SOILD	0x01

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);

int LED[3] = { 2, 3, 4 };

/*class Fire : public SuperLoop{
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
};*/

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

class AnalogReport : public SuperLoop {
public:
	byte analogIsEnable[4];
	int preAnalogValue[4];
	AnalogReport() : SuperLoop(6000) {
		memset(preAnalogValue, 0, sizeof(preAnalogValue));
	}
	void job() override {
		for (int i=0; i<4; i++) {
			if (analogIsEnable[i]) {
				int value = analogRead(i);
				preAnalogValue[i] = value;
				Firmata.sendAnalog(i, value);
			}
		}
	}
};

AnalogReport analogReport;

void setPinModeCallback(uint8_t pin, int mode) {
	if (mode == PIN_MODE_PULLUP)
		pinMode(pin, INPUT_PULLUP);
	else
		pinMode(pin, mode);

}

void reportAnalogCallback(uint8_t pin, int enable) {
	if (pin < 4)
		analogReport.analogIsEnable[pin] = enable;
}

void sysExCallback(uint8_t command, uint8_t argc, uint8_t *argv) {
	switch (command) {
		case COMMAND_SOILD:
			if (argc == 4) {
				byte firstByte = argv[0] | argv[1] << 7;
				byte secondByte = argv[2] | argv[3] << 7;
				int value = firstByte << 8 | secondByte;
				lcd.setCursor(1, 1);
				lcd.write(4);
				lcd.print(String(" ") + value + String("%"));
				lcd.setCursor(9, 1);
				if(value < 25){
					waterpump.state = 1;

					for (int i = 0; i < 3; i++) {
						lcd.write(6);
						lcd.print(" ");
					}
					digitalWrite(LED[0], HIGH);
					digitalWrite(LED[1], LOW);
				}
				else{
					waterpump.state = 0;

					for (int i = 0; i < 3; i++) {
						lcd.write(5);
						lcd.print(" ");
					}
					digitalWrite(LED[0], LOW);
					digitalWrite(LED[1], HIGH);
				}
			}
			break;
		default:
			break;
	}
}

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

	Firmata.begin(115200);
	Firmata.attach(SET_PIN_MODE, setPinModeCallback);
	Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
	Firmata.attach(START_SYSEX, sysExCallback);

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

	/*lcd.setCursor(2, 0);
	lcd.print("IoT Project!!");
	lcd.setCursor(0, 1);
	lcd.print("Smart Flowerpot!");*/
}

Temp_Humid temp_humid;
Moisture moi;
//Fire fire;


void loop(){
	if (Firmata.available())
		Firmata.processInput();

	analogReport.loop();
	waterpump.loop();

}

//	else{
//		moi.loop();
//		waterpump.loop();
//	}

	/*else{

	}*/
//	fire.loop();
//
//	if(fire.state){
//
////		moi.loop();
//		waterpump.loop();
//	}
//
//	else{
//		waterpump.pumpOff();
//	}

#endif
