# ArmGuard — Système de sécurité multi-microcontrôleurs

Projet de concours basé sur un ESP32-D0WD-V3, un Raspberry Pi Pico et un Arduino Nano. Le système combine surveillance Wi-Fi défensive, capteurs physiques et journalisation locale.

## Architecture

- **ESP32-D0WD-V3** : point central, scan Wi-Fi passif, interface Web locale et émission des événements.
- **Raspberry Pi Pico** : PIR, contacts de portes/fenêtres, bouton d'urgence, buzzer/sirène et LED.
- **Arduino Nano** : journalisation de secours et affichage optionnel.
- **Liaison** : messages JSON terminés par `\n` sur UART, spécifiés dans [`docs/PROTOCOL.md`](docs/PROTOCOL.md).

Les sorties dangereuses sont initialisées à l'état inactif et ne sont jamais activées automatiquement au démarrage.

## Structure prévue

```text
esp32_wifi_security/     # Firmware ESP32
pico_sensors_control/    # Firmware Pico
nano_logging_oled/       # Firmware Nano
docs/                    # câblage et protocole
tests/                   # tests hors matériel
```

## Sécurité et périmètre

Ce projet est défensif : il surveille uniquement les réseaux et équipements pour lesquels tu as une autorisation. Il ne contient pas de fonctions de désauthentification, de brouillage, de récupération de mots de passe ou de portail de collecte d'identifiants.

Avant de brancher les cartes :

1. Relie toutes les masses (`GND`) ensemble.
2. Vérifie les niveaux logiques : le Pico et l'ESP32 sont en 3,3 V ; protège l'entrée du Nano si nécessaire.
3. Teste d'abord avec une LED et un buzzer basse tension, jamais avec une sirène ou une serrure réelle.
4. Ajoute un fusible et un transistor/MOSFET pour les charges externes.

## Compilation

Chaque dossier firmware sera un environnement PlatformIO indépendant. Les commandes générales sont :

```bash
pio run
pio device list
pio run -t upload --upload-port COM4
pio device monitor -b 115200
```

Le port `COM4` peut changer selon le câble et le système. La carte ESP32-D0WD-V3 doit généralement être sélectionnée comme `esp32dev` ou `esp32doit-devkit-v1` selon le module utilisé.

## État

Le dépôt contient actuellement le protocole et la documentation initiale. Les firmwares seront ajoutés par étapes afin de pouvoir compiler et tester chaque carte séparément.

## Licence

MIT — voir `LICENSE`.
