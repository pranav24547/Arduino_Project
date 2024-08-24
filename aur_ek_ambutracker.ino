#include <WiFi.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";

const char* host = "192.168.55.149"; // Change this to your host address 
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
  double latitude1 = 11.024217;     // Predefined latitude value
  double longitude1 = 77.002785;    // Predefined longitude value
  String bloodpressure = "160/82";  // Predefined blood pressure value
  float pulserate = 84;             // Predefined pulse value

  // Print values to Serial Monitor
  Serial.print("Latitude: ");
  Serial.println(latitude1);
  Serial.print("Longitude: ");
  Serial.println(longitude1);
  Serial.print("Blood Pressure: ");
  Serial.println(bloodpressure);
  Serial.print("Pulse Rate: ");
  Serial.println(pulserate);

  // Prepare JSON payload
  String jsonPayload = "{";
  jsonPayload += "\"latitude1\":\"" + String(latitude1, 6) + "\",";
  jsonPayload += "\"longitude1\":\"" + String(longitude1, 6) + "\",";
  jsonPayload += "\"bloodPressure\":\"" + bloodpressure + "\",";
  jsonPayload += "\"pulseRate\":\"" + String(pulserate) + "\",";
  jsonPayload += "\"ambId\":\"69\"";
  jsonPayload += "}";

  // Send HTTP POST request
  sendPostRequest(jsonPayload);

  // Wait for a moment before sending the next request
  delay(5000);
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
