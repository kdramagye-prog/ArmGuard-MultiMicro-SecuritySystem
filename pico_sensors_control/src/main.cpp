#include <Arduino.h>
#include <ArduinoJson.h>

// Pins
const uint8_t PIR_PIN = 18;
const uint8_t DOOR_SENSOR_PIN = 19;
const uint8_t BUTTON_PIN = 20;
const uint8_t SIREN_PIN = 21;
const uint8_t LED_PIN = 22;

// UART vers ESP32
Serial1 esp32Com;

uint32_t lastEventTime = 0;
bool armed = false;
bool alarm_state = false;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  
  // Initialiser pins
  pinMode(PIR_PIN, INPUT);
  pinMode(DOOR_SENSOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SIREN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Sécurité: sirène et LED désactivées au démarrage
  digitalWrite(SIREN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  delay(500);
  Serial.println("\n\n=== Pico Sensors & Control Module ===");
  Serial.println("All outputs armed (inactive state).");
  Serial.println("Setup complete.");
}

void loop() {
  // Lire bouton
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);
    if (digitalRead(BUTTON_PIN) == LOW) {
      uint32_t press_start = millis();
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
      uint32_t press_duration = millis() - press_start;
      
      if (press_duration > 1500) {
        // Long press: toggle armed
        armed = !armed;
        Serial.print("Armed: ");
        Serial.println(armed ? "yes" : "no");
        digitalWrite(LED_PIN, armed ? HIGH : LOW);
      } else {
        // Short press: trigger alarm if armed
        if (armed) {
          alarm_state = true;
          digitalWrite(SIREN_PIN, HIGH);
          Serial.println("ALARM TRIGGERED");
        }
      }
      delay(200);
    }
  }
  
  // Lire PIR
  static int last_pir = 0;
  int pir = digitalRead(PIR_PIN);
  if (pir != last_pir) {
    Serial.print("PIR: ");
    Serial.println(pir);
    last_pir = pir;
  }
  
  // Lire contact porte
  static int last_door = 0;
  int door = digitalRead(DOOR_SENSOR_PIN);
  if (door != last_door) {
    Serial.print("DOOR: ");
    Serial.println(door);
    last_door = door;
  }
  
  delay(50);
}
