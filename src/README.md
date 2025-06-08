# Source Directory

This directory contains the core source files and headers for the AC Controller system.

## Core Files

### Headers
- **`ac_controller_lvgl.h`** - Main header with structure definitions, function declarations, and external references
- **`lv_conf.h`** - LVGL library configuration for ESP32-2432S028

## Main Header (`ac_controller_lvgl.h`)

### Key Structures
```cpp
struct ACUnit {
  const char* name;        // Display name (Dutch)
  const char* mqttTopic;   // MQTT topic identifier
  float currentTemp;       // Current temperature
  bool isOn;              // Power status
  uint8_t mode;           // Operating mode (0-4)
  uint8_t fanSpeed;       // Fan speed (0-3)
  uint8_t swingMode;      // Swing mode (0-4)
  float targetTemp;       // Target temperature
  float setTemp;          // User set temperature
};
```

### Function Categories

**Screen Management:**
- `createLoadingScreen()`, `createMainScreen()`, `createUnitScreen()`
- `updateMainScreen()`, `updateUnitScreen()`

**Data Handling:**
- `updateUnitData()`, `updateAllUnits()`
- `setACPower()`, `setACMode()`, `setACFanSpeed()`, `setACSwing()`, `setACTemperature()`

**Event Callbacks:**
- Main screen: `unit_card_event_cb()`, `prev_page_event_cb()`, `next_page_event_cb()`
- Unit screen: `power_button_event_cb()`, `mode_button_modal_event_cb()`, etc.
- Master control: `all_on_event_cb()`, `all_off_event_cb()`

### External Dependencies

The header references configuration files for modular system design:
- AC unit definitions from `../config/ac_units_config.h`
- MQTT settings from `../config/mqtt_config.h`
- Hardware settings from `../config/hardware_config.h`
- UI styling from `../config/ui_config.h`
- Text strings from `../config/translations.h`

## LVGL Configuration (`lv_conf.h`)

Configures LVGL v9.x for the ESP32-2432S028:
- Memory allocation and buffer management
- Display driver settings
- Input device configuration
- Feature enablement (widgets, animations, etc.)
- Performance optimizations for embedded systems

## Integration

These source files are included by:
- Main application (`../ac_controller_lvgl.ino`)
- UI components (`../ui/*.cpp`)
- Configuration files (`../config/*.h`)

The modular design allows the core functionality to remain stable while configuration can be easily modified.