#include <Arduino.h>

// Pin definitions
const int interruptPin = 2;  // Pin to send interrupt signal to Arduino
const int sendPin = 4;       // Pin for sending data to Arduino

// Variable to store the data to be sent
int dataToSend = 0;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set interrupt pin as output
  pinMode(interruptPin, OUTPUT);

  // Set send pin as output
  pinMode(sendPin, OUTPUT);

  // Initialize data to be sent
  dataToSend = 0;
}

void loop() {
  // Check if there is data available from Serial monitor
  if (Serial.available() > 0) {
    // Read the data from Serial monitor
    dataToSend = Serial.parseInt();
    
    // Print the received data
    Serial.print("Data received: ");
    Serial.println(dataToSend);

    // Send interrupt signal to Arduino
    digitalWrite(interruptPin, HIGH);
    delay(100);  // Adjust delay as needed
    digitalWrite(interruptPin, LOW);

    // Wait a moment before sending data
    delay(100);

    // Send the data through the send pin
    digitalWrite(sendPin, dataToSend);
  }
}
