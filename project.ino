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
#include <ArduinoJson.h>

#define EMPTY 0
#define OCCUPIED 1

#define HTTP_OK 200
#define HTTP_NO_CONTENT 204
#define HTTP_NOT_FOUND 404

#define PORT_SSL 443

#define HOST "kf4ofjvtq0.execute-api.us-east-2.amazonaws.com"
#define API_KEY "aD07hFg8zbhdXvx361pQazN5nLqcygTQso1HSA30"


char ssid[] = "Giraffe"; //  your network SSID (name)
char pass[] = "r0bot1C5";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

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
  // TODO : implement this
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

const int getEmptyParkingSpotId() {
  Serial.println("Fetching empty parking spot");

  int statusCode;
  String response = httpRequest("GET", "/prod/parkingspot/empty", "", statusCode);

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

const bool updateParkingSpotStatus(const int id, const int newStatus) {
  Serial.println("Updating parking spot status");

  StaticJsonBuffer<128> jsonBuffer;
  JsonObject & requestBodyJson = jsonBuffer.createObject();
  requestBodyJson["id"] = id;
  requestBodyJson["newStatus"] = newStatus;

  char requestBody[128];
  requestBodyJson.printTo(requestBody, sizeof(requestBody));

  int statusCode;
  String response = httpRequest("PUT", "/prod/parkingspot", String(requestBody), statusCode);

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

String httpRequest(char method[], char resource[], const String body, int & statusCode) {
  Serial.print("Server request: ");
  Serial.print(method);
  Serial.print(" ");
  Serial.println(resource);
  Serial.println(body);
  
  if(client.connectSSL(HOST, PORT_SSL)) {
    client.print(method);
    client.print(" ");
    client.print(resource);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(HOST);
    client.print("x-api-key: ");
    client.println(API_KEY);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(body.length());
    client.println("Connection: close");
    client.println();
    client.println(body);

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
    statusCode = response.substring(statusCodeStartIndex, statusCodeStartIndex + 3).toInt();

    // parse response body
    int bodyStartIndex = response.lastIndexOf("\r\n");
    String body = response.substring(bodyStartIndex);

    Serial.print("Response: ");
    Serial.print(statusCode);
    Serial.print(" ");
    Serial.println(body);
    
    return body;
  } else {
    Serial.println("connection failed");
  }

  client.stop();
  statusCode = 0;
  return "";
}

