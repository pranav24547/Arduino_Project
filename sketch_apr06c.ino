#include <Arduino.h>

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  // Check if there is data available in the serial buffer
  if (Serial.available() > 0) {
    // Read the value entered in the serial monitor
    int value = Serial.parseInt();

    // Check if a valid integer was found
    if (Serial.peek() != '\n') {
      // Print the value back to the serial monitor
      Serial.print("You entered: ");
      Serial.println(value);
    }

    // Clear the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
