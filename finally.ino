#include <WiFi.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";

const char* host = "192.168.241.149"; // Change this to your host address
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
  double latitude1 = 0;          // Default latitude value
  double longitude1 = 0;         // Default longitude value
  String bloodpressure;        // Blood pressure value in the format "152.5/95.2"
  float pulserate = 0;             // Default pulse value

  // Prompt user to enter latitude from Serial Monitor
  Serial.println("Please enter latitude:");
  latitude1 = readDoubleFromSerial();

  // Prompt user to enter longitude from Serial Monitor
  Serial.println("Please enter longitude:");
  longitude1 = readDoubleFromSerial();

  // Prompt user to enter blood pressure from Serial Monitor
  Serial.println("Please enter blood pressure :");
  bloodpressure = readStringFromSerial();

  // Prompt user to enter pulse from Serial Monitor
  Serial.println("Please enter pulse:");
  pulserate = readFloatFromSerial();

  // Prepare JSON payload
  String jsonPayload = "{";
  jsonPayload += "\"latitude\":\"" + String(latitude1, 6) + "\",";
  jsonPayload += "\"longitude\":\"" + String(longitude1, 6) + "\",";
  jsonPayload += "\"bloodPressure\":\"" + bloodpressure + "\",";
  jsonPayload += "\"pulse\":\"" + String(pulserate) + "\",";
  jsonPayload += "\"ambId\":\"8\"";
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
  WiFiClient client;

  if (client.connect(host, port)) {
    Serial.println("Connected to server");

    // Make an HTTP POST request
    client.print("POST /RealTime/postData HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(host);
    client.print("\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Length: ");
    client.print(payload.length());
    client.print("\r\n\r\n");
    client.print(payload);

    // Wait for the server to respond
    delay(100);

    // Read and print server response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    // Read HTTP response code
    String httpResponseHeader = client.readStringUntil('\n');
    int httpResponseCode = httpResponseHeader.substring(9, 12).toInt(); // Extracting HTTP response code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    Serial.println("Server response done");
    client.stop();
  } else {
    Serial.println("Unable to connect to server");
  }
}
