#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";

const char* host = "192.168.248.12"; // Change this to your host address
const int port = 8080; // Change this to your host port

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  double latitude = 0;          // Default latitude value
  double longitude = 0;         // Default longitude value
  String bloodPressure;        // Blood pressure value in the format "152.5/95.2"
  float pulse = 0;             // Default pulse value

  // Prompt user to enter latitude from Serial Monitor
  Serial.println("Please enter latitude:");
  latitude = readDoubleFromSerial();

  // Prompt user to enter longitude from Serial Monitor
  Serial.println("Please enter longitude:");
  longitude = readDoubleFromSerial();

  // Prompt user to enter blood pressure from Serial Monitor
  Serial.println("Please enter blood pressure :");
  bloodPressure = readStringFromSerial();

  // Prompt user to enter pulse from Serial Monitor
  Serial.println("Please enter pulse:");
  pulse = readFloatFromSerial();

  // Prepare JSON payload
  String jsonPayload = "{";
  jsonPayload += "\"latitude\":\"" + String(latitude, 6) + "\",";
  jsonPayload += "\"longitude\":\"" + String(longitude, 6) + "\",";
  jsonPayload += "\"bloodPressure\":\"" + bloodPressure + "\",";
  jsonPayload += "\"pulse\":\"" + String(pulse) + "\",";
  jsonPayload += "\"ambId\":\"6\"";
  jsonPayload += "}";

  // Send HTTP POST request
  sendPostRequest(jsonPayload);

  // Wait for a moment before sending the next request
  delay(5000);
}

double readDoubleFromSerial() {
  // Wait for input
  while (Serial.available() == 0) {
    delay(100); // Add a small delay to reduce CPU load
  }

  // Read double value from Serial Monitor
  double value = Serial.parseFloat();

  // Clear the input buffer
  while (Serial.available() > 0) {
    Serial.read();
  }

  return value;
}

String readStringFromSerial() {
  // Wait for input
  while (Serial.available() == 0) {
    delay(100); // Add a small delay to reduce CPU load
  }

  // Read string value from Serial Monitor
  String value = Serial.readStringUntil('\n');

  // Trim leading and trailing whitespaces
  value.trim();

  // Clear the input buffer
  while (Serial.available() > 0) {
    Serial.read();
  }

  return value;
}

float readFloatFromSerial() {
  // Wait for input
  while (Serial.available() == 0) {
    delay(100); // Add a small delay to reduce CPU load
  }

  // Read float value from Serial Monitor
  float value = Serial.parseFloat();

  // Clear the input buffer
  while (Serial.available() > 0) {
    Serial.read();
  }

  return value;
}

void sendPostRequest(String payload) {
  HTTPClient http;

  if (http.begin(host, port, "/RealTime/postData")) {
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println("Server response: " + response);

    http.end();
  } else {
    Serial.println("Unable to connect to server");
  }
}
