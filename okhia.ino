#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";
const char* serverAddress = "192.168.111.149";
const int serverPort = 8080;

const int interruptPin = 2; // Pin connected to Arduino's interrupt pin (D2)

WiFiClient client;
bool requestSent = false; // Flag to track whether the request has been sent

void setup() {
  pinMode(interruptPin, OUTPUT);

  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !requestSent) { // Check if connected and request not sent
    if (client.connect(serverAddress, serverPort)) {
      Serial.println("Connected to server. Sending GET request...");
      client.print(String("GET /RealTime/getSignalData?id=2 HTTP/1.1\r\n") + // Modify the URL to include the parameter
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

      if (doc.containsKey("lane1") && doc["lane1"].as<bool>()) {
        sendValue(1);
      } else if (doc.containsKey("lane2") && doc["lane2"].as<bool>()) {
        sendValue(2);
      } else if (doc.containsKey("lane3") && doc["lane3"].as<bool>()) {
        sendValue(3);
      } else if (doc.containsKey("lane4") && doc["lane4"].as<bool>()) {
        sendValue(4);
      } else {
        Serial.println("No active lanes found.");
      }
    } else {
      Serial.println("Failed to connect to server");
    }
  } else if (!requestSent) {
    Serial.println("Not connected to WiFi or request already sent");
    connectToWiFi();
  }

  delay(5000); // Adjust the delay as needed
}

void sendValue(int value) {
  // Send interrupt signal to Arduino
  digitalWrite(interruptPin, HIGH);
  delay(1000); // Delay to keep interrupt signal HIGH for a moment
  digitalWrite(interruptPin, LOW);

  // Delay before sending the entered value
  delay(5000);

  // Send the entered value to Arduino
  Serial.println(value);

  requestSent = true; // Set flag to true after request is sent
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
