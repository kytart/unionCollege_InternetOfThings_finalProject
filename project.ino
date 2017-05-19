/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <WiFi101.h>
#include <jsmn.h>

char ssid[] = "Giraffe"; //  your network SSID (name)
char pass[] = "r0bot1C5";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char HOST[] = "kf4ofjvtq0.execute-api.us-east-2.amazonaws.com";
char API_KEY[] = "aD07hFg8zbhdXvx361pQazN5nLqcygTQso1HSA30";

WiFiClient client;

void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();
}

void loop() {
  getEmptyParkingSpotId();
  while (true);
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void getEmptyParkingSpotId() {
  Serial.println("Fetching empty parking spot");

  int statusCode;
  String response = httpRequest("GET", "/prod/parkingspot/empty", &statusCode);
}

void updateParkingSpotStatus() {
  // TODO
}

String httpRequest(char method[], char resource[], int * statusCode) {
  Serial.print("Server request: ");
  Serial.print(method);
  Serial.print(" ");
  Serial.println(resource);
  
  if(client.connectSSL(HOST, 443)) {
    client.print(method);
    client.print(" ");
    client.print(resource);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(HOST);
    client.print("x-api-key: ");
    client.println(API_KEY);
    client.println("Connection: close");
    client.println();

    String response = "";

    // read the whole response into a string
    while(client.connected()) {
      while(client.available()) {
        response += (char) client.read();
      }
    }
    
    client.stop();

    // parse response status code
    int statusCodeStartIndex = 9;
    *statusCode = response.substring(statusCodeStartIndex, statusCodeStartIndex + 3).toInt();

    Serial.print("Response: ");
    Serial.println(*statusCode);
    
    return response;
  } else {
    Serial.println("connection failed");
  }

  client.stop();
  *statusCode = 0;
  return "";
}

//char * httpRequest(char method[], char resource[], int * statusCode) {
//  if(client.connectSSL(HOST, 443)) {
//    client.print(method);
//    client.print(" ");
//    client.print(resource);
//    client.println(" HTTP/1.1");
//    client.print("Host: ");
//    client.println(HOST);
//    client.print("x-api-key: ");
//    client.println(API_KEY);
//    client.println("Connection: close");
//    client.println();
//
//    unsigned int responseSize = 1;
//    unsigned int current = 0;
//    char * response = (char *) malloc(responseSize * sizeof(char));
//
//    // read the whole response into a string
//    // allocate initial size of string and if it's insufficient, double it's size
//    while(client.connected()) {
//      if(client.available()) {
//        unsigned int bytesAvailable = client.available();
//        bool resize = false;
//        
//        // Check if combined size of previously read bytes, new bytes and one byte for terminator
//        // exceed the allocated size for the string.
//        // If it does, reallocate the string and double its size.
//        while(current + bytesAvailable + 1 > responseSize) {
//          responseSize *= 2;
//          resize = true;
//        }
//  
//        if(resize) {
//          response = (char *) realloc(response, responseSize * sizeof(char));
//          resize = false;
//        }
//
//        for(unsigned int i = 0; i < bytesAvailable; i++) {
//          response[current++] = client.read();
//        }
//      }
//
//      delay(100);
//    }
//
//    response[current] = '\0';
//    client.stop();
//
//    if(strlen)
//
//    // parse response status code
//    statusCode = 
//    
//    return response;
//  } else {
//    Serial.println("connection failed");
//  }
//
//  client.stop();
//  return NULL;
//}

