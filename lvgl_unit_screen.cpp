#include <lvgl.h>
#include "src/ac_controller_lvgl.h"

// Create the unit control screen
void createUnitScreen() {
  // Create unit screen with fixed width (non-scrollable)
  unitScreen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(unitScreen, lv_color_hex(0x1a2639), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(unitScreen, LV_OBJ_FLAG_SCROLLABLE); // Make screen non-scrollable
  
  // Create header area with background like main page - more compact
  lv_obj_t *header = lv_obj_create(unitScreen);
  lv_obj_set_size(header, lv_pct(100), 35); // Reduced height
  lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(header, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);
  
  // Unit title (will be updated in updateUnitScreen) - centered in header with smaller font
  unitTitle = lv_label_create(header);
  lv_label_set_text(unitTitle, "Unit Name");
  lv_obj_center(unitTitle);
  lv_obj_set_style_text_font(unitTitle, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(unitTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create status section to display current temperature and operating mode - more compact
  lv_obj_t *statusSection = lv_obj_create(unitScreen);
  lv_obj_set_size(statusSection, lv_pct(90), 35); // Reduced height
  lv_obj_align(statusSection, LV_ALIGN_TOP_MID, 0, 40); // Positioned closer to header
  lv_obj_set_style_bg_color(statusSection, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(statusSection, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_border_width(statusSection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(statusSection, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Add padding
  lv_obj_clear_flag(statusSection, LV_OBJ_FLAG_SCROLLABLE);
  
  // Current temperature display with smaller font
  tempDisplay = lv_label_create(statusSection);
  lv_label_set_text(tempDisplay, "22.0°C");
  lv_obj_align(tempDisplay, LV_ALIGN_LEFT_MID, 10, 0);
  lv_obj_set_style_text_font(tempDisplay, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(tempDisplay, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Current operating mode display with smaller font
  lv_obj_t *modeLabel = lv_label_create(statusSection);
  lv_label_set_text(modeLabel, "Mode:");
  lv_obj_align(modeLabel, LV_ALIGN_RIGHT_MID, -65, 0); // Adjusted position
  lv_obj_set_style_text_font(modeLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(modeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Mode value with smaller font
  lv_obj_t *modeValue = lv_label_create(statusSection);
  lv_label_set_text(modeValue, "Cool");
  lv_obj_align(modeValue, LV_ALIGN_RIGHT_MID, -10, 0); // Adjusted position
  lv_obj_set_style_text_font(modeValue, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(modeValue, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue for cool
  
  // Temperature control section - more compact
  lv_obj_t *tempSection = lv_obj_create(unitScreen);
  lv_obj_set_size(tempSection, lv_pct(90), 50); // Reduced height
  lv_obj_align(tempSection, LV_ALIGN_TOP_MID, 0, 80); // Moved down from 60 to 80 to avoid overlap
  lv_obj_set_style_bg_color(tempSection, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(tempSection, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_border_width(tempSection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(tempSection, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Add padding
  lv_obj_clear_flag(tempSection, LV_OBJ_FLAG_SCROLLABLE);
  
  lv_obj_t *tempLabel = lv_label_create(tempSection);
  lv_label_set_text(tempLabel, "Temperatuur inst.");
  lv_obj_align(tempLabel, LV_ALIGN_TOP_MID, 0, 0); // Positioned closer to top
  lv_obj_set_style_text_font(tempLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(tempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Temperature control with plus/minus buttons
  // Create a container for the temperature controls
  lv_obj_t *tempControlContainer = lv_obj_create(tempSection);
  lv_obj_remove_style_all(tempControlContainer);
  lv_obj_set_size(tempControlContainer, lv_pct(100), 30);
  lv_obj_align(tempControlContainer, LV_ALIGN_BOTTOM_MID, 0, 0);
  
  // Minus button
  lv_obj_t *minusBtn = lv_btn_create(tempControlContainer);
  lv_obj_set_size(minusBtn, 30, 30);
  lv_obj_align(minusBtn, LV_ALIGN_LEFT_MID, 10, 0);
  lv_obj_set_style_radius(minusBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT); // Make it circular
  lv_obj_set_style_bg_color(minusBtn, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(minusBtn, temp_minus_btn_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *minusLabel = lv_label_create(minusBtn);
  lv_label_set_text(minusLabel, "-");
  lv_obj_set_style_text_font(minusLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_center(minusLabel);
  
  // Temperature value display in the center
  lv_obj_t *tempValue = lv_label_create(tempControlContainer);
  lv_label_set_text(tempValue, "22.0°C");
  lv_obj_center(tempValue);
  lv_obj_set_style_text_font(tempValue, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(tempValue, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Plus button
  lv_obj_t *plusBtn = lv_btn_create(tempControlContainer);
  lv_obj_set_size(plusBtn, 30, 30);
  lv_obj_align(plusBtn, LV_ALIGN_RIGHT_MID, -10, 0);
  lv_obj_set_style_radius(plusBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT); // Make it circular
  lv_obj_set_style_bg_color(plusBtn, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(plusBtn, temp_plus_btn_event_cb, LV_EVENT_CLICKED, tempValue);
  
  lv_obj_t *plusLabel = lv_label_create(plusBtn);
  lv_label_set_text(plusLabel, "+");
  lv_obj_set_style_text_font(plusLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_center(plusLabel);
  
  // Mode selection with roller - more compact
  lv_obj_t *modeSection = lv_obj_create(unitScreen);
  lv_obj_set_size(modeSection, lv_pct(90), 70); // Reduced height
  lv_obj_align(modeSection, LV_ALIGN_TOP_MID, 0, 135); // Moved up from 145 to 135 for better spacing
  lv_obj_set_style_bg_color(modeSection, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(modeSection, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_border_width(modeSection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(modeSection, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Add padding
  lv_obj_clear_flag(modeSection, LV_OBJ_FLAG_SCROLLABLE);
  
  lv_obj_t *modeSectionLabel = lv_label_create(modeSection);
  lv_label_set_text(modeSectionLabel, "Modus");
  lv_obj_align(modeSectionLabel, LV_ALIGN_TOP_MID, 0, 2); // Moved up from 5 to 2 to avoid overlap
  lv_obj_set_style_text_font(modeSectionLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(modeSectionLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create mode button instead of roller
  modeTabView = lv_btn_create(modeSection);
  lv_obj_set_size(modeTabView, 145, 35); // Button size
  lv_obj_align(modeTabView, LV_ALIGN_CENTER, 0, 10); // Moved down from 5 to 10 to avoid overlap
  lv_obj_set_style_bg_color(modeTabView, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue for cool (default)
  lv_obj_set_style_radius(modeTabView, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(modeTabView, mode_button_modal_event_cb, LV_EVENT_CLICKED, NULL);
  
  // Mode label on button
  lv_obj_t *modeButtonLabel = lv_label_create(modeTabView);
  lv_label_set_text(modeButtonLabel, "Koelen"); // Default mode in Dutch
  lv_obj_center(modeButtonLabel);
  lv_obj_set_style_text_font(modeButtonLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(modeButtonLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Fan speed and swing sections in a row - more compact layout
  // Fan speed section with roller - more compact
  lv_obj_t *fanSection = lv_obj_create(unitScreen);
  lv_obj_set_size(fanSection, lv_pct(42), 65); // Increased height from 50 to 65 to fit text properly
  lv_obj_align(fanSection, LV_ALIGN_TOP_LEFT, 10, 210); // Moved up from 225 to 210 for better spacing
  lv_obj_set_style_bg_color(fanSection, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(fanSection, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_border_width(fanSection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(fanSection, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Add padding
  lv_obj_clear_flag(fanSection, LV_OBJ_FLAG_SCROLLABLE);
  
  lv_obj_t *fanLabel = lv_label_create(fanSection);
  lv_label_set_text(fanLabel, "Fan snelh.");
  lv_obj_align(fanLabel, LV_ALIGN_TOP_MID, 0, 2); // Positioned closer to top
  lv_obj_set_style_text_font(fanLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(fanLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Fan speed button instead of roller
  lv_obj_t *fanButton = lv_btn_create(fanSection);
  lv_obj_set_size(fanButton, 90, 30); // Button size
  lv_obj_align(fanButton, LV_ALIGN_BOTTOM_MID, 0, -5); // Adjusted position with more margin
  lv_obj_set_style_bg_color(fanButton, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey button
  lv_obj_set_style_radius(fanButton, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(fanButton, fan_button_event_cb, LV_EVENT_CLICKED, NULL);
  
  // Fan speed label on button
  lv_obj_t *fanButtonLabel = lv_label_create(fanButton);
  lv_label_set_text(fanButtonLabel, "Gemiddeld"); // Default fan speed in Dutch
  lv_obj_center(fanButtonLabel);
  lv_obj_set_style_text_font(fanButtonLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(fanButtonLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Swing switch section - more compact
  lv_obj_t *swingSection = lv_obj_create(unitScreen);
  lv_obj_set_size(swingSection, lv_pct(42), 65); // Increased height from 50 to 65 to match fan section
  lv_obj_align(swingSection, LV_ALIGN_TOP_RIGHT, -10, 210); // Moved up from 225 to 210 for better spacing
  lv_obj_set_style_bg_color(swingSection, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(swingSection, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_border_width(swingSection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(swingSection, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Add padding
  lv_obj_clear_flag(swingSection, LV_OBJ_FLAG_SCROLLABLE);
  
  lv_obj_t *swingLabel = lv_label_create(swingSection);
  lv_label_set_text(swingLabel, "Lamelle");
  lv_obj_align(swingLabel, LV_ALIGN_TOP_MID, 0, 2); // Positioned closer to top
  lv_obj_set_style_text_font(swingLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_set_style_text_color(swingLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

  swingButton = lv_button_create(swingSection);
  lv_obj_align(swingButton, LV_ALIGN_CENTER, 0, 8); // Adjusted position
  lv_obj_set_size(swingButton, 80, 25); // Button size
  lv_obj_set_style_bg_color(swingButton, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(swingButton, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(swingButton, swing_button_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *swingButtonLabel = lv_label_create(swingButton);
  lv_label_set_text(swingButtonLabel, swingNames[0]);
  lv_obj_center(swingButtonLabel);
  lv_obj_set_style_text_color(swingButtonLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Navigation buttons at the bottom - more compact
  // Blue Back button at bottom left
  backButton = lv_btn_create(unitScreen);
  lv_obj_set_size(backButton, 90, 30); // Smaller size
  lv_obj_align(backButton, LV_ALIGN_BOTTOM_LEFT, 10, -5); // Positioned closer to bottom
  lv_obj_set_style_radius(backButton, 6, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_bg_color(backButton, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue
  lv_obj_add_event_cb(backButton, back_button_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *backLabel = lv_label_create(backButton);
  lv_label_set_text(backLabel, "Terug");
  lv_obj_set_style_text_font(backLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller font
  lv_obj_center(backLabel);
  
  // Red Power Off button at bottom right
  powerButton = lv_btn_create(unitScreen);
  lv_obj_set_size(powerButton, 90, 30); // Smaller size
  lv_obj_align(powerButton, LV_ALIGN_BOTTOM_RIGHT, -10, -5); // Positioned closer to bottom
  lv_obj_set_style_radius(powerButton, 6, LV_PART_MAIN | LV_STATE_DEFAULT); // Smaller radius
  lv_obj_set_style_bg_color(powerButton, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT); // Pure red
  lv_obj_add_event_cb(powerButton, power_button_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *powerLabel = lv_label_create(powerButton);
  lv_label_set_text(powerLabel, "POWER"); // Shortened from "POWER OFF" to fit better
  lv_obj_set_style_text_font(powerLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT); // Use available font
  lv_obj_center(powerLabel);
}

// Update the unit screen with current data
void updateUnitScreen(int unitIndex) {
  if (unitIndex < 0 || unitIndex >= numUnits) return;
  
  ACUnit *unit = &acUnits[unitIndex];
  
  // Update unit name in header
  lv_label_set_text(unitTitle, unit->name);
  
  // Update current temperature display
  char tempStr[15];
  sprintf(tempStr, "%.1f°C", unit->currentTemp);
  lv_label_set_text(tempDisplay, tempStr);
  
  // Update current mode display
  lv_obj_t *statusSection = lv_obj_get_child(unitScreen, 1); // Get status section
  lv_obj_t *modeValue = lv_obj_get_child(statusSection, 2); // Get mode value label
  
  // Set mode text and color based on current mode
  if (!unit->isOn) {
    lv_label_set_text(modeValue, "Off");
    lv_obj_set_style_text_color(modeValue, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey for off
  } else {
    lv_label_set_text(modeValue, modeNames[unit->mode]);
    
    // Set color based on mode
    switch(unit->mode) {
      case 0: // Cool
        lv_obj_set_style_text_color(modeValue, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue
        break;
      case 1: // Heat
        lv_obj_set_style_text_color(modeValue, lv_color_hex(0xFF8100), LV_PART_MAIN | LV_STATE_DEFAULT); // Orange
        break;
      case 2: // Fan
        lv_obj_set_style_text_color(modeValue, lv_color_hex(0x8A8A8A), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey
        break;
      case 3: // Auto
        lv_obj_set_style_text_color(modeValue, lv_color_hex(0x008000), LV_PART_MAIN | LV_STATE_DEFAULT); // Green
        break;
      case 4: // Dry
        lv_obj_set_style_text_color(modeValue, lv_color_hex(0xEFBD07), LV_PART_MAIN | LV_STATE_DEFAULT); // Ocher
        break;
      default:
        lv_obj_set_style_text_color(modeValue, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Default blue
    }
  }
  
  // Update temperature value display
  float targetTemp = (float)unit->targetTemp; // Use direct Celsius value from MQTT
  sprintf(tempStr, "%.0f°C", targetTemp);
  lv_obj_t *tempSection = lv_obj_get_child(unitScreen, 2); // Get temperature section
  lv_obj_t *tempControlContainer = lv_obj_get_child(tempSection, 1); // Get temperature control container
  lv_obj_t *tempValue = lv_obj_get_child(tempControlContainer, 1); // Get temperature value label
  lv_label_set_text(tempValue, tempStr);
  
  // Update mode button text and color
  lv_obj_t *modeSection = lv_obj_get_child(unitScreen, 3); // Get mode section
  lv_obj_t *modeButton = lv_obj_get_child(modeSection, 1); // Get mode button
  lv_obj_t *modeButtonLabel = lv_obj_get_child(modeButton, 0); // Get mode button label
  
  if (!unit->isOn) {
    lv_label_set_text(modeButtonLabel, "OFF");
    lv_obj_set_style_bg_color(modeButton, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey for off
  } else {
    lv_label_set_text(modeButtonLabel, modeNames[unit->mode]);
    
    // Set button color based on mode
    switch(unit->mode) {
      case 0: // Cool
        lv_obj_set_style_bg_color(modeButton, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue
        break;
      case 1: // Heat
        lv_obj_set_style_bg_color(modeButton, lv_color_hex(0xFF8100), LV_PART_MAIN | LV_STATE_DEFAULT); // Orange
        break;
      case 2: // Fan
        lv_obj_set_style_bg_color(modeButton, lv_color_hex(0x8A8A8A), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey
        break;
      case 3: // Auto
        lv_obj_set_style_bg_color(modeButton, lv_color_hex(0x008000), LV_PART_MAIN | LV_STATE_DEFAULT); // Green
        break;
      case 4: // Dry
        lv_obj_set_style_bg_color(modeButton, lv_color_hex(0xEFBD07), LV_PART_MAIN | LV_STATE_DEFAULT); // Ocher
        break;
      default:
        lv_obj_set_style_bg_color(modeButton, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Default blue
    }
  }
  
  // Update fan speed button
  lv_obj_t *fanSection = lv_obj_get_child(unitScreen, 4); // Get fan section
  lv_obj_t *fanButton = lv_obj_get_child(fanSection, 1); // Get fan button
  lv_obj_t *fanButtonLabel = lv_obj_get_child(fanButton, 0); // Get fan button label
  lv_label_set_text(fanButtonLabel, fanNames[unit->fanSpeed]);
  
  // Update swing button
  lv_obj_t *swingSection = lv_obj_get_child(unitScreen, 5); // Get swing section
  lv_obj_t *swingButton = lv_obj_get_child(swingSection, 1); // Get swing button
  lv_obj_t *swingButtonLabel = lv_obj_get_child(swingButton, 0); // Get swing button label
  lv_label_set_text(swingButtonLabel, swingNames[unit->swingMode]);
  
  // Update power button text and styling based on current state
  lv_obj_t *powerLabel = lv_obj_get_child(powerButton, 0);
  if (unit->isOn) {
    lv_label_set_text(powerLabel, "POWER");
    lv_obj_set_style_bg_color(powerButton, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT); // Red for power off
    lv_obj_clear_state(powerButton, LV_STATE_DISABLED); // Enable button
    lv_obj_set_style_bg_opa(powerButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT); // Full opacity
  } else {
    lv_label_set_text(powerLabel, "POWER");
    lv_obj_set_style_bg_color(powerButton, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey when off
    lv_obj_add_state(powerButton, LV_STATE_DISABLED); // Disable button when off
    lv_obj_set_style_bg_opa(powerButton, 128, LV_PART_MAIN | LV_STATE_DEFAULT); // Reduced opacity when disabled
  }
}

// Event callbacks for unit screen
void power_button_event_cb(lv_event_t *e) {
  if (selectedUnit < 0 || selectedUnit >= numUnits) return;
  
  // Check if button is disabled (when unit is off)
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  if (lv_obj_has_state(btn, LV_STATE_DISABLED)) return;
  
  // Toggle power (only turns off since button is disabled when unit is off)
  bool newState = !acUnits[selectedUnit].isOn;
  
  // In test mode, directly update the unit's power state without Modbus
  if (testMode) {
    acUnits[selectedUnit].isOn = newState;
  } else {
    // Normal mode - send to Modbus
    setACPower(selectedUnit, newState);
  }
  
  // Update the unit screen to reflect the new power state
  updateUnitScreen(selectedUnit);
}

// Temperature minus button callback
void temp_minus_btn_event_cb(lv_event_t *e) {
  if (selectedUnit < 0 || selectedUnit >= numUnits) return;
  
  // Get current temperature value
  float currentValue = acUnits[selectedUnit].targetTemp;
  
  // Decrease by 1°C
  if (currentValue > 16.0) { // Minimum 16°C
    currentValue -= 1.0;
    
    // In test mode, directly update the unit's target temperature
    if (testMode) {
      acUnits[selectedUnit].targetTemp = currentValue;
      acUnits[selectedUnit].setTemp = currentValue;
    } else {
      // Normal mode - send via MQTT
      setACTemperature(selectedUnit, currentValue);
    }
    
    // Update temperature display
    updateUnitScreen(selectedUnit);
  }
}

// Temperature plus button callback
void temp_plus_btn_event_cb(lv_event_t *e) {
  if (selectedUnit < 0 || selectedUnit >= numUnits) return;
  
  // Get current temperature value
  float currentValue = acUnits[selectedUnit].targetTemp;
  
  // Increase by 1°C
  if (currentValue < 30.0) { // Maximum 30°C
    currentValue += 1.0;
    
    // In test mode, directly update the unit's target temperature
    if (testMode) {
      acUnits[selectedUnit].targetTemp = currentValue;
      acUnits[selectedUnit].setTemp = currentValue;
    } else {
      // Normal mode - send via MQTT
      setACTemperature(selectedUnit, currentValue);
    }
    
    // Update temperature display
    updateUnitScreen(selectedUnit);
  }
}

// Mode selection modal callback
void mode_select_cb(lv_event_t *e) {
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  uint32_t id = (uint32_t)(uintptr_t)lv_obj_get_user_data(btn);
  
  Serial.println("=== MODE SELECTION START ===");
  Serial.print("Selected unit: ");
  Serial.print(selectedUnit);
  Serial.print(" (");
  Serial.print(acUnits[selectedUnit].name);
  Serial.println(")");
  Serial.print("Selected mode: ");
  Serial.print(id);
  Serial.print(" (");
  Serial.print(modeNames[id]);
  Serial.println(")");
  
  // Turn on the unit when a mode is selected
  if (testMode) {
    acUnits[selectedUnit].isOn = true;
    acUnits[selectedUnit].mode = id;
    Serial.println("TEST MODE: Updated local state");
  } else {
    // Normal mode - send to MQTT and update local state optimistically
    if (mqttClient.connected()) {
      Serial.print("BEFORE MQTT - Unit ");
      Serial.print(acUnits[selectedUnit].name);
      Serial.print(": Power=");
      Serial.print(acUnits[selectedUnit].isOn ? "on" : "off");
      Serial.print(", Mode=");
      Serial.println(acUnits[selectedUnit].mode);
      
      // Send MQTT commands
      char powerTopic[100];
      sprintf(powerTopic, "hcy/airco/%s/command/power", acUnits[selectedUnit].mqttTopic);
      mqttClient.publish(powerTopic, "on");
      Serial.print("Published to ");
      Serial.print(powerTopic);
      Serial.println(": on");
      
      char modeTopic[100];
      sprintf(modeTopic, "hcy/airco/%s/command/mode", acUnits[selectedUnit].mqttTopic);
      String modeStr = String(modeNamesEN[id]);
      modeStr.toLowerCase();
      if (modeStr == "fan") modeStr = "fan_only";
      mqttClient.publish(modeTopic, modeStr.c_str());
      Serial.print("Published to ");
      Serial.print(modeTopic);
      Serial.print(": ");
      Serial.println(modeStr);
      
      // Update local state immediately for UI responsiveness
      acUnits[selectedUnit].isOn = true;
      acUnits[selectedUnit].mode = id;
      
      Serial.print("AFTER LOCAL UPDATE - Unit ");
      Serial.print(acUnits[selectedUnit].name);
      Serial.print(": Power=");
      Serial.print(acUnits[selectedUnit].isOn ? "on" : "off");
      Serial.print(", Mode=");
      Serial.println(acUnits[selectedUnit].mode);
    }
  }
  
  // Close the modal
  lv_obj_t *modal = lv_obj_get_parent(lv_obj_get_parent(btn));
  lv_obj_del(modal);
  
  // Update status display
  updateUnitScreen(selectedUnit);
  Serial.println("=== MODE SELECTION END ===");
}

// Mode button event callback - opens modal with mode options
void mode_button_modal_event_cb(lv_event_t *e) {
  if (selectedUnit < 0 || selectedUnit >= numUnits) return;
  
  // Create a modal background
  lv_obj_t *modal = lv_obj_create(lv_scr_act());
  lv_obj_set_size(modal, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(modal, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(modal, 180, LV_PART_MAIN | LV_STATE_DEFAULT); // Semi-transparent
  lv_obj_set_style_border_width(modal, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create a container for the mode buttons
  lv_obj_t *cont = lv_obj_create(modal);
  lv_obj_set_size(cont, 200, 220); // Increased height from 190 to 220 to fit header and all buttons
  lv_obj_center(cont);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); // Make it unscrollable
  
  // Add header text
  lv_obj_t *headerLabel = lv_label_create(cont);
  lv_label_set_text(headerLabel, "Kies Modus");
  lv_obj_align(headerLabel, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_style_text_font(headerLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(headerLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Close button (X) in the top right
  lv_obj_t *closeBtn = lv_btn_create(cont);
  lv_obj_set_size(closeBtn, 24, 24);
  lv_obj_align(closeBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_radius(closeBtn, 12, LV_PART_MAIN | LV_STATE_DEFAULT); // Make it circular
  lv_obj_set_style_bg_color(closeBtn, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey
  lv_obj_add_event_cb(closeBtn, [](lv_event_t *e) {
    // Close the modal without making any changes
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *modal = lv_obj_get_parent(lv_obj_get_parent(btn));
    lv_obj_del(modal);
  }, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *closeLabel = lv_label_create(closeBtn);
  lv_label_set_text(closeLabel, "X"); // Simple X character instead of Unicode
  lv_obj_center(closeLabel);
  lv_obj_set_style_text_font(closeLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create mode buttons (removed OFF mode)
  const char* modes[] = {"Koelen", "Verwarmen", "Ventilatie", "Auto", "Drogen"};
  lv_color_t colors[] = {
    lv_color_hex(0x2B9AF9), // Blue for Cool
    lv_color_hex(0xFF8100), // Orange for Heat
    lv_color_hex(0x8A8A8A), // Grey for Fan
    lv_color_hex(0x008000), // Green for Auto
    lv_color_hex(0xEFBD07)  // Ocher for Dry
  };
  
  for (int i = 0; i < 5; i++) { // Changed from 6 to 5 (removed OFF)
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_set_size(btn, 160, 30);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 55 + i * 32); // Moved down from 30 to 55 to accommodate header
    lv_obj_set_style_bg_color(btn, colors[i], LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, modes[i]);
    lv_obj_center(label);
    
    // Store the mode ID as user data
    lv_obj_set_user_data(btn, (void*)(uintptr_t)i); // For modes 0-4
    
    // Add click event - all buttons use mode_select_cb now
    lv_obj_add_event_cb(btn, mode_select_cb, LV_EVENT_CLICKED, NULL);
  }
}

// Fan speed selection modal callback
void fan_select_cb(lv_event_t *e) {
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  uint32_t id = (uint32_t)(uintptr_t)lv_obj_get_user_data(btn);
  
  // In test mode, directly update the unit's fan speed
  if (testMode) {
    acUnits[selectedUnit].fanSpeed = id;
  } else {
    // Normal mode - send to Modbus
    setACFanSpeed(selectedUnit, id);
  }
  
  // Close the modal
  lv_obj_t *modal = lv_obj_get_parent(lv_obj_get_parent(btn));
  lv_obj_del(modal);
  
  // Update status display
  updateUnitScreen(selectedUnit);
}

// Fan button event callback - opens modal with fan speed options
void fan_button_event_cb(lv_event_t *e) {
  if (selectedUnit < 0 || selectedUnit >= numUnits) return;
  if (!acUnits[selectedUnit].isOn) return; // Don't open fan modal if unit is off
  
  // Create a modal background
  lv_obj_t *modal = lv_obj_create(lv_scr_act());
  lv_obj_set_size(modal, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(modal, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(modal, 180, LV_PART_MAIN | LV_STATE_DEFAULT); // Semi-transparent
  lv_obj_set_style_border_width(modal, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create a container for the fan speed buttons
  lv_obj_t *cont = lv_obj_create(modal);
  lv_obj_set_size(cont, 180, 220); // Increased height to fit header and 4 fan speed buttons
  lv_obj_center(cont);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); // Make it unscrollable
  
  // Add header text
  lv_obj_t *headerLabel = lv_label_create(cont);
  lv_label_set_text(headerLabel, "Kies Fan Snelheid");
  lv_obj_align(headerLabel, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_style_text_font(headerLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(headerLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Close button (X) in the top right
  lv_obj_t *closeBtn = lv_btn_create(cont);
  lv_obj_set_size(closeBtn, 24, 24);
  lv_obj_align(closeBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_radius(closeBtn, 12, LV_PART_MAIN | LV_STATE_DEFAULT); // Make it circular
  lv_obj_set_style_bg_color(closeBtn, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey
  lv_obj_add_event_cb(closeBtn, [](lv_event_t *e) {
    // Close the modal without making any changes
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *modal = lv_obj_get_parent(lv_obj_get_parent(btn));
    lv_obj_del(modal);
  }, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *closeLabel = lv_label_create(closeBtn);
  lv_label_set_text(closeLabel, "X"); // Simple X character instead of Unicode
  lv_obj_center(closeLabel);
  lv_obj_set_style_text_font(closeLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create fan speed buttons - now includes 4 speeds
  const char* speeds[] = {"Laag", "Gemiddeld", "Hoog", "Krachtig"};
  
  for (int i = 0; i < 4; i++) {
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_set_size(btn, 140, 30);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 55 + i * 35); // Moved down from 30 to 55 to accommodate header
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, speeds[i]);
    lv_obj_center(label);
    
    // Store the fan speed ID as user data
    lv_obj_set_user_data(btn, (void*)(uintptr_t)i);
    
    // Add click event
    lv_obj_add_event_cb(btn, fan_select_cb, LV_EVENT_CLICKED, NULL);
  }
}

void swing_button_event_cb(lv_event_t *e) {
  if (selectedUnit < 0 || selectedUnit >= numUnits) return;
  
  // Create a modal background
  lv_obj_t *modal = lv_obj_create(lv_scr_act());
  lv_obj_set_size(modal, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(modal, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(modal, 180, LV_PART_MAIN | LV_STATE_DEFAULT); // Semi-transparent
  lv_obj_set_style_border_width(modal, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create a container for the swing buttons
  lv_obj_t *cont = lv_obj_create(modal);
  lv_obj_set_size(cont, 200, 250); // Increased size to fit all 5 options
  lv_obj_center(cont);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(cont, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); // Make it unscrollable
  
  // Add header text
  lv_obj_t *headerLabel = lv_label_create(cont);
  lv_label_set_text(headerLabel, "Kies Lamelle Modus");
  lv_obj_align(headerLabel, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_style_text_font(headerLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(headerLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Close button (X) in the top right
  lv_obj_t *closeBtn = lv_btn_create(cont);
  lv_obj_set_size(closeBtn, 24, 24);
  lv_obj_align(closeBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_radius(closeBtn, 12, LV_PART_MAIN | LV_STATE_DEFAULT); // Make it circular
  lv_obj_set_style_bg_color(closeBtn, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey
  lv_obj_add_event_cb(closeBtn, [](lv_event_t *e) {
    // Close the modal without making any changes
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *modal = lv_obj_get_parent(lv_obj_get_parent(btn));
    lv_obj_del(modal);
  }, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *closeLabel = lv_label_create(closeBtn);
  lv_label_set_text(closeLabel, "X"); // Simple X character instead of Unicode
  lv_obj_center(closeLabel);
  lv_obj_set_style_text_font(closeLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create swing buttons for all 5 options
  for (int i = 0; i < 5; i++) {
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_set_size(btn, 160, 30);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 40 + i * 35); // Start at 40 to accommodate header
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, swingNames[i]);
    lv_obj_center(label);

    // Store the swing ID as user data
    lv_obj_set_user_data(btn, (void*)(uintptr_t)i);

    // Add click event
    lv_obj_add_event_cb(btn, [](lv_event_t *e) {
      lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
      uint32_t id = (uint32_t)(uintptr_t)lv_obj_get_user_data(btn);

      // In test mode, directly update the unit's swing mode
      if (testMode) {
        acUnits[selectedUnit].swingMode = id;
      } else {
        // Normal mode - send via MQTT
        setACSwing(selectedUnit, id);
      }

      // Close the modal
      lv_obj_t *modal = lv_obj_get_parent(lv_obj_get_parent(btn));
      lv_obj_del(modal);

      // Update status display
      updateUnitScreen(selectedUnit);
    }, LV_EVENT_CLICKED, NULL);
  }
}

void back_button_event_cb(lv_event_t *e) {
  // Return to main screen
  lv_scr_load(mainScreen);
  updateMainScreen();
}
