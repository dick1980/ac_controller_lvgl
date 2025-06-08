#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

// MQTT Topic Configuration
// Base topic structure: hcy/airco/{unit_topic}/{topic_type}

// Topic types for commands
#define MQTT_COMMAND_POWER "command/power"
#define MQTT_COMMAND_MODE "command/mode"
#define MQTT_COMMAND_FAN_MODE "command/fan_mode"
#define MQTT_COMMAND_SWING_MODE "command/swing_mode"
#define MQTT_COMMAND_TEMPERATURE "command/temperature"

// Topic types for status
#define MQTT_STATUS "status"

// Topic generation helpers
#define MQTT_TOPIC_MAX_LENGTH 100
#define MQTT_BASE_TOPIC "hcy/airco"

// Helper function declarations for topic generation
void generateMQTTTopic(char* buffer, const char* unitTopic, const char* topicType);
void generateCommandTopic(char* buffer, const char* unitTopic, const char* command);
void generateStatusTopic(char* buffer, const char* unitTopic);

// MQTT Command Values
// Power commands
#define MQTT_POWER_ON "on"
#define MQTT_POWER_OFF "off"

// HVAC Mode mappings (ESP32 index -> MQTT value)
// 0=cool, 1=heat, 2=fan_only, 3=auto, 4=dry
#define MQTT_MODE_COOL "cool"
#define MQTT_MODE_HEAT "heat"
#define MQTT_MODE_FAN_ONLY "fan_only"
#define MQTT_MODE_AUTO "auto"
#define MQTT_MODE_DRY "dry"

// Fan Mode mappings (ESP32 index -> MQTT value)
// 0=low, 1=medium, 2=high, 3=powerful
#define MQTT_FAN_LOW "low"
#define MQTT_FAN_MEDIUM "medium"
#define MQTT_FAN_HIGH "high"
#define MQTT_FAN_POWERFUL "powerful"

// Swing Mode mappings (ESP32 index -> MQTT value)
// 0=swing, 1=position_1, 2=position_2, 3=position_3, 4=position_4
#define MQTT_SWING_SWING "swing"
#define MQTT_SWING_POSITION_1 "position_1"
#define MQTT_SWING_POSITION_2 "position_2"
#define MQTT_SWING_POSITION_3 "position_3"
#define MQTT_SWING_POSITION_4 "position_4"

// Temperature limits
#define MQTT_TEMP_MIN 16.0
#define MQTT_TEMP_MAX 30.0

// Helper functions implementation (inline for efficiency)
inline void generateMQTTTopic(char* buffer, const char* unitTopic, const char* topicType) {
  sprintf(buffer, "%s/%s/%s", MQTT_BASE_TOPIC, unitTopic, topicType);
}

inline void generateCommandTopic(char* buffer, const char* unitTopic, const char* command) {
  generateMQTTTopic(buffer, unitTopic, command);
}

inline void generateStatusTopic(char* buffer, const char* unitTopic) {
  generateMQTTTopic(buffer, unitTopic, MQTT_STATUS);
}

// Function to convert mode index to MQTT string
inline const char* getMQTTModeString(uint8_t mode) {
  switch(mode) {
    case 0: return MQTT_MODE_COOL;
    case 1: return MQTT_MODE_HEAT;
    case 2: return MQTT_MODE_FAN_ONLY;
    case 3: return MQTT_MODE_AUTO;
    case 4: return MQTT_MODE_DRY;
    default: return MQTT_MODE_COOL;
  }
}

// Function to convert fan speed index to MQTT string
inline const char* getMQTTFanString(uint8_t fanSpeed) {
  switch(fanSpeed) {
    case 0: return MQTT_FAN_LOW;
    case 1: return MQTT_FAN_MEDIUM;
    case 2: return MQTT_FAN_HIGH;
    case 3: return MQTT_FAN_POWERFUL;
    default: return MQTT_FAN_LOW;
  }
}

// Function to convert swing mode index to MQTT string
inline const char* getMQTTSwingString(uint8_t swingMode) {
  switch(swingMode) {
    case 0: return MQTT_SWING_SWING;
    case 1: return MQTT_SWING_POSITION_1;
    case 2: return MQTT_SWING_POSITION_2;
    case 3: return MQTT_SWING_POSITION_3;
    case 4: return MQTT_SWING_POSITION_4;
    default: return MQTT_SWING_SWING;
  }
}

// Function to convert MQTT mode string to index
inline uint8_t getModeIndexFromMQTT(const char* mode) {
  if (strcmp(mode, MQTT_MODE_COOL) == 0) return 0;
  if (strcmp(mode, MQTT_MODE_HEAT) == 0) return 1;
  if (strcmp(mode, MQTT_MODE_FAN_ONLY) == 0) return 2;
  if (strcmp(mode, MQTT_MODE_AUTO) == 0) return 3;
  if (strcmp(mode, MQTT_MODE_DRY) == 0) return 4;
  return 0; // Default to cool
}

// Function to convert MQTT fan string to index
inline uint8_t getFanIndexFromMQTT(const char* fan) {
  if (strcmp(fan, MQTT_FAN_LOW) == 0) return 0;
  if (strcmp(fan, MQTT_FAN_MEDIUM) == 0) return 1;
  if (strcmp(fan, MQTT_FAN_HIGH) == 0) return 2;
  if (strcmp(fan, MQTT_FAN_POWERFUL) == 0) return 3;
  return 0; // Default to low
}

// Function to convert MQTT swing string to index
inline uint8_t getSwingIndexFromMQTT(const char* swing) {
  if (strcmp(swing, MQTT_SWING_SWING) == 0) return 0;
  if (strcmp(swing, MQTT_SWING_POSITION_1) == 0) return 1;
  if (strcmp(swing, MQTT_SWING_POSITION_2) == 0) return 2;
  if (strcmp(swing, MQTT_SWING_POSITION_3) == 0) return 3;
  if (strcmp(swing, MQTT_SWING_POSITION_4) == 0) return 4;
  return 0; // Default to swing
}

#endif // MQTT_CONFIG_H