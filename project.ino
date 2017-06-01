#include "parking_spots.h"
#include "entrance.h"

#define ASSIGN_PARKING_SPOT_RETRY_INTERVAL 2000

#define PIN_ENTRANCE_BREAKBEAM 9
#define PIN_GATE_SERVO 10

const int parkingSpotPins[] = { A3, A4, A5 };
const int ledPathPins[] = { 10, 11, 12 };

bool carDetected = false;
int assignedParkingSpotId = -1;
unsigned long lastTimeTriedAssignParkingSpot = 0;

ParkingSpot * parkingSpots;


void setup() {
  parkingSpots = initializeParkingSpots(parkingSpotPins, ledPathPins);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  updateParkingSpotsStatus();
  updateCarDetected();
  delay(100);
}

void updateParkingSpotsStatus() {
  bool updateSuccessful;
  bool occupied;
  
  for(int i = 0; i < PARKING_SPOTS_COUNT; i++) {
    occupied = readParkingSpotStatus(parkingSpots[i]);

    if(parkingSpots[i].occupied != occupied) {
      updateSuccessful = updateParkingSpotStatusOnServer(parkingSpots[i], occupied);

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

void updateCarDetected() {
  bool detectionStatus = readCarDetectionStatus(PIN_ENTRANCE_BREAKBEAM);
  int assignedParkingSpot;
  unsigned long currentTime = millis();

  if(carDetected != detectionStatus) {
    if(carDetected) {
      carDetected = false;
    } else {
      Serial.println("Incoming car detected");
      carDetected = true;
      
      // only try assign an empty parking spot, if haven't tried recently
      if(lastTimeTriedAssignParkingSpot == 0 || 
         currentTime - lastTimeTriedAssignParkingSpot >= ASSIGN_PARKING_SPOT_RETRY_INTERVAL
        ) {
          assignedParkingSpot = getEmptyParkingSpotIdFromServer();

          if(assignedParkingSpot > 0) {
            assignedParkingSpotId = assignedParkingSpot;
            // open gate
            openGate(PIN_GATE_SERVO);
            // light up path to the assigned parking spot
            changeLedPathToTheParkingSpot(parkingSpots[assignedParkingSpotId - 1], true);
          } else {
            Serial.println("Failed assigning a parking spot");
          }

          lastTimeTriedAssignParkingSpot = 0;
         }
    }
  }
}

