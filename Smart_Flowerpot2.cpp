#include <DHT.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN		A0
#define DHTTYPE		DHT11

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);

void setup(){
	Serial.begin(115200);

	lcd.init();
	lcd.backlight();
	lcd.clear();

	pinMode(DHTPIN,INPUT);

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
	lcd.print(String("Humid : ") + h);

	lcd.setCursor(0, 1);
	lcd.print(String("Temp : ") + t);


	delay(1000);
}
