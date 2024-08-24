#include <Arduino.h>

const int interruptPin = 2; // Pin connected to interrupt from ESP32

volatile bool receivedInterrupt = false; // Flag to indicate interrupt received
volatile bool newDataReceived = false; // Flag to indicate new data received over Serial

// Define LED pins and variables
const int ledPins[] = {5, 4, 3, 6};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
volatile bool interruptFlag = false;
volatile int receivedLed = 0; // Variable to store received LED number

unsigned long previousMillis = 0;
const long interval = 15000; // 15 seconds
const long ledDuration = 25000; // 25 seconds

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);

  // Set the interrupt pin as input
  pinMode(interruptPin, INPUT_PULLUP);

  // Attach an interrupt to the pin
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);

  // Set LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs are initially off
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle LED turning on for 15 seconds in the normal loop
  if (!interruptFlag && (currentMillis - previousMillis >= interval)) {
    previousMillis = currentMillis;
    static int currentLed = 0;
    turnOffAllLEDs(); // Turn off all LEDs
    digitalWrite(ledPins[currentLed], HIGH); // Turn on current LED
    currentLed = (currentLed + 1) % numLeds; // Move to the next LED
  }

  if (interruptFlag && !receivedInterrupt) {
    // Print a message indicating interrupt received
    Serial.println("Interrupt received from ESP32");

    receivedInterrupt = true; // Set receivedInterrupt to true
  }

  if (receivedInterrupt) {
    // Check if new data is available
    if (newDataReceived) {
      // Read the received LED number
      int receivedValue = Serial.parseInt();

      // If received LED number is valid, turn on the corresponding LED and start the timer
      if (receivedValue >= 1 && receivedValue <= numLeds) {
        // Print statement for valid LED number received
        Serial.print("Received LED number: ");
        Serial.println(receivedValue);

        // Turn off all LEDs except the received LED
        turnOffAllLEDs();
        digitalWrite(ledPins[receivedValue - 1], HIGH); // Turn on the selected LED

        // Record the start time of LED activation
        unsigned long startTime = millis();

        // Loop until the 25 seconds duration is over
        while (millis() - startTime < ledDuration) {
          // Check if new data is available
          if (Serial.available()) {
            newDataReceived = true;
            break;
          }
        }

        // Turn off the LED after the specified duration
        turnOffAllLEDs();
        receivedInterrupt = false; // Reset receivedInterrupt
        interruptFlag = false; // Reset interrupt flag
      }

      // Reset the flag
      newDataReceived = false;
    }
  }
}

void handleInterrupt() {
  // Called when there's a change in state on the interrupt pin
  static unsigned long lastInterruptTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime > 200) { // Debouncing time
    interruptFlag = true;
    lastInterruptTime = currentMillis;
  }
}

void serialEvent() {
  // Called when new data is available on Serial
  newDataReceived = true;
}

void turnOffAllLEDs() {
  // Turn off all LEDs
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
