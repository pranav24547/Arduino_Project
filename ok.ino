#include <Arduino.h>

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    // Read data from ESP32 and print it
    String receivedData = Serial.readStringUntil('\n');
    Serial.println("Data received from ESP32: " + receivedData);
  }

  delay(100);
}
