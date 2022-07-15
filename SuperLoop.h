
#ifndef _UTIL_SUPERLOOP_H_
#define _UTIL_SUPERLOOP_H_

#include <Arduino.h>

class SuperLoop {
	int previousMillis;
protected:
  	int cycle;
public:
    SuperLoop(int cycle) {
		previousMillis = millis();
      	this->cycle = cycle;
    }
    void loop();
    virtual void job();
    virtual ~SuperLoop();
};

#endif /* _UTIL_SUPERLOOP_H_ */
