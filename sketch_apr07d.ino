#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";
const char* serverAddress = "192.168.81.149";
const int serverPort = 8080;

const int interruptPin = 2; // Pin connected to Arduino's interrupt pin (D2)

WiFiClient client;
bool requestSent = false; // Flag to track whether the request has been sent

// Variables to store JSON data
int id;
String name;
bool lane1;
bool lane2;
bool lane3;
bool lane4;

void setup() {
  pinMode(interruptPin, OUTPUT);

  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // Check if connected
    if (!requestSent) { // Check if request not sent
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

        // Extract data from JSON
        id = response.substring(response.indexOf("\"id\":") + 5, response.indexOf(',', response.indexOf("\"id\":"))).toInt();
        name = response.substring(response.indexOf("\"name\":\"") + 8, response.indexOf('"', response.indexOf("\"name\":\"") + 9));
        lane1 = response.substring(response.indexOf("\"lane1\":") + 8, response.indexOf(',', response.indexOf("\"lane1\":"))) == "true";
        lane2 = response.substring(response.indexOf("\"lane2\":") + 8, response.indexOf(',', response.indexOf("\"lane2\":"))) == "true";
        lane3 = response.substring(response.indexOf("\"lane3\":") + 8, response.indexOf(',', response.indexOf("\"lane3\":"))) == "true";
        lane4 = response.substring(response.indexOf("\"lane4\":") + 8, response.indexOf(',', response.indexOf("\"lane4\":"))) == "true";

        // Send value corresponding to lane with value 1
        if (lane1) {
          sendValue(1);
        } 
        if (lane2) {
          sendValue(2);
        } 
        if (lane3) {
          sendValue(3);
        } 
        if (lane4) {
          sendValue(4);
        }

        requestSent = true; // Set flag to true after request is sent
      }
    }
    // Reset requestSent flag to false to make another request
    requestSent = false;
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
