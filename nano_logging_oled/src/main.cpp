#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// SoftwareSerial pour lire les événements de l'ESP32
SoftwareSerial eventLog(10, 11); // RX=D10, TX=D11 (pas utilisé)

void setup() {
  Serial.begin(115200);
  eventLog.begin(9600);
  
  delay(500);
  Serial.println("\n\n=== Nano Logging & Display Module ===");
  Serial.println("Ready to log events from ESP32.");
}

void loop() {
  // Lire les événements de l'ESP32
  while (eventLog.available()) {
    String line = eventLog.readStringUntil('\n');
    Serial.print("Event logged: ");
    Serial.println(line);
  }
  
  delay(100);
}
