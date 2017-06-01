#include "http.h"
#include "parking_spots.h"
#include "entrance.h"

#define ASSIGN_PARKING_SPOT_RETRY_INTERVAL 2000
#define PARKING_SPOT_ASSIGNED_TIMEOUT 10000

#define PIN_STATUS 13
#define PIN_ENTRANCE_BREAKBEAM 9
#define PIN_GATE_SERVO 10

const int parkingSpotPins[] = { A3, A4, A5 };
const int ledPathPins[] = { 10, 11, 12 };

bool carDetected = false;
int assignedParkingSpotId;

unsigned long lastTimeTriedAssignParkingSpot;
unsigned long parkingSpotAssignedAt;

ParkingSpot * parkingSpots;


void setup() {
  // setup status LED pin
  pinMode(PIN_STATUS, OUTPUT);
  digitalWrite(PIN_STATUS, LOW);
  
  // setup break-beam pin
  pinMode(PIN_ENTRANCE_BREAKBEAM, INPUT);
  digitalWrite(PIN_ENTRANCE_BREAKBEAM, HIGH);

  // setup LED pins
  for(int i = 0; i < PARKING_SPOTS_COUNT; i++) {
    pinMode(ledPathPins[i], OUTPUT);
  }

  // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // setup WiFi
  setupWifi();
  
  // initialize parking spots
  Serial.println("initializing parking spots internal state");
  parkingSpots = initializeParkingSpots(parkingSpotPins, ledPathPins);
  resetParkingSpotAssigned();

  

  // when everything is setup, light up status LED
  Serial.println("ready...");
  digitalWrite(PIN_STATUS, HIGH);
}


void loop() {
  updateParkingSpotsStatus();
  updateCarDetected();
  checkIfShouldResetParkingSpotAssigned();
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
            parkingSpotAssignedAt = millis();
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


void checkIfShouldResetParkingSpotAssigned() {
  unsigned long currentTime = millis();
  
  if(assignedParkingSpotId > 0 && currentTime - parkingSpotAssignedAt > PARKING_SPOT_ASSIGNED_TIMEOUT) {
    resetParkingSpotAssigned();
  }
}


void resetParkingSpotAssigned() {
  changeLedPathToTheParkingSpot(parkingSpots[assignedParkingSpotId - 1], false);
  closeGate(PIN_GATE_SERVO);
  
  assignedParkingSpotId = -1;
  lastTimeTriedAssignParkingSpot = 0;
  parkingSpotAssignedAt = 0;
}

