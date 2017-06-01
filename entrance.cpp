#include "Arduino.h"
#include "entrance.h"


bool readCarDetectionStatus(const int pin) {
  const int value = digitalRead(pin);
  return value == LOW;
}

