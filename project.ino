#include "parking_spots.h"


ParkingSpot * parkingSpots;


void setup() {
  parkingSpots = initializeParkingSpots();
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // TODO : implement this
}

