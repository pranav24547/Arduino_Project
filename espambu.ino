#include <WiFi.h>

const char* ssid = "Pranav A34 5G";
const char* password = "pranav06";

const char* host = "192.168.81.149"; // Change this to your host address 
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
  double latitude1 = 11.024174;          // Default latitude value
  double longitude1 = 77.002804;         // Default longitude value
  String bloodpressure = "152/68";       // Blood pressure value
  float pulserate = 86.0;                 // Default pulse value
  
  Serial.print("latitude:");
  Serial.println(latitude1, 6); // Print with 6 decimal places

  Serial.print("longitude:");
  Serial.println(longitude1, 6); // Print with 6 decimal places

  Serial.print("bloodpressure:");
  Serial.println(bloodpressure);

  Serial.print("pulserate:");
  Serial.println(pulserate);


  // Prepare JSON payload
  String jsonPayload = "{";
  jsonPayload += "\"latitude1\":\"" + String(latitude1, 6) + "\",";
  jsonPayload += "\"longitude1\":\"" + String(longitude1, 6) + "\",";
  jsonPayload += "\"systolic\":\"" + bloodpressure.substring(0, bloodpressure.indexOf('/')) + "\",";
  jsonPayload += "\"diastolic\":\"" + bloodpressure.substring(bloodpressure.indexOf('/') + 1) + "\",";
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
    int httpResponseCode = -1; // Default value for error handling
    if (httpResponseHeader.length() >= 12) {
      httpResponseCode = httpResponseHeader.substring(9, 12).toInt(); // Extracting HTTP response code
    }
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    Serial.println("Server response done");
    client.stop();
  } else {
    Serial.println("Unable to connect to server");
  }
}
