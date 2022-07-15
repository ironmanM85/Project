#ifndef _UTIL_BUTTON_H_
#define _UTIL_BUTTON_H_

#include "SuperLoop.h"

class Button: public SuperLoop {
	int pinNumber;
	int buttonState;
	int lastReading;

	void (*push)();	// 함수 포인터 변수
	void (*up)();
public:
	Button(int pinNumber);
	void job() override;
	void setPushHandler(void (*push)()) {
		this->push = push;
	}
	void setUpHandler(void (*up)()) {
		this->up = up;
	}
	virtual void btn_push();
	virtual void btn_up();
};

#endif /* SRC_UTIL_BUTTON_H_ */
