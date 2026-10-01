#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "../include/config.h"

// UART pour Pico
HardwareSerial picoCom(1);

void setup() {
  Serial.begin(115200);
  picoCom.begin(9600, SERIAL_8N1, ESP32_PICO_RX_PIN, ESP32_PICO_TX_PIN);
  
  delay(1000);
  Serial.println("\n\n=== ESP32 WiFi Security Module ===");
  Serial.println("Initializing...");
  
  // Initialiser WiFi
  WiFi.mode(WIFI_STA);
  if (strlen(ARMGuard_WIFI_SSID) > 0) {
    WiFi.begin(ARMGuard_WIFI_SSID, ARMGuard_WIFI_PASSWORD);
    Serial.println("WiFi: Connecting...");
  } else {
    Serial.println("WiFi: No SSID configured (dashboard offline)");
  }
  
  Serial.println("Setup complete.");
}

void loop() {
  // Lire les événements du Pico
  while (picoCom.available()) {
    String line = picoCom.readStringUntil('\n');
    Serial.print("From Pico: ");
    Serial.println(line);
  }
  
  delay(100);
}
