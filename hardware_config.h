#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// Hardware Configuration for ESP32-2432S028

// Touch controller pins
#define XPT2046_CS 33
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25

// Display configuration
#define TFT_BL 21  // Backlight control pin
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
#define TFT_ROTATION 0  // Portrait mode

// Touch calibration values
#define TOUCH_RAW_MIN 300
#define TOUCH_RAW_MAX 3800

// SPI configuration
#define TOUCH_SPI_INSTANCE HSPI

// Serial configuration
#define SERIAL_BAUD_RATE 115200

// LVGL buffer configuration
#define LVGL_BUFFER_SIZE (TFT_WIDTH * 3)  // Buffer size for display

// LVGL timing configuration
#define LVGL_TIMER_INTERVAL 5    // LVGL handler interval in ms
#define LVGL_TICK_INTERVAL 1     // Tick increment interval in ms
#define TOUCH_CHECK_INTERVAL 10  // Touch check interval in ms

// Data update intervals
#define DATA_UPDATE_INTERVAL 2000     // Main data update timer in ms
#define CONNECTION_CHECK_INTERVAL 10000  // MQTT connection check in ms
#define TEST_MODE_TEMP_UPDATE 30000   // Temperature simulation interval in test mode

// Input device configuration
#define INDEV_LONG_PRESS_TIME 400     // Long press time in ms
#define INDEV_SCROLL_LIMIT 5          // Scroll limit for touch

// Memory configuration
#define MQTT_JSON_BUFFER_SIZE 200     // JSON document buffer size

// WiFi configuration
#define WIFI_CONNECTION_TIMEOUT 20    // WiFi connection timeout in iterations (20 * 500ms = 10s)
#define WIFI_CHECK_DELAY 500         // Delay between WiFi connection checks in ms

// MQTT configuration  
#define MQTT_RECONNECT_DELAY 5000    // MQTT reconnection delay in ms
#define MQTT_SUBSCRIPTION_DELAY 1000 // Delay after MQTT subscriptions in ms

// Hardware validation macros
#define VALIDATE_UNIT_INDEX(idx) ((idx) >= 0 && (idx) < numUnits)
#define VALIDATE_MODE(mode) ((mode) <= 4)
#define VALIDATE_FAN_SPEED(speed) ((speed) <= 3)
#define VALIDATE_SWING_MODE(swing) ((swing) <= 4)

// Helper macros for pin validation
#define IS_VALID_PIN(pin) ((pin) >= 0 && (pin) <= 39)

#endif // HARDWARE_CONFIG_H