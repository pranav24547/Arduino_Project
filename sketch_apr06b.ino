#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("2"); // Send the value '3' to Arduino
  delay(1000); // Adjust delay as needed
}
