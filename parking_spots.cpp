#include "Arduino.h"
#include "parking_spots.h"
#include "http.h"
#include <ArduinoJson.h>

#define PHOTORESISTOR_THRESHOLD 400


ParkingSpot::ParkingSpot() : occupied(true) {}


ParkingSpot * initializeParkingSpots(const int statusPins[], const int ledPathPins[]) {
  ParkingSpot * parkingSpots = new ParkingSpot[PARKING_SPOTS_COUNT];

  for(int i = 0; i < PARKING_SPOTS_COUNT; i++) {
    parkingSpots[i].id = i + 1;
    parkingSpots[i].statusPin = statusPins[i];
    parkingSpots[i].ledPathPin = ledPathPins[i];
  }

  return parkingSpots;
}


bool readParkingSpotStatus(ParkingSpot parkingSpot) {
  const int analogValue = analogRead(parkingSpot.statusPin);

  if(analogValue > PHOTORESISTOR_THRESHOLD) {
    return PARKING_SPOT_EMPTY;
  }

  return PARKING_SPOT_OCCUPIED;
}


bool updateParkingSpotStatusOnServer(const ParkingSpot parkingSpot, const int newStatus) {
  Serial.println("Updating parking spot status");

  StaticJsonBuffer<128> jsonBuffer;
  JsonObject & requestBodyJson = jsonBuffer.createObject();
  requestBodyJson["id"] = parkingSpot.id;
  requestBodyJson["newStatus"] = newStatus;

  char requestBody[128];
  requestBodyJson.printTo(requestBody, sizeof(requestBody));

  int statusCode;
  String response = httpRequest(HTTP_PUT, PARKING_SPOT_RESOURCE, String(requestBody), statusCode);

  if(statusCode == HTTP_OK) {
    JsonObject & responseJson = jsonBuffer.parseObject(response);

    if(responseJson.success()) {
      const int responseStatusCode = responseJson["statusCode"];
      
      if(responseStatusCode == HTTP_NO_CONTENT) {
        Serial.println("Parking spot status updated");
        return true;
      } else {
        Serial.print("Unexpected response status code: ");
        Serial.println(responseStatusCode);
      }
    } else {
      Serial.println("Unable to parse response body");
    }
  } else {
    Serial.println("Updating parking spot status failed.");
  }

  return false;
}


const int getEmptyParkingSpotIdFromServer() {
  Serial.println("Fetching empty parking spot");

  int statusCode;
  String response = httpRequest(HTTP_GET, PARKING_SPOT_EMPTY_RESOURCE, "", statusCode);

  if(statusCode == HTTP_OK) {
    StaticJsonBuffer<128> jsonBuffer;
    JsonObject & root = jsonBuffer.parseObject(response);

    if(root.success()) {
      const int responseStatusCode = root["statusCode"];

      if(responseStatusCode == HTTP_OK) {
        return root["body"]["id"];
      } else if(responseStatusCode == HTTP_NOT_FOUND) {
        Serial.println("There are no empty parking spots left.");
      } else {
        Serial.print("Unpexpected status code: ");
        Serial.println(statusCode);
      }
    } else {
      Serial.println("Parsing JSON body failed");
    }
  } else {
    Serial.print("Unpexpected status code: ");
    Serial.println(statusCode);
  }

  return -1;
}


void changeLedPathToTheParkingSpot(const ParkingSpot parkingSpot, bool newState) {
  digitalWrite(parkingSpot.ledPathPin, newState ? HIGH : LOW);
}

