# Protocole JSON-UART

## Vue d'ensemble

Tous les microcontrôleurs (ESP32, Pico, Nano) communiquent via **UART 9600 baud, 8N1** en échangeant des **messages JSON terminés par `\n`**.

## Format Message Standard

```json
{
  "device": "esp32|pico|nano",
  "type": "event|command|status|ack",
  "timestamp": 1664123456,
  "seq": 12345,
  "payload": {...}
}
```

### Champs obligatoires
- `device`: Identifiant du micro qui envoie
- `type`: Type de message
- `timestamp`: Timestamp Unix (s'il y a RTC, sinon 0)
- `payload`: Données du message

### Champs optionnels
- `seq`: Numéro de séquence (pour ACK)
- `target`: Micro destinataire (pour command)

---

## Types de Messages

### 1. EVENT (Événement)

Émis par n'importe quel micro quand quelque chose se produit.

#### Event: WiFi - Appareil non autorisé détecté (ESP32)
```json
{
  "device": "esp32",
  "type": "event",
  "timestamp": 1664123456,
  "seq": 100,
  "payload": {
    "event_type": "wifi_unauthorized_device",
    "mac": "AA:BB:CC:DD:EE:FF",
    "ssid": "HomeNetwork",
    "signal_strength": -55,
    "first_seen": 1664123400
  }
}
```

#### Event: WiFi - Attaque détectée (ESP32)
```json
{
  "device": "esp32",
  "type": "event",
  "timestamp": 1664123456,
  "seq": 101,
  "payload": {
    "event_type": "wifi_attack_detected",
    "attack_type": "deauthentication|jamming|beacon_flood",
    "target_mac": "AA:BB:CC:DD:EE:FF",
    "intensity": 85,
    "duration_sec": 12
  }
}
```

#### Event: Capteur - Mouvement détecté (Pico)
```json
{
  "device": "pico",
  "type": "event",
  "timestamp": 1664123456,
  "seq": 50,
  "payload": {
    "event_type": "motion_detected",
    "pin": 18,
    "intensity": 255,
    "duration_ms": 500
  }
}
```

#### Event: Capteur - Porte/Fenêtre ouvert (Pico)
```json
{
  "device": "pico",
  "type": "event",
  "timestamp": 1664123456,
  "seq": 51,
  "payload": {
    "event_type": "door_opened",
    "pin": 19,
    "location": "front_door"
  }
}
```

#### Event: Capteur - Porte/Fenêtre fermé (Pico)
```json
{
  "device": "pico",
  "type": "event",
  "timestamp": 1664123456,
  "seq": 52,
  "payload": {
    "event_type": "door_closed",
    "pin": 19,
    "location": "front_door"
  }
}
```

#### Event: Logging - Événement enregistré (Nano)
```json
{
  "device": "nano",
  "type": "event",
  "timestamp": 1664123456,
  "seq": 200,
  "payload": {
    "event_type": "log_stored",
    "filename": "security_2026_10_01.csv",
    "total_events": 42,
    "sd_free_kb": 15360
  }
}
```

---

### 2. COMMAND (Commande)

Envoyé par ESP32 vers Pico/Nano pour contrôler les actuateurs.

#### Command: Activer Sirène (ESP32 → Pico)
```json
{
  "device": "esp32",
  "type": "command",
  "timestamp": 1664123456,
  "seq": 102,
  "target": "pico",
  "payload": {
    "cmd": "siren_on",
    "duration_ms": 5000,
    "frequency_hz": 1000
  }
}
```

#### Command: Désactiver Sirène (ESP32 → Pico)
```json
{
  "device": "esp32",
  "type": "command",
  "timestamp": 1664123456,
  "seq": 103,
  "target": "pico",
  "payload": {
    "cmd": "siren_off"
  }
}
```

#### Command: LED RGB (ESP32 → Pico)
```json
{
  "device": "esp32",
  "type": "command",
  "timestamp": 1664123456,
  "seq": 104,
  "target": "pico",
  "payload": {
    "cmd": "led_set_color",
    "color": "red|green|blue|yellow|cyan|magenta",
    "blink_interval_ms": 500
  }
}
```

#### Command: Verrouiller/Déverrouiller (ESP32 → Pico)
```json
{
  "device": "esp32",
  "type": "command",
  "timestamp": 1664123456,
  "seq": 105,
  "target": "pico",
  "payload": {
    "cmd": "lock_set",
    "state": "locked|unlocked"
  }
}
```

#### Command: Synchro RTC (ESP32 → Nano)
```json
{
  "device": "esp32",
  "type": "command",
  "timestamp": 1664123456,
  "seq": 106,
  "target": "nano",
  "payload": {
    "cmd": "rtc_set_time",
    "unix_timestamp": 1664123456
  }
}
```

---

### 3. STATUS (Statut)

Envoyé périodiquement par chaque micro pour indiquer son état.

#### Status: ESP32
```json
{
  "device": "esp32",
  "type": "status",
  "timestamp": 1664123456,
  "seq": 200,
  "payload": {
    "wifi_connected": true,
    "access_points_visible": 8,
    "connected_devices": 5,
    "unauthorized_devices": 2,
    "uptime_sec": 3600,
    "cpu_temp": 42.5,
    "heap_free_bytes": 102400,
    "last_scan": 1664123455
  }
}
```

#### Status: Pico
```json
{
  "device": "pico",
  "type": "status",
  "timestamp": 1664123456,
  "seq": 100,
  "payload": {
    "pir_armed": true,
    "door_contacts_armed": 3,
    "siren_ready": true,
    "led_status": "green",
    "battery_voltage": 12.4,
    "uptime_sec": 3600,
    "events_count": 15
  }
}
```

#### Status: Nano
```json
{
  "device": "nano",
  "type": "status",
  "timestamp": 1664123456,
  "seq": 50,
  "payload": {
    "sd_mounted": true,
    "rtc_synced": true,
    "oled_active": true,
    "sd_free_mb": 128,
    "logs_stored": 3500,
    "uptime_sec": 3600,
    "cpu_temp": 35.2
  }
}
```

---

### 4. ACK (Acquittement)

Envoyé en réponse à une COMMAND pour confirmer exécution.

#### ACK: Succès
```json
{
  "device": "pico",
  "type": "ack",
  "timestamp": 1664123456,
  "seq": 102,
  "payload": {
    "status": "ok",
    "cmd": "siren_on",
    "execution_time_ms": 45
  }
}
```

#### ACK: Erreur
```json
{
  "device": "pico",
  "type": "ack",
  "timestamp": 1664123456,
  "seq": 102,
  "payload": {
    "status": "error",
    "cmd": "siren_on",
    "error_code": "SIREN_NOT_ARMED",
    "error_msg": "Sirène non armée pour des raisons de sécurité"
  }
}
```

---

## Codes d'Erreur

| Code | Signification |
|------|---------------|
| `OK` | Succès |
| `INVALID_JSON` | Message JSON malformé |
| `UNKNOWN_CMD` | Commande inconnue |
| `DEVICE_NOT_FOUND` | Micro cible non trouvé |
| `PERMISSION_DENIED` | Pas de permission (ex: sirène non armée) |
| `TIMEOUT` | Timeout lors de l'exécution |
| `HARDWARE_ERROR` | Erreur matériel (capteur défaillant) |
| `SD_ERROR` | Erreur SD card |
| `RTC_NOT_SYNCED` | RTC non synchronisée |

---

## Flux de Communication

### Scénario 1: Détection Intrusion WiFi

```
ESP32: EVENT (unauthorized_device)
  ↓
ESP32 → Pico: COMMAND (siren_on + led_set red)
  ↓
Pico: ACK (ok)
  ↓
Pico: EVENT (siren_activated)
  ↓
ESP32 → Nano: COMMAND (rtc_set_time) [if not synced]
  ↓
Nano: ACK (ok)
  ↓
Nano: EVENT (log_stored) [après enregistrement]
```

### Scénario 2: Détection Mouvement + Fermeture Porte

```
Pico: EVENT (motion_detected + door_opened)
  ↓
Pico → ESP32: EVENT (motion_detected)
  ↓
ESP32: [Décision: mode intrus?]
  ↓
ESP32 → Pico: COMMAND (siren_on)
  ↓
Pico: ACK (ok)
  ↓
Pico → Nano: EVENT (motion_detected) [forward]
  ↓
Nano: EVENT (log_stored)
```

---

## Règles de Communication

1. **Tous les messages doivent être terminés par `\n`**
2. **Pas de chiffrement au démarrage** (ajouter SSL/TLS en production)
3. **Timeout ACK: 5 secondes** (retry après 2s)
4. **Séquence (seq): incrémenter à chaque message**
5. **Timestamp: 0 si RTC non disponible**
6. **Armement Sirène: TOUJOURS commander via JSON** (pas d'activation automatique)

---

## Parser JSON Recommandé

**Arduino/ESP32**: ArduinoJson (taille légère)
**Python (tests)**: json standard library

Voir `lib/` pour implémentations.
