#include <SoftwareSerial.h>

SoftwareSerial espSerial(1, 0); // RX, TX (Arduino Uno pins 2 and 3)

void setup() {
    Serial.begin(9600);
    espSerial.begin(9600);
    delay(1000);
    
    Serial.println("Sending AT commands to ESP8266...");
    espSerial.println("AT+RST"); // Reset the module
    delay(2000);
    espSerial.println("AT+CWMODE=1"); // Set the module to Station mode
    delay(1000);
    espSerial.println("AT+CWJAP=\"PranavA345G\",\"pranav06\""); // Connect to WiFi network
    delay(5000); // Wait for connection
    Serial.println("Setup completed.");
}

void loop() {
    // Check WiFi connection status
    if (checkWiFiConnection()) {
        Serial.println("Connected to WiFi");
    } else {
        Serial.println("WiFi not connected");
    }
    
    delay(5000); // Check every 5 seconds
}

bool checkWiFiConnection() {
    espSerial.println("AT+CWJAP?");
    delay(1000); // Wait for response
    
    while (espSerial.available()) {
        String response = espSerial.readStringUntil('\n');
        if (response.indexOf("OK") != -1) {
            return true; // WiFi connected
        }
    }
    
    return false; // WiFi not connected
}
