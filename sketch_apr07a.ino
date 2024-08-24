// Example code for ESP32 sending an interrupt signal to Arduino after receiving a value from the serial monitor

const int interruptPin = 2; // Pin connected to Arduino's interrupt pin (D2)

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the value entered in the serial monitor
    int value = Serial.parseInt();

    // Check if the value is within the valid range (1 to 4)
    if (value >= 1 && value <= 4) {
      // Send interrupt signal to Arduino
      digitalWrite(interruptPin, HIGH);
      delay(1000); // Delay to keep interrupt signal HIGH for a moment
      digitalWrite(interruptPin, LOW);

      // Delay before sending the entered value
      delay(5000);

      // Send the entered value to Arduino
      Serial.println(value);
    } else {
      // Prompt the user to enter a valid value
      Serial.println("Please enter a value between 1 and 4.");
    }
  }
}
