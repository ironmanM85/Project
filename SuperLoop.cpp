
#include "SuperLoop.h"

void SuperLoop::loop() {
    int currentMillis = millis();
    if (currentMillis-previousMillis>=cycle) {
      	previousMillis=currentMillis;

      	job();
    }
}

void SuperLoop::job() {

}

SuperLoop::~SuperLoop() {

}
