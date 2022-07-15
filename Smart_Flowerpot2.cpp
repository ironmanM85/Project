#include <DHT.h>
#include <Arduino.h>
#include <Emotion_Farm.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN		A0
#define DHTTYPE		DHT11

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);

void setup(){
	Serial.begin(115200);

	lcd.init();
	lcd.backlight();	lcd.createChar(0, temp);
	lcd.createChar(1, C);
	lcd.createChar(2, humi);
	lcd.createChar(4, water);
	lcd.createChar(5, good);
	lcd.createChar(6, bad);
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
	delay(1000);

	lcd.clear();
}

void loop(){
	int h = dht.readHumidity();
	int t = dht.readTemperature();

	Serial.println(String("Humid : ") + h + String(", Temp : ") + t);

	lcd.setCursor(0, 0);
	lcd.write(2);
	lcd.print(String("Humid : ") + h);

	lcd.setCursor(0, 1);
	lcd.write(0);
	lcd.print(String("Temp : ") + t);


	delay(1000);
}
