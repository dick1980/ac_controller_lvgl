/*
 * AC Controller for ESP32-2432S028
 * Controls multiple AC units via MQTT
 * Using LVGL for modern UI
 */

#include <SPI.h>
#include <PubSubClient.h>
#include "src/ac_controller_lvgl.h"
#include "config/credentials.h"
#include "config/ac_units_config.h"
#include "config/mqtt_config.h"
#include "config/hardware_config.h"
#include "config/ui_config.h"
#include "config/translations.h"

// Network and MQTT configuration (loaded from credentials.h)
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqttBroker = MQTT_BROKER;
const int mqttPort = MQTT_PORT;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;

// Production mode flag
bool productionMode = PRODUCTION_MODE;

// Debug printing macros - only print if not in production mode
#define DEBUG_PRINT(x) do { if (!productionMode) Serial.print(x); } while(0)
#define DEBUG_PRINTLN(x) do { if (!productionMode) Serial.println(x); } while(0)
#define DEBUG_PRINTF(x, ...) do { if (!productionMode) Serial.printf(x, __VA_ARGS__); } while(0)

// Test mode flag - set to true to skip MQTT connection and use dummy data
bool testMode = false; // Change to false for normal operation

// Create separate SPI instance for touch controller
SPIClass touchSPI = SPIClass(TOUCH_SPI_INSTANCE);

// Create touchscreen instance with IRQ pin
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

// Create display and MQTT instances
TFT_eSPI tft = TFT_eSPI();
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// LVGL display buffer - configured from hardware config
static lv_color_t buf1[LVGL_BUFFER_SIZE];
static lv_color_t buf2[LVGL_BUFFER_SIZE];

// LVGL display driver
static lv_display_t *display;

// LVGL input device driver
static lv_indev_t *indev;

// Screen control variables
int currentPage = 0;
int unitsPerPage = UI_UNITS_PER_PAGE;
int selectedUnit = -1;

// AC units and related arrays are now loaded from ac_units_config.h

// LVGL screens
lv_obj_t *mainScreen = NULL;
lv_obj_t *unitDetailScreen = NULL;
lv_obj_t *loadingScreen = NULL;
lv_obj_t *unitScreen = NULL;

// LVGL objects for main screen
lv_obj_t *mainTitle = NULL;
lv_obj_t *connectionStatus = NULL;
// Status icons for the header
lv_obj_t *testModeIcon = NULL;
lv_obj_t *mqttIcon = NULL;
lv_obj_t *wifiIcon = NULL;
lv_obj_t *unitCards[4]; // One card per unit on screen
lv_obj_t *prevButton = NULL;
lv_obj_t *nextButton = NULL;
lv_obj_t *pageIndicator = NULL;

// LVGL objects for unit screen
lv_obj_t *unitTitle;
lv_obj_t *tempDisplay;
lv_obj_t *powerButton;
lv_obj_t *tempSlider;
lv_obj_t *modeTabView;
lv_obj_t *fanSpeedButtons[3];
lv_obj_t *swingButton;  // Changed from swingSwitch to swingButton
lv_obj_t *backButton;

// Modal screens
lv_obj_t *modeModal = NULL;
lv_obj_t *fanModal = NULL;
lv_obj_t *swingModal = NULL;

// Function declarations are now in ac_controller_lvgl.h

// LVGL display flush callback
static void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)px_map, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(disp);
}

// Global variables to store touch state
static bool g_is_touched = false;
static uint16_t g_touch_x = 0;
static uint16_t g_touch_y = 0;
static bool g_processing_touch = false; // Flag to prevent recursive touch processing

// Direct touchpad read callback for LVGL v9.x
static void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  static uint16_t last_x = 0;
  static uint16_t last_y = 0;
  static bool was_touched = false;
  
  // Use global touch state first (set in the main loop)
  if (g_is_touched) {
    // Map raw touch coordinates to screen coordinates
    uint16_t x = map(g_touch_x, TOUCH_RAW_MIN, TOUCH_RAW_MAX, 0, TFT_WIDTH);
    uint16_t y = map(g_touch_y, TOUCH_RAW_MIN, TOUCH_RAW_MAX, 0, TFT_HEIGHT);
    
    // Ensure coordinates are within screen boundaries
    x = constrain(x, 0, TFT_WIDTH-1);
    y = constrain(y, 0, TFT_HEIGHT-1);
    
    // Set the touch state for LVGL
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
    
    // Store last position
    last_x = x;
    last_y = y;
    was_touched = true;
    
    // Debug output removed
    return;
  }
  
  // If we get here, check the touchscreen directly as a backup
  bool irq_active = (digitalRead(XPT2046_IRQ) == LOW);
  
  if (irq_active && ts.touched()) {
    TS_Point p = ts.getPoint();
    
    // Map raw touch coordinates to screen coordinates
    uint16_t x = map(p.x, TOUCH_RAW_MIN, TOUCH_RAW_MAX, 0, TFT_WIDTH);
    uint16_t y = map(p.y, TOUCH_RAW_MIN, TOUCH_RAW_MAX, 0, TFT_HEIGHT);
    
    // Ensure coordinates are within screen boundaries
    x = constrain(x, 0, TFT_WIDTH-1);
    y = constrain(y, 0, TFT_HEIGHT-1);
    
    // Set the touch state for LVGL
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
    
    // Store last position
    last_x = x;
    last_y = y;
    was_touched = true;
    
    // Debug output removed
    
    // REMOVED recursive calls to prevent stack overflow
  }
  
  // No touch detected, set to released state
  data->state = LV_INDEV_STATE_RELEASED;
  
  // Use last position for release state
  if (was_touched) {
    data->point.x = last_x;
    data->point.y = last_y;
    was_touched = false;
    // Touch release debug output removed
  }
}

// LVGL timer callback for updating data
static void update_data_timer(lv_timer_t *timer) {
  // Update all unit data from MQTT
  updateAllUnits();
  
  // Update current screen based on which one is active
  lv_obj_t *current_screen = lv_scr_act();
  
  if (current_screen == mainScreen) {
    updateMainScreen();
  } else if (current_screen == unitScreen && selectedUnit >= 0) {
    updateUnitScreen(selectedUnit);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000); // Allow serial to initialize
  DEBUG_PRINTLN(TXT_DEBUG_AC_STARTING);
  
  // Set backlight pin as output and turn it on
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Initialize display
  tft.init();
  tft.setRotation(TFT_ROTATION); // Portrait mode from config
  
  // Initialize touchscreen with custom SPI - exactly as in the working touch test
  touchSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  if (!ts.begin(touchSPI)) {
    DEBUG_PRINTLN(TXT_DEBUG_TOUCH_INIT_FAILED);
  } else {
    DEBUG_PRINTLN(TXT_DEBUG_TOUCH_INIT_SUCCESS);
  }
  ts.setRotation(TFT_ROTATION); // Match display rotation
  
  // Initialize LVGL
  Serial.println("Initializing LVGL...");
  lv_init();
  Serial.println("LVGL initialized successfully");
  
  // Initialize display for portrait mode
  Serial.println("Creating display...");
  display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
  Serial.println("Setting display flush callback...");
  lv_display_set_flush_cb(display, my_disp_flush);
  Serial.println("Setting display buffers...");
  lv_display_set_buffers(display, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
  
  // Make sure we're in portrait mode
  Serial.println("Setting display rotation...");
  lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);
  
  // Add debug message for display initialization
  Serial.print("Display initialized: ");
  Serial.print(TFT_WIDTH);
  Serial.print("x");
  Serial.println(TFT_HEIGHT);
  
  // Initialize input device with enhanced configuration for LVGL v9.x
  Serial.println("Creating input device...");
  indev = lv_indev_create();
  Serial.println("Setting input device type...");
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  Serial.println("Setting input device read callback...");
  lv_indev_set_read_cb(indev, my_touchpad_read);
  Serial.println("Setting input device display...");
  lv_indev_set_display(indev, display);
  
  // Set long press time to make touch more responsive
  lv_indev_set_long_press_time(indev, INDEV_LONG_PRESS_TIME);
  lv_indev_set_scroll_limit(indev, INDEV_SCROLL_LIMIT);
  
  // Force the input device to be active
  Serial.println("Activating touch input device...");
  lv_indev_enable(indev, true);
  
  // Register the input device with LVGL
  Serial.println("Registering input device with LVGL...");
  lv_indev_set_group(indev, NULL); // No input group needed for touchscreen
  
  Serial.println("Touch input device fully configured and activated");
  
  // Test touch input device
  Serial.println("Testing touch input device...");
  if (lv_indev_get_type(indev) == LV_INDEV_TYPE_POINTER) {
    Serial.println("Touch input device type verified as POINTER");
  } else {
    Serial.println("ERROR: Touch input device type verification failed!");
  }
  
  // Create LVGL screens
  Serial.println("Creating loading screen...");
  createLoadingScreen();
  Serial.println("Creating main screen...");
  createMainScreen();
  Serial.println("Creating unit screen...");
  createUnitScreen();
//  Serial.println("Creating unit detail screen...");
//  createUnitDetailScreen();
  Serial.println("All screens created successfully");
  
  // Show loading screen initially
  Serial.println("Loading initial screen...");
  lv_scr_load(loadingScreen);
  Serial.println("Initial screen loaded");
  
  if (testMode) {
    // Skip WiFi and MQTT connection in test mode
    Serial.println("Test mode enabled - skipping WiFi and MQTT connection");
    
    // Initialize AC units with dummy values for testing
    for (int i = 0; i < numUnits; i++) {
      // Set dummy values for testing
      acUnits[i].currentTemp = 22.5 + (i * 0.5);
      acUnits[i].isOn = (i % 2 == 0); // Alternate on/off
      acUnits[i].mode = i % 5; // Cycle through modes
      acUnits[i].fanSpeed = i % 4; // Cycle through fan speeds
      acUnits[i].swingMode = i % 5; // Some with swing on
      acUnits[i].targetTemp = 23 + (i % 5); // Different target temps
    }
    
    // Create timer for periodic UI updates even in test mode
    lv_timer_create(update_data_timer, DATA_UPDATE_INTERVAL, NULL);
    
    // Show main screen immediately
    lv_scr_load(mainScreen);
    
    // Explicitly update the main screen to show the AC units
    updateMainScreen();
    
    // Update connection status to show test mode
    lv_label_set_text(connectionStatus, "TEST MODE - No MQTT Connection");
    lv_obj_set_style_text_color(connectionStatus, lv_color_hex(0xFFAA00), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    // Normal operation mode - connect to WiFi and MQTT
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    
    // Wait for connection with timeout
    int wifiTimeout = 0;
    while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20) {
      delay(500);
      Serial.print(".");
      wifiTimeout++;
      lv_timer_handler(); // Keep LVGL running during connection
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
      Serial.println("IP address: " + WiFi.localIP().toString());
      
      // Connect to MQTT broker
      Serial.println("Connecting to MQTT broker...");
      
      Serial.print("Attempting to connect to ");
      Serial.print(mqttBroker);
      Serial.print(":");
      Serial.println(mqttPort);
      
      mqttClient.setServer(mqttBroker, mqttPort);
      mqttClient.setCallback(mqttCallback);
      
      while (!mqttClient.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
          Serial.println("MQTT connection successful!");
          
          // Subscribe to all AC unit status topics
          for (int i = 0; i < numUnits; i++) {
            char statusTopic[MQTT_TOPIC_MAX_LENGTH];
            generateStatusTopic(statusTopic, acUnits[i].mqttTopic);
            mqttClient.subscribe(statusTopic);
            DEBUG_PRINT("Subscribed to: ");
            DEBUG_PRINTLN(statusTopic);
          }
          
          // Wait for initial status updates to arrive naturally from Home Assistant
          // Removed: force status polling as it can cause race conditions
          delay(1000); // Wait a bit for subscriptions to be processed
        } else {
          Serial.print("MQTT connection failed, rc=");
          Serial.print(mqttClient.state());
          Serial.println(" try again in 5 seconds");
          delay(5000);
        }
      }
      
      // Initialize AC units with default values
      for (int i = 0; i < numUnits; i++) {
        updateUnitData(i);
      }
      
      // Create timer for periodic updates
      lv_timer_create(update_data_timer, DATA_UPDATE_INTERVAL, NULL);
      
      // Show main screen
      lv_scr_load(mainScreen);
    } else {
      Serial.println("WiFi connection failed");
      // Update loading screen to show error
      lv_obj_t *errorLabel = lv_label_create(loadingScreen);
      lv_label_set_text(errorLabel, "WiFi connection failed");
      lv_obj_align(errorLabel, LV_ALIGN_CENTER, 0, 40);
    }
  }
}

void loop() {
  static uint32_t frame_count = 0;
  static uint32_t last_fps_update = 0;
  static uint32_t last_touch_check = 0;
  static uint32_t last_lvgl_update = 0;
  static uint32_t last_tick_update = 0;
  static uint32_t last_connection_check = 0;
  uint32_t now = millis();
  
  // Update LVGL tick counter - critical for animations and timers
  uint32_t tick_elapsed = now - last_tick_update;
  if (tick_elapsed > 0) {
    lv_tick_inc(tick_elapsed);
    last_tick_update = now;
  }
  
  // Handle MQTT communication in test mode
  if (!testMode) {
    if (!mqttClient.connected()) {
      reconnect();
    }
    mqttClient.loop();
    
    // Check MQTT connection status periodically
    if (now - last_connection_check > CONNECTION_CHECK_INTERVAL) {
      if (!mqttClient.connected()) {
        Serial.println("WARNING: MQTT connection lost! Attempting to reconnect...");
        reconnect();
      }
      last_connection_check = now;
    }
  }
  
  // Check for touch events directly (for debugging)
  if (now - last_touch_check > TOUCH_CHECK_INTERVAL && !g_processing_touch) { // Check touch, avoid recursive processing
    // Set processing flag to prevent recursive calls
    g_processing_touch = true;
    
    // Check IRQ pin directly
    bool irq_active = (digitalRead(XPT2046_IRQ) == LOW);
    
    if (irq_active && ts.touched()) {
      TS_Point p = ts.getPoint();
      
      // Store global touch state for LVGL callback
      g_is_touched = true;
      g_touch_x = p.x;
      g_touch_y = p.y;
      
      // Map raw touch coordinates to screen coordinates
      uint16_t x = map(p.x, 300, 3800, 0, TFT_WIDTH);
      uint16_t y = map(p.y, 300, 3800, 0, TFT_HEIGHT);
      
      // Direct touch debug output removed
      
      // Let LVGL process this touch in the next timer handler call
      // No direct calls to lv_indev_read here to prevent recursion
    } else {
      // Reset touch state when released
      if (g_is_touched) {
        g_is_touched = false;
        // Touch release debug output removed
        
        // Let LVGL process the release in the next timer handler call
        // No direct calls to lv_indev_read here to prevent recursion
      }
    }
    
    last_touch_check = now;
    g_processing_touch = false; // Clear processing flag
  }
  
  // Handle LVGL tasks more frequently
  if (now - last_lvgl_update > LVGL_TIMER_INTERVAL) { // Process LVGL tasks for smoother UI
    lv_timer_handler(); // This will process any pending touch events via the callback
    last_lvgl_update = now;
  }
  
  // Use a smaller delay to keep the UI responsive while preventing watchdog reset
  delay(1);
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Subscribe to all AC unit status topics
      for (int i = 0; i < numUnits; i++) {
        char statusTopic[MQTT_TOPIC_MAX_LENGTH];
        generateStatusTopic(statusTopic, acUnits[i].mqttTopic);
        mqttClient.subscribe(statusTopic);
        DEBUG_PRINT("Subscribed to: ");
        DEBUG_PRINTLN(statusTopic);
      }
      
      // Wait for natural status updates from Home Assistant
      // Removed: force status polling as it can cause race conditions
      delay(1000); // Wait a bit for subscriptions to be processed
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  DEBUG_PRINTLN("=== MQTT CALLBACK START ===");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINTLN(topic);
  
  // Convert payload to string
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  
  DEBUG_PRINT("Payload: ");
  DEBUG_PRINTLN(message);
  
  // Parse JSON status message
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);
  
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Find which unit this status update is for
  bool topicMatched = false;
  for (int i = 0; i < numUnits; i++) {
    char statusTopic[MQTT_TOPIC_MAX_LENGTH];
    generateStatusTopic(statusTopic, acUnits[i].mqttTopic);
    
    if (strcmp(topic, statusTopic) == 0) {
      topicMatched = true;
      Serial.print("Topic matched for unit ");
      Serial.print(i);
      Serial.print(" (");
      Serial.print(acUnits[i].name);
      Serial.println(")");
      
      // Update unit data from JSON
      ACUnit *unit = &acUnits[i];
      
      Serial.print("BEFORE UPDATE - Unit ");
      Serial.print(unit->name);
      Serial.print(": Power=");
      Serial.print(unit->isOn ? "on" : "off");
      Serial.print(", Mode=");
      Serial.println(unit->mode);
      
      if (doc.containsKey("current_temperature")) {
        unit->currentTemp = doc["current_temperature"];
        Serial.print("  - Current temp: ");
        Serial.println(unit->currentTemp);
      }
      
      if (doc.containsKey("power")) {
        const char* power = doc["power"];
        bool oldState = unit->isOn;
        // Handle both "on"/"off" and "ON"/"OFF" cases
        unit->isOn = (strcasecmp(power, "on") == 0);
        Serial.print("  - Power: ");
        Serial.print(power);
        Serial.print(" (was: ");
        Serial.print(oldState ? "on" : "off");
        Serial.print(", now: ");
        Serial.print(unit->isOn ? "on" : "off");
        Serial.println(")");
      }
      
      if (doc.containsKey("hvac_mode")) {
        const char* mode = doc["hvac_mode"];
        DEBUG_PRINT("  - HVAC mode: ");
        DEBUG_PRINT(mode);
        // Convert HVAC mode string to index using helper function
        unit->mode = getModeIndexFromMQTT(mode);
        DEBUG_PRINT(" -> index ");
        DEBUG_PRINTLN(unit->mode);
      }
      
      if (doc.containsKey("fan_mode")) {
        const char* fan = doc["fan_mode"];
        DEBUG_PRINT("  - Fan mode: ");
        DEBUG_PRINT(fan);
        // Convert fan mode string to index using helper function
        unit->fanSpeed = getFanIndexFromMQTT(fan);
        DEBUG_PRINT(" -> index ");
        DEBUG_PRINTLN(unit->fanSpeed);
      }
      
      if (doc.containsKey("swing_mode")) {
        const char* swing = doc["swing_mode"];
        DEBUG_PRINT("  - Swing mode: ");
        DEBUG_PRINT(swing);
        // Convert swing mode string to index using helper function
        unit->swingMode = getSwingIndexFromMQTT(swing);
        DEBUG_PRINT(" -> index ");
        DEBUG_PRINTLN(unit->swingMode);
      }
      
      if (doc.containsKey("setpoint")) {
        unit->targetTemp = doc["setpoint"];
        Serial.print("  - Setpoint: ");
        Serial.println(unit->targetTemp);
      }
      
      Serial.print("AFTER UPDATE - Unit ");
      Serial.print(unit->name);
      Serial.print(": Temp=");
      Serial.print(unit->currentTemp);
      Serial.print("°C, Power=");
      Serial.print(unit->isOn ? "on" : "off");
      Serial.print(", Mode=");
      Serial.print(unit->mode);
      Serial.print(", Fan=");
      Serial.print(unit->fanSpeed);
      Serial.print(", Swing=");
      Serial.println(unit->swingMode);
      
      Serial.println("=== MQTT CALLBACK END ===");
      break;
    }
  }
  
  if (!topicMatched) {
    Serial.println("Topic did not match any unit");
  }
}

// Update data for a specific unit
void updateUnitData(int unitIndex) {
  if (!VALIDATE_UNIT_INDEX(unitIndex)) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  if (testMode) {
    // In test mode, simulate temperature changes over time
    static unsigned long lastTempUpdate = 0;
    unsigned long now = millis();
    
    if (now - lastTempUpdate > TEST_MODE_TEMP_UPDATE) { // Update temperature simulation
      // Simulate small temperature fluctuations
      float tempChange = (random(-20, 21) / 100.0); // -0.2 to +0.2 degrees
      unit->currentTemp += tempChange;
      
      // Keep temperature in reasonable range
      if (unit->currentTemp < 18.0) unit->currentTemp = 18.0;
      if (unit->currentTemp > 28.0) unit->currentTemp = 28.0;
      
      lastTempUpdate = now;
    }
    // No need to update other values in test mode as they're
    // controlled by user actions
  } 
  else if (mqttClient.connected()) {
    // In normal mode, data is updated via MQTT callbacks
    // No need to actively poll - data updates happen via callbacks
  } else {
    Serial.print("WARNING: MQTT not connected - skipping data update for unit ");
    Serial.println(unitIndex);
  }
}

// Update all units
void updateAllUnits() {
  for (int i = 0; i < numUnits; i++) {
    updateUnitData(i);
  }
}

// Set AC power state
void setACPower(int unitIndex, bool state) {
  if (!VALIDATE_UNIT_INDEX(unitIndex)) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  if (testMode) {
    // In test mode, just update local state
    unit->isOn = state;
    DEBUG_PRINT("TEST MODE: Setting power for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(state ? "on" : "off");
  } else if (mqttClient.connected()) {
    char topic[MQTT_TOPIC_MAX_LENGTH];
    generateCommandTopic(topic, unit->mqttTopic, MQTT_COMMAND_POWER);
    mqttClient.publish(topic, state ? MQTT_POWER_ON : MQTT_POWER_OFF);
    unit->isOn = state;
    DEBUG_PRINT("MQTT: Setting power for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(state ? "on" : "off");
  }
}

// Set AC mode
void setACMode(int unitIndex, uint8_t mode) {
  if (!VALIDATE_UNIT_INDEX(unitIndex) || !VALIDATE_MODE(mode)) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  if (testMode) {
    // In test mode, just update local state
    unit->mode = mode;
    DEBUG_PRINT("TEST MODE: Setting mode for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(modeNames[mode]);
  } else if (mqttClient.connected()) {
    char topic[MQTT_TOPIC_MAX_LENGTH];
    generateCommandTopic(topic, unit->mqttTopic, MQTT_COMMAND_MODE);
    const char* modeStr = getMQTTModeString(mode);
    mqttClient.publish(topic, modeStr);
    unit->mode = mode;
    DEBUG_PRINT("MQTT: Setting mode for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(modeStr);
  }
}

// Set AC fan speed
void setACFanSpeed(int unitIndex, uint8_t speed) {
  if (!VALIDATE_UNIT_INDEX(unitIndex) || !VALIDATE_FAN_SPEED(speed)) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  if (testMode) {
    // In test mode, just update local state
    unit->fanSpeed = speed;
    DEBUG_PRINT("TEST MODE: Setting fan speed for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(fanNames[speed]);
  } else if (mqttClient.connected()) {
    char topic[MQTT_TOPIC_MAX_LENGTH];
    generateCommandTopic(topic, unit->mqttTopic, MQTT_COMMAND_FAN_MODE);
    const char* fanStr = getMQTTFanString(speed);
    mqttClient.publish(topic, fanStr);
    unit->fanSpeed = speed;
    DEBUG_PRINT("MQTT: Setting fan speed for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(fanStr);
  }
}

// Set AC swing mode
void setACSwing(int unitIndex, uint8_t mode) {
  if (!VALIDATE_UNIT_INDEX(unitIndex) || !VALIDATE_SWING_MODE(mode)) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  if (testMode) {
    // In test mode, just update local state
    unit->swingMode = mode;
    DEBUG_PRINT("TEST MODE: Setting swing for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(swingNames[mode]);
  } else if (mqttClient.connected()) {
    char topic[MQTT_TOPIC_MAX_LENGTH];
    generateCommandTopic(topic, unit->mqttTopic, MQTT_COMMAND_SWING_MODE);
    const char* swingStr = getMQTTSwingString(mode);
    mqttClient.publish(topic, swingStr);
    unit->swingMode = mode;
    DEBUG_PRINT("MQTT: Setting swing mode for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINTLN(swingStr);
  }
}

// Set AC target temperature
void setACTemperature(int unitIndex, float temp) {
  if (!VALIDATE_UNIT_INDEX(unitIndex)) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  // Ensure temperature is within valid range using MQTT config constants
  if (temp < MQTT_TEMP_MIN) temp = MQTT_TEMP_MIN;
  if (temp > MQTT_TEMP_MAX) temp = MQTT_TEMP_MAX;
  
  if (testMode) {
    // In test mode, just update local state
    unit->targetTemp = temp;
    DEBUG_PRINT("TEST MODE: Setting temperature for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINT(temp);
    DEBUG_PRINTLN("°C");
  } else if (mqttClient.connected()) {
    char topic[MQTT_TOPIC_MAX_LENGTH];
    generateCommandTopic(topic, unit->mqttTopic, MQTT_COMMAND_TEMPERATURE);
    mqttClient.publish(topic, String(temp).c_str());
    unit->targetTemp = temp;  // Store actual temperature value
    DEBUG_PRINT("MQTT: Setting temperature for ");
    DEBUG_PRINT(unit->name);
    DEBUG_PRINT(" to ");
    DEBUG_PRINT(temp);
    DEBUG_PRINTLN("°C");
  }
}

// Turn off all AC units
void turnOffAllACUnits() {
  Serial.println("Turning off all AC units via MQTT");
  for (int i = 0; i < numUnits; i++) {
    setACPower(i, false);
    Serial.print("Turning off unit: ");
    Serial.println(acUnits[i].name);
  }
}
