#include "Arduino.h"
#include "http.h"
#include <SPI.h>
#include <WiFi101.h>

#define PORT_SSL 443

#define HOST "kf4ofjvtq0.execute-api.us-east-2.amazonaws.com"
#define API_KEY "aD07hFg8zbhdXvx361pQazN5nLqcygTQso1HSA30"

char ssid[] = "Giraffe"; //  your network SSID (name)
char pass[] = "r0bot1C5";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiClient client;


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


void setupWifi() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

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

