# Configuratie Directory

Deze directory bevat alle configuratiebestanden voor het AC Controller systeem. Deze bestanden bieden een modulaire manier om verschillende aspecten van het systeem aan te passen zonder de kern applicatiecode te wijzigen.

## Configuratiebestanden

### Kern Configuratie
- **`credentials.h`** - WiFi en MQTT inloggegevens (niet in git)
- **`credentials.h.template`** - Sjabloon voor het instellen van inloggegevens met test/productie omgevingen

### Systeem Configuratie  
- **`ac_units_config.h`** - AC unit definities en weergavenamen
- **`mqtt_config.h`** - MQTT topic structuur en communicatie mappings
- **`hardware_config.h`** - Hardware pinnen, timing en validatie instellingen
- **`ui_config.h`** - UI layout, kleuren, fonts en styling
- **`translations.h`** - Tekst strings en taal instellingen

## Snelle Aanpassingsgids

### AC Units Toevoegen/Wijzigen
Bewerk `ac_units_config.h`:
```cpp
ACUnit acUnits[] = {
  {
    "Weergave Naam",     // Nederlandse naam getoond in UI
    "mqtt_topic_name",   // MQTT topic identifier  
    22.0, false, 0, 0, 0, 22.0, 22.0  // Standaardwaarden
  },
  // ... meer units
};
```

### Kleuren/Layout Wijzigen
Bewerk `ui_config.h`:
```cpp
#define UI_COLOR_MODE_COOL 0x2B9AF9  // Blauw
#define UI_CARD_HEIGHT 50
#define UI_UNITS_PER_PAGE 4
```

### MQTT Topics Wijzigen
Bewerk `mqtt_config.h`:
```cpp
#define MQTT_BASE_TOPIC "hcy/airco"
#define MQTT_COMMAND_POWER "command/power"
```

### Hardware Porting
Bewerk `hardware_config.h`:
```cpp
#define XPT2046_CS 33
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
```

### Tekst Wijzigingen
Bewerk `translations.h`:
```cpp
#define TXT_APP_TITLE "AC Bediening HCY"
#define TXT_BACK "Terug"
```

### Inloggegevens Instellen
Kopieer `credentials.h.template` naar `credentials.h` en vul je waarden in:
```cpp
// Stel productie modus in
const bool PRODUCTION_MODE = false;  // false voor test, true voor productie

// Test omgeving
const char* TEST_WIFI_SSID = "jouw_test_wifi";
const char* TEST_MQTT_BROKER = "192.168.1.100";

// Productie omgeving  
const char* PROD_WIFI_SSID = "jouw_prod_wifi";
const char* PROD_MQTT_BROKER = "prod.mqtt.server";
```

Het systeem schakelt automatisch tussen test en productie inloggegevens op basis van de `PRODUCTION_MODE` boolean.

## Beveiligingsopmerking

Het `credentials.h` bestand bevat gevoelige informatie en wordt uitgesloten van versiebeheer via `.gitignore`. Gebruik altijd het sjabloonbestand om je inloggegevens in te stellen.