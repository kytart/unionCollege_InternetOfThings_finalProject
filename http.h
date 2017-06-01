#include "Arduino.h"

#define HTTP_OK 200
#define HTTP_NO_CONTENT 204
#define HTTP_NOT_FOUND 404

#define HTTP_GET "GET"
#define HTTP_PUT "PUT"

#define PARKING_SPOT_EMPTY_RESOURCE "/prod/parkingspot/empty"
#define PARKING_SPOT_RESOURCE "/prod/parkingspot"


void setupWifi();

String httpRequest(char method[], char resource[], const String body, int & statusCode);

