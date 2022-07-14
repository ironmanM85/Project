#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
	Serial.begin(115200);

	lcd.init();
	lcd.backlight();
	lcd.clear();

	lcd.setCursor(2, 0);
	lcd.print("IoT Project");
	lcd.setCursor(0, 1);
	lcd.print("Smart Flowerpot!");
	delay(1000);
}

void loop(){

}
