#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char* ssid = "Pranav A34 5G1";
const char* password = "pranav06";
const char* serverAddress = "192.168.55.149";
const int serverPort = 8080;

const int interruptPin = D1; // Pin for sending interrupt to another Arduino (change as needed)
const int digitalPin = D2;   // Pin for sending a digital signal (change as needed)

WiFiClient client;
bool requestSent = false; // Flag to track whether the request has been sent

void setup() {
  pinMode(interruptPin, OUTPUT);
  pinMode(digitalPin, OUTPUT);

  Serial.begin(115200);

  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !requestSent) { // Check if connected and request not sent
    if (client.connect(serverAddress, serverPort)) {
      Serial.println("Connected to server. Sending GET request...");
      client.print(String("GET /RealTime/getSignalData?id=1 HTTP/1.1\r\n") + // Modify the URL to include the parameter
                   "Host: " + serverAddress + "\r\n" +
                   "Connection: close\r\n\r\n");

      delay(100);

      String response = "";
      while (client.available()) {
        response += client.readStringUntil('\r');
      }
      client.stop();

      Serial.println("Response received:");
      Serial.println(response);

      // Parse JSON response to extract lane information
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response);

      bool lane1 = doc["lane1"];
      bool lane2 = doc["lane2"];
      bool lane3 = doc["lane3"];
      bool lane4 = doc["lane4"];

      int laneNumber = 3; // Default lane number

      if (lane1) laneNumber = 1;
      else if (lane2) laneNumber = 2;
      else if (lane3) laneNumber = 3;
      else if (lane4) laneNumber = 4;

      // Print lane number for debugging
      Serial.print("Lane number: ");
      Serial.println(laneNumber);

      // Send interrupt signal with the determined lane number
      sendInterrupt();
      Serial.println("intruptt sent");
      digitalWrite(digitalPin, HIGH); // Send HIGH signal to indicate lane is active
      delay(100); // Adjust the delay as needed
      digitalWrite(digitalPin, LOW); // Reset the pin
      delay(100); // Adjust the delay as needed
      digitalWrite(digitalPin, laneNumber); // Send determined lane number to Arduino Uno

      requestSent = true; // Set flag to true after request is sent
    } else {
      Serial.println("Failed to connect to server");
    }
  } else if (!requestSent) {
    Serial.println("Not connected to WiFi or request already sent");
    connectToWiFi();
  }

  delay(5000); // Adjust the delay as needed
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }

  Serial.println("Connected to WiFi");
}

void sendInterrupt() {
  digitalWrite(interruptPin, HIGH);
  delay(100);
  digitalWrite(interruptPin, LOW);
}
