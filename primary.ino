// Define LED pins
const int ledPins[] = {5, 4, 3, 2};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
volatile bool interruptFlag = false;
volatile int receivedLed = 0; // Variable to store received LED number

void setup() {
  // Set LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs are initially off
  }

  // Attach interrupt to pin 0 (for receiving the interrupt signal)
  attachInterrupt(digitalPinToInterrupt(0), interruptHandler, CHANGE);

  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  // Handle LED turning on for 15 seconds
  static int currentLed = 0;
  static unsigned long previousMillis = 0;
  const long interval = 15000; // 15 seconds

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    turnOffAllLEDs(); // Turn off all LEDs
    digitalWrite(ledPins[currentLed], HIGH); // Turn on current LED
    currentLed = (currentLed + 1) % numLeds; // Move to the next LED
  }

  // Check if an interrupt occurred
  if (interruptFlag) {
    // Reset interrupt flag
    interruptFlag = false;

    // Handle interrupt
    handleInterrupt();
  }
}

void interruptHandler() {
  // Print statement for interrupt received
  Serial.println("Interrupt received");

  // Nothing else to do here, interrupt is used to trigger the handleInterrupt function
}

void handleInterrupt() {
  // Read the received LED number from digital input pin 1
  receivedLed = digitalRead(1);

  // If received LED number is valid, turn on the corresponding LED
  if (receivedLed >= 1 && receivedLed <= numLeds) {
    // Print statement for valid LED number received
    Serial.print("Received LED number: ");
    Serial.println(receivedLed);

    // Turn off all LEDs except the received LED
    turnOffAllLEDs();
    digitalWrite(ledPins[receivedLed - 1], HIGH); // Turn on the selected LED
  }
}

void turnOffAllLEDs() {
  // Turn off all LEDs
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
