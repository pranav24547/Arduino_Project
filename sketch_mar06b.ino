#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX, TX (Arduino Uno pins 2 and 3)

void setup() {
    Serial.begin(9600);
    espSerial.begin(9600);
    delay(1000);
    
    Serial.println("Sending AT commands to ESP8266...");
    espSerial.println("AT+RST"); // Reset the module
    delay(2000);
    espSerial.println("AT+CWMODE=1"); // Set the module to Station mode
    delay(1000);
    espSerial.println("AT+CWJAP=\"Pranav A34 5G\",\"pranav06\""); // Connect to WiFi network
    delay(5000);
    Serial.println("Setup completed.");
}

void loop() {
    
}
