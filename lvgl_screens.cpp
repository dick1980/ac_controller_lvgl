#include <lvgl.h>
#include "ac_controller_lvgl.h"

// Create the loading screen
void createLoadingScreen() {
  loadingScreen = lv_obj_create(NULL);
  lv_obj_set_size(loadingScreen, 240, 320); // Set to 240x320 portrait layout
  lv_obj_set_style_bg_color(loadingScreen, lv_color_hex(0x1a2639), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create a spinner
  lv_obj_t *spinner = lv_spinner_create(loadingScreen);
  lv_obj_set_size(spinner, 60, 60); // Slightly smaller for portrait layout
  lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -20);
  
  // Create a label
  lv_obj_t *label = lv_label_create(loadingScreen);
  lv_label_set_text(label, "Connecting...");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 50);
  lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create the main screen with unit cards and pagination
void createMainScreen() {
  // Create main screen for 240x320 portrait layout
  mainScreen = lv_obj_create(NULL);
  lv_obj_set_size(mainScreen, 240, 320); // Fixed 240x320 portrait layout
  lv_obj_set_style_bg_color(mainScreen, lv_color_hex(0x1a2639), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(mainScreen, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
  
  // Create header area for title and status icons
  lv_obj_t *headerArea = lv_obj_create(mainScreen);
  lv_obj_set_size(headerArea, 240, 35);
  lv_obj_align(headerArea, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(headerArea, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(headerArea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(headerArea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(headerArea, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(headerArea, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
  
  // Create title - left-aligned
  mainTitle = lv_label_create(headerArea);
  lv_label_set_text(mainTitle, "AC Bediening HCY");
  lv_obj_align(mainTitle, LV_ALIGN_LEFT_MID, 5, 0);
  lv_obj_set_style_text_font(mainTitle, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(mainTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create status icons container - right-aligned
  lv_obj_t *statusIcons = lv_obj_create(headerArea);
  lv_obj_set_size(statusIcons, 70, 20);
  lv_obj_align(statusIcons, LV_ALIGN_RIGHT_MID, -5, 0);
  lv_obj_set_style_bg_opa(statusIcons, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT); // Transparent background
  lv_obj_set_style_border_width(statusIcons, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(statusIcons, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_flex_flow(statusIcons, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(statusIcons, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(statusIcons, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
  
  // Create test mode icon
  testModeIcon = lv_label_create(statusIcons);
  lv_label_set_text(testModeIcon, "T");
  lv_obj_set_style_text_font(testModeIcon, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(testModeIcon, lv_color_hex(0xFFAA00), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(testModeIcon, lv_color_hex(0x1a2639), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(testModeIcon, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(testModeIcon, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(testModeIcon, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create MQTT connection icon
  mqttIcon = lv_label_create(statusIcons);
  lv_label_set_text(mqttIcon, "M");
  lv_obj_set_style_text_font(mqttIcon, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(mqttIcon, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(mqttIcon, lv_color_hex(0x1a2639), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(mqttIcon, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(mqttIcon, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create WiFi connection icon
  wifiIcon = lv_label_create(statusIcons);
  lv_label_set_text(wifiIcon, "W");
  lv_obj_set_style_text_font(wifiIcon, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(wifiIcon, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(wifiIcon, lv_color_hex(0x1a2639), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(wifiIcon, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(wifiIcon, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(wifiIcon, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Hide the old connection status label
  connectionStatus = lv_label_create(mainScreen);
  lv_obj_add_flag(connectionStatus, LV_OBJ_FLAG_HIDDEN);
  
  // Optimize units per page for 240x320 portrait layout
  unitsPerPage = 4; // Reduced from 5 to 4 units per page to prevent overlap with pagination
  
  // Create unit cards (will be populated in updateMainScreen)
  for (int i = 0; i < unitsPerPage; i++) {
    unitCards[i] = lv_obj_create(mainScreen);
    lv_obj_set_size(unitCards[i], 220, 50); // Slightly increased height for better readability
    lv_obj_align(unitCards[i], LV_ALIGN_TOP_MID, 0, 45 + i * 58); // Adjusted spacing: 45px start + 58px spacing
    lv_obj_set_style_radius(unitCards[i], 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(unitCards[i], lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(unitCards[i], 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(unitCards[i], lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(unitCards[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT); // Remove shadow
    lv_obj_set_style_pad_all(unitCards[i], 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Add padding
    
    // Make cards more touch-friendly with enhanced settings
    lv_obj_add_flag(unitCards[i], LV_OBJ_FLAG_CLICKABLE); // Ensure clickable
    lv_obj_clear_flag(unitCards[i], LV_OBJ_FLAG_SCROLL_ON_FOCUS); // Prevent scrolling
    lv_obj_clear_flag(unitCards[i], LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    lv_obj_set_style_bg_opa(unitCards[i], LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Add visual feedback for touch states
    lv_obj_set_style_bg_color(unitCards[i], lv_color_hex(0x465166), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(unitCards[i], lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(unitCards[i], 2, LV_PART_MAIN | LV_STATE_PRESSED);
    
    // Add click event callback to each card with multiple event types
    lv_obj_add_event_cb(unitCards[i], unit_card_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(unitCards[i], unit_card_event_cb, LV_EVENT_PRESSED, NULL);
    
    // Initially hide cards
    lv_obj_add_flag(unitCards[i], LV_OBJ_FLAG_HIDDEN);
  }
  
  // Create pagination row - fixed at bottom
  lv_obj_t *paginationRow = lv_obj_create(mainScreen);
  lv_obj_set_size(paginationRow, 240, 35); // Reduced height to fit layout
  lv_obj_align(paginationRow, LV_ALIGN_BOTTOM_MID, 0, 0); // Fixed position at bottom
  lv_obj_set_style_radius(paginationRow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(paginationRow, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(paginationRow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(paginationRow, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(paginationRow, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
  
  // Previous button - more touch-friendly
  prevButton = lv_btn_create(paginationRow);
  lv_obj_set_size(prevButton, 50, 25); // Optimized button size
  lv_obj_align(prevButton, LV_ALIGN_LEFT_MID, 8, 0);
  lv_obj_set_style_radius(prevButton, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Square corners
  lv_obj_set_style_bg_color(prevButton, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  // Add feedback states for touch
  lv_obj_set_style_bg_color(prevButton, lv_color_hex(0x2AA1A9), LV_PART_MAIN | LV_STATE_PRESSED);
  // Make sure button is clickable
  lv_obj_clear_flag(prevButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(prevButton, LV_OBJ_FLAG_CLICKABLE);
  // Add event callback with explicit debug output
  lv_obj_add_event_cb(prevButton, prev_page_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *prevIcon = lv_label_create(prevButton);
  lv_label_set_text(prevIcon, LV_SYMBOL_LEFT);
  lv_obj_center(prevIcon);
  
  // Next button - more touch-friendly
  nextButton = lv_btn_create(paginationRow);
  lv_obj_set_size(nextButton, 50, 25); // Optimized button size
  lv_obj_align(nextButton, LV_ALIGN_RIGHT_MID, -8, 0);
  lv_obj_set_style_radius(nextButton, 5, LV_PART_MAIN | LV_STATE_DEFAULT); // Square corners
  lv_obj_set_style_bg_color(nextButton, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  // Add feedback states for touch
  lv_obj_set_style_bg_color(nextButton, lv_color_hex(0x2AA1A9), LV_PART_MAIN | LV_STATE_PRESSED);
  // Make sure button is clickable
  lv_obj_clear_flag(nextButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
  lv_obj_add_flag(nextButton, LV_OBJ_FLAG_CLICKABLE);
  // Add event callback with explicit debug output
  lv_obj_add_event_cb(nextButton, next_page_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *nextIcon = lv_label_create(nextButton);
  lv_label_set_text(nextIcon, LV_SYMBOL_RIGHT);
  lv_obj_center(nextIcon);
  
  // Page indicator - smaller font
  pageIndicator = lv_label_create(paginationRow);
  lv_label_set_text(pageIndicator, "Pagina 1/3");
  lv_obj_align(pageIndicator, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(pageIndicator, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(pageIndicator, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Update the main screen with current data
void updateMainScreen() {
  // Update status icons
  
  // Test mode icon
  if (testMode) {
    lv_obj_clear_flag(testModeIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_color(testModeIcon, lv_color_hex(0xFFAA00), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    lv_obj_add_flag(testModeIcon, LV_OBJ_FLAG_HIDDEN);
  }
  
  // MQTT connection icon
  if (WiFi.status() == WL_CONNECTED && mqttClient.connected()) {
    lv_obj_clear_flag(mqttIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_color(mqttIcon, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue for connected
  } else {
    lv_obj_clear_flag(mqttIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_color(mqttIcon, lv_color_hex(0xFF5757), LV_PART_MAIN | LV_STATE_DEFAULT); // Red for disconnected
  }
  
  // WiFi connection icon
  if (WiFi.status() == WL_CONNECTED) {
    lv_obj_clear_flag(wifiIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_color(wifiIcon, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue for connected
  } else {
    lv_obj_clear_flag(wifiIcon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_color(wifiIcon, lv_color_hex(0xFF5757), LV_PART_MAIN | LV_STATE_DEFAULT); // Red for disconnected
  }
  
  // Calculate pagination
  int totalPages = (numUnits + unitsPerPage - 1) / unitsPerPage;
  int startUnit = currentPage * unitsPerPage;
  int endUnit = min(startUnit + unitsPerPage, numUnits);
  
  // Update page indicator
  char pageText[20];
  sprintf(pageText, "Pagina %d/%d", currentPage + 1, totalPages);
  lv_label_set_text(pageIndicator, pageText);
  
  // First, hide all unit cards
  for (int i = 0; i < unitsPerPage; i++) {
    lv_obj_add_flag(unitCards[i], LV_OBJ_FLAG_HIDDEN);
    lv_obj_clean(unitCards[i]); // Clear all children
  }
  
  // Update unit cards
  for (int i = 0; i < unitsPerPage; i++) {
    int unitIndex = startUnit + i;
    
    if (unitIndex < numUnits) {
      // Show and update card
      lv_obj_clear_flag(unitCards[i], LV_OBJ_FLAG_HIDDEN);
      
      // Store unit index in user data
      lv_obj_set_user_data(unitCards[i], (void*)(intptr_t)unitIndex);
      
      // Reset background color and add visual feedback for touch
      lv_obj_set_style_bg_color(unitCards[i], lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(unitCards[i], lv_color_hex(0x465166), LV_PART_MAIN | LV_STATE_PRESSED);
      
      // Unit name - left side with proper spacing
      lv_obj_t *nameLabel = lv_label_create(unitCards[i]);
      lv_label_set_text(nameLabel, acUnits[unitIndex].name);
      lv_obj_align(nameLabel, LV_ALIGN_LEFT_MID, 8, 0);
      lv_obj_set_style_text_font(nameLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_text_color(nameLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_width(nameLabel, 130); // Adequate width for unit names
      lv_label_set_long_mode(nameLabel, LV_LABEL_LONG_DOT); // Truncate with dots if too long
      
      // Current temperature - center right
      char tempStr[10];
      sprintf(tempStr, "%.1f°C", acUnits[unitIndex].currentTemp);
      lv_obj_t *tempLabel = lv_label_create(unitCards[i]);
      lv_label_set_text(tempLabel, tempStr);
      lv_obj_align(tempLabel, LV_ALIGN_RIGHT_MID, -30, 0);
      lv_obj_set_style_text_font(tempLabel, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_text_color(tempLabel, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
      
      // Status info removed as requested
      
      // Status indicator - right side with proper spacing
      lv_obj_t *statusIndicator = lv_obj_create(unitCards[i]);
      lv_obj_set_size(statusIndicator, 16, 16);
      lv_obj_align(statusIndicator, LV_ALIGN_RIGHT_MID, -8, 0);
      lv_obj_set_style_radius(statusIndicator, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
      
      // Set border style for all states
      lv_obj_set_style_border_width(statusIndicator, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
      
      if (acUnits[unitIndex].isOn) {
        // Unit is on - show color based on mode
        switch(acUnits[unitIndex].mode) {
          case 0: // Cool
            lv_obj_set_style_bg_color(statusIndicator, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Blue
            lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT);
            break;
          case 1: // Heat
            lv_obj_set_style_bg_color(statusIndicator, lv_color_hex(0xFF8100), LV_PART_MAIN | LV_STATE_DEFAULT); // Orange
            lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0xFF8100), LV_PART_MAIN | LV_STATE_DEFAULT);
            break;
          case 2: // Fan
            lv_obj_set_style_bg_color(statusIndicator, lv_color_hex(0x8A8A8A), LV_PART_MAIN | LV_STATE_DEFAULT); // Grey
            lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0x8A8A8A), LV_PART_MAIN | LV_STATE_DEFAULT);
            break;
          case 3: // Auto
            lv_obj_set_style_bg_color(statusIndicator, lv_color_hex(0x008000), LV_PART_MAIN | LV_STATE_DEFAULT); // Green
            lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0x008000), LV_PART_MAIN | LV_STATE_DEFAULT);
            break;
          case 4: // Dry
            lv_obj_set_style_bg_color(statusIndicator, lv_color_hex(0xEFBD07), LV_PART_MAIN | LV_STATE_DEFAULT); // Ocher
            lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0xEFBD07), LV_PART_MAIN | LV_STATE_DEFAULT);
            break;
          default:
            lv_obj_set_style_bg_color(statusIndicator, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT); // Default blue
            lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0x2B9AF9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        lv_obj_set_style_bg_opa(statusIndicator, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
      } else {
        // Unit is off - show outline only
        lv_obj_set_style_bg_opa(statusIndicator, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT); // Transparent background
        lv_obj_set_style_border_color(statusIndicator, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT); // Light grey border
      }
    } else {
      // Hide unused cards
      lv_obj_add_flag(unitCards[i], LV_OBJ_FLAG_HIDDEN);
    }
  }
  
  // Update pagination buttons
  if (currentPage <= 0) {
    lv_obj_add_state(prevButton, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(prevButton, lv_color_hex(0x777777), LV_PART_MAIN | LV_STATE_DISABLED);
  } else {
    lv_obj_clear_state(prevButton, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(prevButton, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  
  if (currentPage >= totalPages - 1) {
    lv_obj_add_state(nextButton, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(nextButton, lv_color_hex(0x777777), LV_PART_MAIN | LV_STATE_DISABLED);
  } else {
    lv_obj_clear_state(nextButton, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(nextButton, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  }
}

// Event callbacks for main screen
void unit_card_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *card = (lv_obj_t *)lv_event_get_target(e);
  int unitIndex = (int)(intptr_t)lv_obj_get_user_data(card);
  
  // Provide visual feedback based on event type
  if (code == LV_EVENT_PRESSED) {
    Serial.print("Card PRESSED for unit: ");
    Serial.println(unitIndex);
    // Visual feedback is handled by styles
    return;
  }
  
  if (code == LV_EVENT_CLICKED) {
    Serial.print("Card CLICKED for unit: ");
    Serial.println(unitIndex);
    
    // Store the selected unit index
    selectedUnit = unitIndex;
    
    // Show a brief visual feedback before switching screens
    lv_obj_set_style_bg_color(card, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    Serial.print("Opening detail screen for unit: ");
    Serial.println(unitIndex);
    
    // Force LVGL to process the current state
    lv_timer_handler();
    
    // Small delay for visual feedback
    delay(50);
    
    // Load the redesigned unit screen instead of the detail screen
    lv_scr_load(unitScreen);
    
    // Update the unit screen with the selected unit's data
    updateUnitScreen(unitIndex);
    
    // Force LVGL to process the screen change immediately
    lv_timer_handler();
    
    Serial.println("Unit control screen should now be visible");
  }
}

void prev_page_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  
  if (code == LV_EVENT_CLICKED) {
    Serial.println("Previous page button clicked");
    
    // Show visual feedback
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2AA1A9), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Force LVGL to process the button press visual feedback
    lv_timer_handler();
    
    if (currentPage > 0) {
      currentPage--;
      Serial.print("Moving to page: ");
      Serial.println(currentPage);
      
      // Small delay for visual feedback
      delay(50);
      
      // Update the screen
      updateMainScreen();
      
      // Force LVGL to process the screen update immediately
      lv_timer_handler();
      Serial.println("Screen should now be updated to previous page");
    } else {
      Serial.println("Already at first page");
      
      // Small delay for visual feedback
      delay(50);
      
      // Reset button color
      lv_obj_set_style_bg_color(btn, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
      
      // Force LVGL to process the button color reset
      lv_timer_handler();
    }
  }
}

void next_page_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  
  if (code == LV_EVENT_CLICKED) {
    Serial.println("Next page button clicked");
    
    // Show visual feedback
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2AA1A9), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Force LVGL to process the button press visual feedback
    lv_timer_handler();
    
    int totalPages = (numUnits + unitsPerPage - 1) / unitsPerPage;
    Serial.print("Total pages: ");
    Serial.println(totalPages);
    
    if (currentPage < totalPages - 1) {
      currentPage++;
      Serial.print("Moving to page: ");
      Serial.println(currentPage);
      
      // Small delay for visual feedback
      delay(50);
      
      // Update the screen
      updateMainScreen();
      
      // Force LVGL to process the screen update immediately
      lv_timer_handler();
      Serial.println("Screen should now be updated to next page");
    } else {
      Serial.println("Already at last page");
      
      // Small delay for visual feedback
      delay(50);
      
      // Reset button color
      lv_obj_set_style_bg_color(btn, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
      
      // Force LVGL to process the button color reset
      lv_timer_handler();
    }
  }
}
