void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == "1") { // Compare with the string "1"
      // Send an interrupt
      digitalWrite(2, HIGH); // Simulating an interrupt on pin 2
      delayMicroseconds(50); // Ensure a stable signal
      digitalWrite(2, LOW);
      Serial.println("Interrupt sent.");
    }
  }
}
