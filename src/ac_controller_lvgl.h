#ifndef AC_CONTROLLER_LVGL_H
#define AC_CONTROLLER_LVGL_H

#include <lvgl.h>


// Always include TFT_eSPI since we're using it in our project
#include <TFT_eSPI.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <XPT2046_Touchscreen.h>
#include <ArduinoJson.h>

// External declarations for global objects
extern TFT_eSPI tft;
extern WiFiClient wifiClient;
extern PubSubClient mqttClient;
extern const char* mqttBroker;
extern const int mqttPort;
extern const char* mqttUser;
extern const char* mqttPassword;

// Test mode flag to skip MQTT connection
extern bool testMode;

// Screen control variables
extern int currentPage;
extern int unitsPerPage;
extern int selectedUnit;

// AC Units configuration
struct ACUnit {
  const char* name;        // Display name
  const char* mqttTopic;   // MQTT topic identifier (e.g., "ac_bestuurskamer")
  float currentTemp;
  bool isOn;
  uint8_t mode;
  uint8_t fanSpeed;
  uint8_t swingMode;       // Changed from bool swingOn to uint8_t swingMode (0-4)
  float targetTemp;        // Changed to float for MQTT
  float setTemp;           // Changed to float for MQTT - Temperature set by user
};

// External declarations for global arrays (defined in ac_units_config.h)
extern ACUnit acUnits[];
extern const int numUnits;
extern const char* modeNames[];
extern const char* modeNamesEN[];
extern const char* fanNames[];
extern const char* fanNamesEN[];
extern const char* swingNames[];
extern const char* swingNamesEN[];

// LVGL screens
extern lv_obj_t *mainScreen;
extern lv_obj_t *unitDetailScreen;
extern lv_obj_t *loadingScreen;
extern lv_obj_t *unitScreen;
extern lv_obj_t *loadingScreen;

// LVGL objects for main screen
extern lv_obj_t *mainTitle;
extern lv_obj_t *connectionStatus; // Hidden but kept for compatibility
extern lv_obj_t *testModeIcon;
extern lv_obj_t *mqttIcon;
extern lv_obj_t *wifiIcon;
extern lv_obj_t *unitCards[4]; // One card per unit on screen
extern lv_obj_t *prevButton;
extern lv_obj_t *nextButton;
extern lv_obj_t *pageIndicator;

// LVGL objects for unit screen
extern lv_obj_t *unitTitle;
extern lv_obj_t *tempDisplay;
extern lv_obj_t *powerButton;
extern lv_obj_t *tempSlider;
extern lv_obj_t *modeTabView;
extern lv_obj_t *fanSpeedButtons[3];
extern lv_obj_t *swingButton;  // Changed from swingSwitch to swingButton
extern lv_obj_t *backButton;

// Modal screens
extern lv_obj_t *modeModal;
extern lv_obj_t *fanModal;
extern lv_obj_t *swingModal;  // Added swing modal

// Function declarations for screens
void createLoadingScreen();
void createMainScreen();
void createUnitScreen();
void createMasterControl();
void createUnitDetailScreen();
void updateMainScreen();
void updateUnitScreen(int unitIndex);
void showUnitDetail(int unitIndex);

// Function declarations for data handling
void updateUnitData(int unitIndex);
void updateAllUnits();
void setACPower(int unitIndex, bool state);
void setACMode(int unitIndex, uint8_t mode);
void setACFanSpeed(int unitIndex, uint8_t speed);
void setACSwing(int unitIndex, uint8_t state);
void setACTemperature(int unitIndex, float temp);
void setAllACPower(bool state);
void turnOffAllACUnits();

// Event callbacks for main screen
void unit_card_event_cb(lv_event_t *e);
void prev_page_event_cb(lv_event_t *e);
void next_page_event_cb(lv_event_t *e);

// Event callbacks for unit screen
void power_button_event_cb(lv_event_t *e);
void temp_minus_btn_event_cb(lv_event_t *e);
void temp_plus_btn_event_cb(lv_event_t *e);
void mode_matrix_event_cb(lv_event_t *e);
void mode_roller_event_cb(lv_event_t *e);
void mode_button_modal_event_cb(lv_event_t *e);
void fan_button_event_cb(lv_event_t *e);
void fan_roller_event_cb(lv_event_t *e);
void swing_button_event_cb(lv_event_t *e); // Changed from swing_switch_event_cb to swing_button_event_cb
void swing_roller_event_cb(lv_event_t *e); // Added swing roller event callback
void back_button_event_cb(lv_event_t *e);

// Event callbacks for unit detail screen
void back_event_cb(lv_event_t *e);
void temp_minus_event_cb(lv_event_t *e);
void temp_plus_event_cb(lv_event_t *e);
void mode_button_event_cb(lv_event_t *e);
void fan_speed_event_cb(lv_event_t *e);
void power_switch_event_cb(lv_event_t *e);
void save_settings_event_cb(lv_event_t *e);

// Event callbacks for master control
void all_on_event_cb(lv_event_t *e);
void all_on_msgbox_event_cb(lv_event_t *e);
void all_off_event_cb(lv_event_t *e);
void all_off_msgbox_event_cb(lv_event_t *e);
void notification_timer_cb(lv_timer_t *timer);

#endif // AC_CONTROLLER_LVGL_H
