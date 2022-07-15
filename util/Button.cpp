#include "Button.h"

Button::Button(int pinNumber) : SuperLoop(30) {
	this->pinNumber = pinNumber;
	this->buttonState = 1;
	this->lastReading = 1;

	push = nullptr;
	up = nullptr;

	pinMode(pinNumber, INPUT_PULLUP); // 내부 풀업저항 사용
}

void Button::job() {
	int reading = digitalRead(pinNumber);

	if (reading != buttonState) {
		if (reading == lastReading) {
			if (buttonState == 1) {
				//Serial.println("PUSH");
				if (push != nullptr)
					push();
				else
					btn_push();
			} else {
				//Serial.println("UP");
				if (up != nullptr)
					up();
				else
					btn_up();
			}

			buttonState = reading;
		}
	}

	lastReading = reading;
}

void Button::btn_push() {

}
void Button::btn_up() {

}
