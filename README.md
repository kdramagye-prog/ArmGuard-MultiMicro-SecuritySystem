# ArmGuard-Sénégal

Prototype de sécurité locale autour d'un ESP32-D0WD-V3, d'un Raspberry Pi Pico et d'un Arduino Nano classique (ATmega328P).

## Ce qui fonctionne dans cette version

- Le Pico lit un PIR, un contact de porte et un bouton; il commande une sortie relais et une LED d'alerte.
- Un appui bref sur le bouton déclenche l'alarme. Maintenir le bouton 1,5 s arme ou désarme le système. Une alarme est mémorisée jusqu'au désarmement.
- L'ESP32 reçoit les événements du Pico, réalise un scan des points d'accès Wi-Fi visibles toutes les 30 s et sert un tableau de bord local en lecture seule.
- Le Nano reçoit une copie des événements, les affiche sur un OLED SSD1306 et les ajoute à `/events.csv` sur une carte microSD, horodatés si le DS3231 est présent.

Le scan Wi-Fi est passif au sens où il ne se connecte pas aux réseaux découverts et n'envoie pas de trames d'attaque. Il dénombre les points d'accès qui annoncent leur présence. Il ne voit pas les appareils clients, ne prouve pas une intrusion et ne détecte pas de façon fiable le brouillage ni les trames de désauthentification. La détection d'intrusion réseau nécessiterait des données du routeur ou un capteur radio dédié.

Les notifications Telegram, SMS/GSM et e-mail, le servo de verrouillage, l'authentification du tableau de bord et les schémas Fritzing ne sont pas implémentés dans ce prototype. Ne présente pas ces fonctions comme disponibles lors d'une démonstration. Le tableau de bord doit rester sur un réseau local de confiance.

## Construction

Installer PlatformIO Core, puis depuis ce dossier:

```sh
pio run -e esp32
pio run -e pico
pio run -e nano
```

Les dépendances OLED et RTC du Nano sont déclarées dans `platformio.ini`. La cible Pico utilise la plateforme Arduino-Pico de `maxgerhardt`. Pour le moniteur série, sélectionner l'environnement voulu et utiliser `pio device monitor -b 115200`.

Avant de flasher l'ESP32, renseigner `ARMGuard_WIFI_SSID` et `ARMGuard_WIFI_PASSWORD` dans `include/config.h` pour rendre le tableau de bord accessible sur le Wi-Fi. Ne publier ni ces identifiants ni une capture d'écran qui les révèle. Sans SSID configuré, la partie capteurs et journalisation fonctionne encore, mais aucun tableau de bord réseau n'est accessible.

## Mise en service

1. Vérifier le câblage décrit dans [docs/wiring.md](docs/wiring.md), en particulier la masse commune et l'interface du relais.
2. Flasher et tester le Pico seul: vérifier les états du PIR/contact et le clic bref/long du bouton avant de connecter la sirène.
3. Connecter l'ESP32 et vérifier la réception des événements sur le moniteur série. Configurer ensuite les identifiants Wi-Fi et consulter l'adresse IP annoncée par le routeur.
4. Ajouter le Nano, l'OLED, le DS3231 et la carte SD. Vérifier l'affichage `SD: OK` et `RTC: OK`, puis contrôler le fichier `/events.csv`.
5. Tester avec une LED ou une charge basse tension avant toute sirène. Une alarme réelle requiert une alimentation dimensionnée et un étage de puissance adapté.

## Protocole inter-cartes

L'ESP32 et le Pico échangent des lignes ASCII terminées par `\n`, à 9600 bauds, format `TYPE|nom|0-ou-1`. Exemples: `EV|pir|1`, `EV|door|1`, `ST|armed|1`, `ST|alarm|1`. La sortie TX de l'ESP32 est également reliée à l'entrée SoftwareSerial du Nano pour journaliser les mêmes lignes.

Voir [docs/PROTOCOL.md](docs/PROTOCOL.md) pour les directions série, les messages et le comportement au démarrage.

## Limites matérielles

Cette version vise une carte Nano ATmega328P 5 V avec 2 Ko de SRAM. Les capteurs et le relais ne doivent pas être alimentés depuis une broche GPIO. Ne raccorde jamais une sortie 5 V à une entrée ESP32/Pico 3,3 V. Voir les précautions détaillées dans le guide de câblage.
