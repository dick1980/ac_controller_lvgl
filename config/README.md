# Configuration Directory

This directory contains all configuration files for the AC Controller system. These files provide a modular way to customize different aspects of the system without modifying the core application code.

## Configuration Files

### Core Configuration
- **`credentials.h`** - WiFi and MQTT credentials (not in git)
- **`credentials.h.template`** - Template for setting up credentials

### System Configuration  
- **`ac_units_config.h`** - AC unit definitions and display names
- **`mqtt_config.h`** - MQTT topic structure and communication mappings
- **`hardware_config.h`** - Hardware pins, timing, and validation settings
- **`ui_config.h`** - UI layout, colors, fonts, and styling
- **`translations.h`** - Text strings and language settings

## Quick Customization Guide

### Adding/Modifying AC Units
Edit `ac_units_config.h`:
```cpp
ACUnit acUnits[] = {
  {
    "Display Name",     // Dutch name shown in UI
    "mqtt_topic_name",  // MQTT topic identifier  
    22.0, false, 0, 0, 0, 22.0, 22.0  // Default values
  },
  // ... more units
};
```

### Changing Colors/Layout
Edit `ui_config.h`:
```cpp
#define UI_COLOR_MODE_COOL 0x2B9AF9  // Blue
#define UI_CARD_HEIGHT 50
#define UI_UNITS_PER_PAGE 4
```

### Modifying MQTT Topics
Edit `mqtt_config.h`:
```cpp
#define MQTT_BASE_TOPIC "hcy/airco"
#define MQTT_COMMAND_POWER "command/power"
```

### Hardware Porting
Edit `hardware_config.h`:
```cpp
#define XPT2046_CS 33
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
```

### Text Changes
Edit `translations.h`:
```cpp
#define TXT_APP_TITLE "AC Bediening HCY"
#define TXT_BACK "Terug"
```

## Security Note

The `credentials.h` file contains sensitive information and is excluded from version control via `.gitignore`. Always use the template file to set up your credentials.