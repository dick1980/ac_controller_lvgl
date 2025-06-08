# AC Controller LVGL voor ESP32-2432S028

Een geavanceerde airconditioning controller interface voor de ESP32-2432S028 development board met LVGL GUI. Dit systeem biedt een moderne touchscreen interface voor het bedienen van meerdere AC units via MQTT communicatie met Home Assistant.

## Hardware Specificaties

### ESP32-2432S028 Development Board
- **Processor**: ESP32 dual-core processor
- **Display**: 2.8" TFT LCD 240x320 pixels
- **Touch**: XPT2046 resistive touchscreen controller
- **WiFi**: Ingebouwde ESP32 WiFi-module
- **Flash**: 4MB flash geheugen

### Pin Configuratie
```cpp
// Touch controller pins
#define XPT2046_CS 33
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25

// Backlight control
#define TFT_BL 21
```

## Software Architectuur

### Core Componenten

#### 1. LVGL Graphics Library (v9.x)
- **Bestandslocatie**: `lv_conf.h`
- **Functie**: Moderne grafische user interface met touch support
- **Features**: Anti-aliasing, animaties, theming, responsive layouts

#### 2. MQTT Client (PubSubClient)
- **Bestandslocatie**: `ac_controller_lvgl.ino` (lijnen 7, 42-43)
- **Functie**: Communicatie met Home Assistant
- **Topics**: 
  - Command: `hcy/airco/{unit_name}/command/{parameter}`
  - Status: `hcy/airco/{unit_name}/status`

#### 3. Touchscreen Controller (XPT2046)
- **Bestandslocatie**: `ac_controller_lvgl.ino` (lijnen 34-38, 292-298)
- **Functie**: Touch input verwerking met IRQ-gebaseerde detectie
- **Calibratie**: Mapping van raw touch coordinates naar screen pixels

### Bestandsstructuur

```
ac_controller_lvgl/
├── ac_controller_lvgl.ino      # Main application file
├── ac_controller_lvgl.h        # Header met declarations
├── credentials.h               # WiFi en MQTT inloggegevens (NIET in git)
├── credentials.h.template      # Template voor credentials
├── ac_units_config.h           # AC unit configuratie en teksten
├── mqtt_config.h               # MQTT topic configuratie en mappings
├── lv_conf.h                   # LVGL configuratie
├── lvgl_screens.cpp            # Main screen en loading screen
├── lvgl_unit_screen.cpp        # Unit detail screen
├── lvgl_master_control.cpp     # Master control functies
├── .gitignore                  # Git ignore file
└── README.md                   # Deze documentatie
```

### Modulaire Architectuur

Het systeem is opgedeeld in modulaire configuratie bestanden:

**`ac_units_config.h`**: 
- AC unit definities en namen
- Nederlandse UI teksten voor modes, fan speeds, swing modes
- Engelse MQTT communicatie teksten

**`mqtt_config.h`**:
- MQTT topic structuren en constanten
- Helper functies voor topic generatie
- Conversie functies tussen UI indices en MQTT strings
- Centralized MQTT command/status mappings

**`credentials.h`** (niet in git):
- WiFi SSID en wachtwoord
- MQTT broker configuratie
- Production mode schakelaar

### AC Unit Data Structuur

```cpp
struct ACUnit {
  const char* name;        // Nederlandse display naam
  const char* mqttTopic;   // MQTT topic identifier
  float currentTemp;       // Huidige temperatuur
  bool isOn;              // Power status
  uint8_t mode;           // Operating mode (0-4)
  uint8_t fanSpeed;       // Fan speed (0-3)
  uint8_t swingMode;      // Swing mode (0-4)
  float targetTemp;       // Doel temperatuur
  float setTemp;          // User ingestelde temperatuur
};
```

### Configured AC Units

Het systeem is geconfigureerd voor de volgende 11 AC units:

1. **Centrale ruimte** (`ac_grote_ruimte_1`)
2. **Bestuurskamer** (`ac_bestuurskamer`)
3. **Vergaderzaal boven** (`ac_vergaderzaal_boven`)
4. **EHBO** (`ac_ehbo`)
5. **Kleedkamer dames** (`ac_kleedkamer_dames`)
6. **Kleedkamer dames gasten BSO** (`ac_kleedkamer_dames_gasten_bso`)
7. **Kleedkamer heren** (`ac_kleedkamer_heren`)
8. **Kleedkamer heren gasten** (`ac_kleedkamer_heren_gasten`)
9. **Scheidsrechters** (`ac_scheidsrechters`)
10. **Materiaalhok binnen** (`ac_materiaalhok_binnen`)
11. **Materiaalhok buiten** (`ac_materiaalhok_buiten`)

Alle units worden weergegeven met Nederlandse namen in de interface, terwijl de MQTT communicatie gebruikmaakt van de gespecificeerde topic identifiers.

## Operating Modes

### HVAC Modi (Nederlandse interface / MQTT communicatie)
- **0**: Koelen / `cool`
- **1**: Verwarmen / `heat`
- **2**: Ventilatie / `fan_only`
- **3**: Auto / `auto`
- **4**: Drogen / `dry`

### Fan Snelheden (Nederlandse interface / MQTT communicatie)
- **0**: Laag / `low`
- **1**: Gemiddeld / `medium`
- **2**: Hoog / `high`
- **3**: Krachtig / `powerful`

### Swing Modi (Nederlandse interface / MQTT communicatie)
- **0**: Swing / `swing`
- **1**: Positie 1 / `position_1`
- **2**: Positie 2 / `position_2`
- **3**: Positie 3 / `position_3`
- **4**: Positie 4 / `position_4`

## MQTT Communicatie Protocol

### Command Topics
```
hcy/airco/{unit_name}/command/power      # "on" / "off"
hcy/airco/{unit_name}/command/mode       # "cool", "heat", "fan_only", "auto", "dry"
hcy/airco/{unit_name}/command/fan_mode   # "low", "medium", "high", "powerful"
hcy/airco/{unit_name}/command/swing_mode # "swing", "position_1", ..., "position_4"
hcy/airco/{unit_name}/command/temperature # "16.0" - "30.0"
```

### Status Topics
```
hcy/airco/{unit_name}/status
```

### Status JSON Format
```json
{
  "hvac_mode": "fan_only",
  "power": "ON",
  "setpoint": "21.0",
  "current_temperature": "22.0",
  "fan_mode": "medium",
  "swing_mode": "position_3"
}
```

## User Interface Componenten

### 1. Main Screen (`lvgl_screens.cpp`)
- **Header**: "AC Bediening HCY" met status icons
- **Unit Cards**: Overzicht van 4 units per pagina
- **Paginatie**: Navigatie tussen unit pagina's
- **Status Icons**: WiFi, MQTT, en test mode indicatoren

### 2. Unit Detail Screen (`lvgl_unit_screen.cpp`)
- **Temperature Display**: Huidige en doel temperatuur
- **Mode Selection**: Modal dialog voor operating mode
- **Fan Speed Control**: Modal dialog voor fan snelheid
- **Swing Control**: Modal dialog voor lamelle positie
- **Power Button**: Aan/uit schakelaar (grijs indien uit, rood indien aan)

### 3. Loading Screen (`lvgl_screens.cpp`)
- **Spinner**: Visuele feedback tijdens opstarten
- **Connection Status**: "Connecting..." bericht

## Touch Event Handling

### Event Flow
1. **Hardware IRQ**: XPT2046_IRQ pin detecteert touch
2. **Coordinate Reading**: Raw touch coordinates uitlezen
3. **Coordinate Mapping**: Raw naar screen pixels transformatie
4. **LVGL Processing**: Event doorgeven aan LVGL input system
5. **UI Response**: Button presses, modal dialogs, etc.

### Touch Optimalisaties
- **IRQ-based Detection**: Vermindert CPU usage
- **Debouncing**: Voorkomt multiple events per touch
- **Coordinate Constraints**: Beperkt touch area tot screen grenzen

## Power Management Logic

### Power State Transitions
1. **Unit Off → Unit On**: 
   - Selecteer operating mode → unit gaat automatisch aan
   - Power button wordt rood en clickable
   
2. **Unit On → Unit Off**:
   - Click power button → unit gaat uit
   - Power button wordt grijs en non-clickable
   - Alle controles blijven zichtbaar maar mode button toont "OFF"

### UI State Management
```cpp
// Power button styling
if (unit->isOn) {
    // Red button, clickable
    lv_obj_set_style_bg_color(powerButton, lv_color_hex(0xFF0000));
    lv_obj_clear_state(powerButton, LV_STATE_DISABLED);
} else {
    // Grey button, disabled
    lv_obj_set_style_bg_color(powerButton, lv_color_hex(0x666666));
    lv_obj_add_state(powerButton, LV_STATE_DISABLED);
}
```

## Network Configuration

### WiFi Settings
```cpp
const char* ssid = "pruimenlimonade";
const char* password = "dikkerd1";
```

### MQTT Broker Settings
```cpp
const char* mqttBroker = "77.172.19.225";
const int mqttPort = 9999;
const char* mqttUser = "mqttuser";
const char* mqttPassword = "xjf4kqx7quv.PQM6bkv";
```

## Test Mode

### Activatie
```cpp
bool testMode = false; // Set to true for test mode
```

### Test Mode Features
- **Geen MQTT**: Lokale state simulation
- **Dummy Data**: Realistische temperatuur fluctuaties
- **UI Testing**: Volledige interface functionaliteit
- **Debug Output**: Uitgebreide serial logging

## Error Handling & Debugging

### MQTT Connection Issues
- **Auto-Reconnect**: Automatische herverbinding bij connectie verlies
- **Status Logging**: Connection status updates elke 10 seconden
- **Fallback**: Test mode als backup

### Touch Issues
- **IRQ Monitoring**: Real-time touch event debugging
- **Coordinate Validation**: Touch binnen screen grenzen
- **State Prevention**: Voorkomt recursive touch processing

### Case Sensitivity Fix
Het systeem handelt both uppercase en lowercase MQTT responses af:
```cpp
// Handle both "ON"/"on" and "OFF"/"off"
unit->isOn = (strcasecmp(power, "on") == 0);
```

## Memory Management

### LVGL Buffers
```cpp
static lv_color_t buf1[TFT_WIDTH * 3]; // Primary buffer
static lv_color_t buf2[TFT_WIDTH * 3]; // Secondary buffer
```

### Stack Optimization
- **Static Variables**: Vermijdt heap fragmentatie
- **Buffer Sizing**: Geoptimaliseerd voor 240x320 display
- **Timer Management**: Efficiënte update intervals

## Update Intervals

### Timer-based Updates
- **Data Timer**: 2000ms voor MQTT data synchronisatie
- **LVGL Handler**: 5ms voor smooth UI rendering
- **Touch Check**: 10ms voor responsive touch

### Connection Monitoring
- **MQTT Status**: 10 seconden interval
- **Temperature Updates**: 30 seconden in test mode

## Security Features

### Credential Management
Voor beveiliging zijn alle gevoelige gegevens verplaatst naar een apart bestand:

- **`credentials.h`**: Bevat WiFi en MQTT inloggegevens
- **`.gitignore`**: Voorkomt dat credentials worden gecommit naar version control
- **`credentials.h.template`**: Template bestand met placeholder waarden

### Production Mode
Het systeem heeft een production mode functie die alle debug output uitschakelt:

```cpp
// In credentials.h
const bool PRODUCTION_MODE = true; // Set to true for production
```

Wanneer `PRODUCTION_MODE = true`:
- Alle `DEBUG_PRINT()`, `DEBUG_PRINTLN()`, `DEBUG_PRINTF()` statements worden uitgeschakeld
- Gevoelige informatie zoals MQTT payloads worden niet naar serial output gestuurd
- Vermindert geheugengebruik en verbetert prestaties
- Voorkomt informatielekken via serial console

### Setup Instructions voor Credentials
1. Kopieer `credentials.h.template` naar `credentials.h`
2. Vul je eigen WiFi en MQTT gegevens in
3. Stel `PRODUCTION_MODE` in op `true` voor productie deployments
4. Het `credentials.h` bestand wordt automatisch genegeerd door git

## Installatiehandleiding

### Benodigde Libraries
```cpp
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <ArduinoJson.h>
#include <lvgl.h>
```

### Compilatie Settings
- **Board**: ESP32 Dev Module
- **Partition Scheme**: Default 4MB
- **Core Debug Level**: None (voor production)

### Upload Procedure
1. Selecteer correct COM port
2. Compileer en upload firmware
3. Monitor serial output voor debug info
4. Test touch calibratie
5. Verificeer MQTT connectie

## Troubleshooting

### Veelvoorkomende Problemen

**Touch werkt niet**:
- Controleer XPT2046 pin connecties
- Verificeer SPI configuratie
- Check touch calibratie mapping

**MQTT verbinding mislukt**:
- Controleer WiFi credentials
- Verificeer MQTT broker settings
- Test netwerkconnectiviteit

**UI reageert traag**:
- Verhoog LVGL timer frequency
- Controleer memory usage
- Optimaliseer buffer sizes

**Power state incorrect**:
- Verificeer MQTT case sensitivity handling
- Check status topic responses
- Monitor debug output voor state changes (zet `PRODUCTION_MODE = false` voor debugging)

**Geen debug output zichtbaar**:
- Controleer of `PRODUCTION_MODE = false` in `credentials.h`
- Verificeer serial monitor baud rate (115200)
- Check serial monitor configuratie

## Bijdragen

Voor wijzigingen of uitbreidingen:
1. Test in test mode eerst
2. Verificeer MQTT compatibiliteit
3. Update documentatie
4. Test alle UI states

## Licentie

Dit project is ontwikkeld voor HCY airconditioning systeem.