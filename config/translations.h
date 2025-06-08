#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

// Translation Configuration
// Currently supports Dutch (NL) as primary language
// English (EN) used for MQTT communication only

// Language selection (future expansion)
typedef enum {
    LANG_NL = 0,  // Dutch (primary)
    LANG_EN = 1   // English (MQTT only)
} language_t;

// Current language setting
#define CURRENT_LANGUAGE LANG_NL

// Main Screen Text
#define TXT_APP_TITLE "AC Bediening HCY"
#define TXT_PAGE_INDICATOR "Pagina"
#define TXT_CONNECTING "Connecting..."
#define TXT_WIFI_FAILED "WiFi connection failed"
#define TXT_TEST_MODE_STATUS "TEST MODE - No MQTT Connection"

// Navigation Text
#define TXT_BACK "Terug"
#define TXT_POWER "POWER"
#define TXT_ALL_ON "ALL ON"
#define TXT_ALL_OFF "ALL OFF"
#define TXT_CANCEL "Cancel"
#define TXT_CONFIRM "Confirm"
#define TXT_CLOSE "X"

// Unit Screen Text
#define TXT_TEMPERATURE_SETTING "Temperatuur inst."
#define TXT_MODE "Modus"
#define TXT_MODE_LABEL "Mode:"
#define TXT_FAN_SPEED "Fan snelh."
#define TXT_SWING_MODE "Lamelle"
#define TXT_OFF "OFF"

// Modal Headers
#define TXT_CHOOSE_MODE "Kies Modus"
#define TXT_CHOOSE_FAN_SPEED "Kies Fan Snelheid"
#define TXT_CHOOSE_SWING_MODE "Kies Lamelle Modus"

// Confirmation Messages
#define TXT_CONFIRM_ALL_ON "Turn ON all AC units?"
#define TXT_CONFIRM_ALL_OFF "Turn OFF all AC units?"
#define TXT_CONFIRMATION "Confirmation"

// Notification Messages
#define TXT_ALL_UNITS_ON "All units turned ON"
#define TXT_ALL_UNITS_OFF "All units turned OFF"

// Status Messages  
#define TXT_MQTT_CONNECTION_OK "MQTT connection: OK"
#define TXT_UNIT_DATA_UPDATED "Unit data updated via callbacks"

// Debug Messages (Dutch for consistency)
#define TXT_DEBUG_AC_STARTING "AC Controller with LVGL starting..."
#define TXT_DEBUG_TOUCH_INIT_FAILED "ERROR: Touch controller initialization failed!"
#define TXT_DEBUG_TOUCH_INIT_SUCCESS "Touch controller initialized successfully"
#define TXT_DEBUG_LVGL_INIT "Initializing LVGL..."
#define TXT_DEBUG_LVGL_SUCCESS "LVGL initialized successfully"
#define TXT_DEBUG_MQTT_CALLBACK_START "=== MQTT CALLBACK START ==="
#define TXT_DEBUG_MQTT_CALLBACK_END "=== MQTT CALLBACK END ==="
#define TXT_DEBUG_MODE_SELECTION_START "=== MODE SELECTION START ==="
#define TXT_DEBUG_MODE_SELECTION_END "=== MODE SELECTION END ==="

// Error Messages
#define TXT_ERROR_JSON_PARSING_FAILED "JSON parsing failed: "
#define TXT_ERROR_TOPIC_NO_MATCH "Topic did not match any unit"
#define TXT_ERROR_UNIT_INDEX_INVALID "Invalid unit index"
#define TXT_ERROR_MQTT_NOT_CONNECTED "WARNING: MQTT not connected"

// Temperature Units
#define TXT_CELSIUS_SYMBOL "°C"
#define TXT_TEMPERATURE_FORMAT "%.1f°C"
#define TXT_TEMPERATURE_SET_FORMAT "%.0f°C"

// Status Indicators
#define TXT_STATUS_ON "on"
#define TXT_STATUS_OFF "off"
#define TXT_STATUS_CONNECTED "connected"
#define TXT_STATUS_DISCONNECTED "disconnected"

// Mode Names (UI Display - Dutch)
extern const char* modeNames[];
// Mode Names (MQTT Communication - English)  
extern const char* modeNamesEN[];

// Fan Speed Names (UI Display - Dutch)
extern const char* fanNames[];
// Fan Speed Names (MQTT Communication - English)
extern const char* fanNamesEN[];

// Swing Mode Names (UI Display - Dutch)
extern const char* swingNames[];
// Swing Mode Names (MQTT Communication - English)
extern const char* swingNamesEN[];

// Helper function to get translated text (for future multi-language support)
inline const char* getText(const char* key) {
    // Currently returns the key directly since we only have Dutch
    // Future: implement lookup table for multiple languages
    return key;
}

// Helper functions for mode names
inline const char* getModeDisplayName(uint8_t mode) {
    if (mode <= 4) return modeNames[mode];
    return modeNames[0]; // Default to first mode
}

inline const char* getFanDisplayName(uint8_t fanSpeed) {
    if (fanSpeed <= 3) return fanNames[fanSpeed];
    return fanNames[0]; // Default to first speed
}

inline const char* getSwingDisplayName(uint8_t swingMode) {
    if (swingMode <= 4) return swingNames[swingMode];
    return swingNames[0]; // Default to first mode
}

// Validation helpers
#define IS_VALID_MODE(mode) ((mode) <= 4)
#define IS_VALID_FAN_SPEED(speed) ((speed) <= 3)
#define IS_VALID_SWING_MODE(swing) ((swing) <= 4)

// Text formatting helpers
#define FORMAT_PAGE_INDICATOR(buffer, current, total) \
    sprintf(buffer, "%s %d/%d", TXT_PAGE_INDICATOR, current, total)

#define FORMAT_TEMPERATURE(buffer, temp) \
    sprintf(buffer, TXT_TEMPERATURE_FORMAT, temp)

#define FORMAT_TEMPERATURE_SET(buffer, temp) \
    sprintf(buffer, TXT_TEMPERATURE_SET_FORMAT, temp)

// Accessibility text (for future screen reader support)
#define TXT_ACC_UNIT_CARD "AC unit control card"
#define TXT_ACC_POWER_BUTTON "Power toggle button"
#define TXT_ACC_MODE_BUTTON "Operating mode selection"
#define TXT_ACC_FAN_BUTTON "Fan speed selection"
#define TXT_ACC_SWING_BUTTON "Air swing control"
#define TXT_ACC_TEMP_INCREASE "Increase temperature"
#define TXT_ACC_TEMP_DECREASE "Decrease temperature"

#endif // TRANSLATIONS_H