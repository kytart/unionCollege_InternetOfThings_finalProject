#include "parking_spots.h"

const int parkingSpotPins[] = { A3, A4, A5 };

ParkingSpot * parkingSpots;


void setup() {
  parkingSpots = initializeParkingSpots(parkingSpotPins);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  updateParkingSpotsStatus();
  delay(100);
}

void updateParkingSpotsStatus() {
  bool updateSuccessful;
  bool occupied;
  
  for(int i = 0; i < PARKING_SPOTS_COUNT; i++) {
    occupied = readParkingSpotStatus(parkingSpots[i]);

    if(parkingSpots[i].occupied != occupied) {
      updateSuccessful = updateParkingSpotStatus(parkingSpots[i], occupied);

      if(updateSuccessful) {
        parkingSpots[i].occupied = occupied;
      } else {
        Serial.print("Updating parking spot (id ");
        Serial.print(parkingSpots[i].id);
        Serial.println(") failed.");
      }
    }
  }
}

