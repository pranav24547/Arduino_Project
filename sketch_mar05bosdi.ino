#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* serverAddress = "yourserver.com";
const int serverPort = 80; // HTTP port usually 80

const int trigpin1 = 13;
const int echopin1 = 12;
const int trigpin2 = 11;
const int echopin2 = 10;
const int trigpin3 = 9;
const int echopin3 = 8;
const int trigpin4 = 7;
const int echopin4 = 6;
const int led1 = 5;
const int led2 = 4;
const int led3 = 3;
const int led4 = 2;
long duration1, duration2, duration3, duration4;
int distance1, distance2, distance3, distance4;

WiFiClient client;

void setup() {
  Serial.begin(9600);

  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(trigpin2, OUTPUT);
  pinMode(echopin2, INPUT);
  pinMode(trigpin3, OUTPUT);
  pinMode(echopin3, INPUT);
  pinMode(trigpin4, OUTPUT);
  pinMode(echopin4, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Connect to WiFi
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

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  CalculateDistances();

  if (distance1 <= 20 || distance2 <= 20) {
    digitalWrite(led1, HIGH);
    delay(10000);
    digitalWrite(led1, LOW);
  } else {
    digitalWrite(led1, HIGH);
    delay(5000);
    digitalWrite(led1, LOW);
  }

  digitalWrite(led2, HIGH);
  delay(5000);
  digitalWrite(led2, LOW);

  if (distance3 <= 20 || distance4 <= 4) {
    digitalWrite(led3, HIGH);
    delay(10000);
    digitalWrite(led3, LOW);
  } else {
    digitalWrite(led3, HIGH);
    delay(5000);
    digitalWrite(led3, LOW);
  }

  digitalWrite(led4, HIGH);
  delay(5000);
  digitalWrite(led4, LOW);
}

void CalculateDistances() {
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigpin1, LOW);
  duration1 = pulseIn(echopin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.println("Sensor 1");
  Serial.println(distance1);
  delay(1000);

  digitalWrite(trigpin2, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigpin2, LOW);
  duration2 = pulseIn(echopin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  Serial.println("Sensor 2");
  Serial.println(distance2);
  delay(1000);

  digitalWrite(trigpin3, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigpin3, LOW);
  duration3 = pulseIn(echopin3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  Serial.println("Sensor 3");
  Serial.println(distance3);
  delay(1000);

  digitalWrite(trigpin4, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigpin4, LOW);
  duration4 = pulseIn(echopin4, HIGH);
  distance4 = duration4 * 0.034 / 2;
  Serial.println("Sensor 4");
  Serial.println(distance4);
  delay(1000);
}

void serialEvent() {
  while (Serial.available() > 0) {
    int inputValue = Serial.parseInt();
    if (inputValue >= 1 && inputValue <= 4) {
      httpRequest(inputValue);
    }
  }
}

void httpRequest(int value) {
  if (client.connect(serverAddress, serverPort)) {
    Serial.println("Connected to server");

    // Make HTTP GET request
    client.print("GET /your_endpoint?value=");
    client.print(value);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.print(serverAddress);
    client.print("\r\n");
    client.print("Connection: close\r\n\r\n");

    Serial.println("Request sent");

    // Wait for response
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    // Close connection
    client.stop();
    Serial.println("Connection closed");
  } else {
    Serial.println("Connection failed");
  }
}
