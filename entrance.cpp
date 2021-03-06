#include "Arduino.h"
#include "entrance.h"
#include <Servo.h>

#define GATE_OPEN 70
#define GATE_CLOSED 160

Servo servo;


bool readCarDetectionStatus(const int pin) {
  const int value = digitalRead(pin);
  return value == LOW;
}


void openGate(const int pin) {
  servo.attach(pin);
  servo.write(GATE_OPEN);
}


void closeGate(const int pin) {
  servo.attach(pin);
  servo.write(GATE_CLOSED);
}

