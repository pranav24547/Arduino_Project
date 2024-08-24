#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";
const char* serverAddress = "192.168.111.149";
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

      // Extract data from JSON
      id = response.substring(response.indexOf("\"id\":") + 5, response.indexOf(',', response.indexOf("\"id\":"))).toInt();
      name = response.substring(response.indexOf("\"name\":\"") + 8, response.indexOf('"', response.indexOf("\"name\":\"") + 9));
      lane1 = response.substring(response.indexOf("\"lane1\":") + 8, response.indexOf(',', response.indexOf("\"lane1\":"))) == "true";
      lane2 = response.substring(response.indexOf("\"lane2\":") + 8, response.indexOf(',', response.indexOf("\"lane2\":"))) == "true";
      lane3 = response.substring(response.indexOf("\"lane3\":") + 8, response.indexOf(',', response.indexOf("\"lane3\":"))) == "true";
      lane4 = response.substring(response.indexOf("\"lane4\":") + 8, response.indexOf(',', response.indexOf("\"lane4\":"))) == "true";

      // Now you can use the extracted data as needed

      // Example:
      Serial.print("ID: ");
      Serial.println(id);
      Serial.print("Name: ");
      Serial.println(name);
      Serial.print("Lane 1: ");
      Serial.println(lane1);
      Serial.print("Lane 2: ");
      Serial.println(lane2);
      Serial.print("Lane 3: ");
      Serial.println(lane3);
      Serial.print("Lane 4: ");
      Serial.println(lane4);

      //requestSent = true; // Set flag to true after request is sent
    }
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
