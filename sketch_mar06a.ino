void setup() {
  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);
}

void loop() {
  // Print "Hello, World!" to the serial monitor
  Serial.println("Hello, World!");

  // Wait for a brief moment
  delay(1000); // 1 second delay
}
