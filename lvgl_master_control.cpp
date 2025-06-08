#include <lvgl.h>
#include "src/ac_controller_lvgl.h"

// Create and add master control panel to main screen
void createMasterControl() {
  // Create master control panel - positioned at bottom of screen with fixed width
  lv_obj_t *masterPanel = lv_obj_create(mainScreen);
  lv_obj_set_size(masterPanel, 240, 40); // Fixed width, compact height
  lv_obj_align(masterPanel, LV_ALIGN_TOP_MID, 0, 275); // Fixed position from top
  lv_obj_set_style_radius(masterPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // No rounded corners
  lv_obj_set_style_bg_color(masterPanel, lv_color_hex(0x364156), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(masterPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(masterPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT); // No padding
  lv_obj_clear_flag(masterPanel, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
  
  // Create a row container for buttons - fixed and unscrollable
  lv_obj_t *btnRow = lv_obj_create(masterPanel);
  lv_obj_remove_style_all(btnRow); // Remove background
  lv_obj_set_size(btnRow, 240, 30); // Fixed width
  lv_obj_set_flex_flow(btnRow, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(btnRow, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_align(btnRow, LV_ALIGN_CENTER, 0, 0);
  lv_obj_clear_flag(btnRow, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
  
  // All ON button - more touch-friendly
  lv_obj_t *allOnBtn = lv_btn_create(btnRow);
  lv_obj_set_size(allOnBtn, 100, 30);
  lv_obj_set_style_radius(allOnBtn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(allOnBtn, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(allOnBtn, lv_color_hex(0x2AA1A9), LV_PART_MAIN | LV_STATE_PRESSED); // Feedback
  lv_obj_add_event_cb(allOnBtn, all_on_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *allOnLabel = lv_label_create(allOnBtn);
  lv_label_set_text(allOnLabel, "ALL ON");
  lv_obj_center(allOnLabel);
  
  // All OFF button
  lv_obj_t *allOffBtn = lv_btn_create(btnRow);
  lv_obj_set_size(allOffBtn, 100, 30);
  lv_obj_set_style_radius(allOffBtn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(allOffBtn, lv_color_hex(0xE63946), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(allOffBtn, lv_color_hex(0xC62936), LV_PART_MAIN | LV_STATE_PRESSED); // Feedback
  lv_obj_add_event_cb(allOffBtn, all_off_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *allOffLabel = lv_label_create(allOffBtn);
  lv_label_set_text(allOffLabel, "ALL OFF");
  lv_obj_center(allOffLabel);
}

// Event callbacks for master control
void all_on_event_cb(lv_event_t *e) {
  // Show confirmation dialog with fixed width
  lv_obj_t *mbox = lv_msgbox_create(NULL);
  
  // Set fixed width and make unscrollable
  lv_obj_set_size(mbox, 220, LV_SIZE_CONTENT);
  lv_obj_set_style_max_width(mbox, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(mbox, LV_OBJ_FLAG_SCROLLABLE);
  
  // Find the content area and make it unscrollable too
  lv_obj_t *content = lv_msgbox_get_content(mbox);
  if (content) {
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLLABLE);
  }
  
  // Add text and title
  lv_msgbox_add_text(mbox, "Turn ON all AC units?");
  lv_msgbox_add_title(mbox, "Confirmation");
  
  // Add buttons individually
  lv_msgbox_add_footer_button(mbox, "Cancel");
  lv_msgbox_add_footer_button(mbox, "Confirm");
  
  // Add event callback to return to main screen when closed
  lv_obj_add_event_cb(mbox, all_on_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_center(mbox);
}

void all_on_msgbox_event_cb(lv_event_t *e) {
  lv_obj_t *mbox = (lv_obj_t *)lv_event_get_current_target(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  uint16_t btn_id = lv_obj_get_index(btn);
  
  if (btn_id == 1) { // Confirm button
    setAllACPower(true);
    
    // Show notification with fixed width
    lv_obj_t *notification = lv_obj_create(lv_layer_top());
    lv_obj_set_size(notification, 220, 50);
    lv_obj_set_style_bg_color(notification, lv_color_hex(0x3FC1C9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(notification, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(notification, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(notification, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(notification, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
    lv_obj_center(notification);
    
    lv_obj_t *notifLabel = lv_label_create(notification);
    lv_label_set_text(notifLabel, "All units turned ON");
    lv_obj_center(notifLabel);
    
    // Return to main screen
    lv_scr_load(mainScreen);
    
    // Auto-close notification after 1.5 seconds
    lv_timer_t *notifTimer = lv_timer_create(notification_timer_cb, 1500, notification);
    lv_timer_set_repeat_count(notifTimer, 1);
  }
  
  lv_msgbox_close(mbox);
}

void all_off_event_cb(lv_event_t *e) {
  // Show confirmation dialog with fixed width
  lv_obj_t *mbox = lv_msgbox_create(NULL);
  
  // Set fixed width and make unscrollable
  lv_obj_set_size(mbox, 220, LV_SIZE_CONTENT);
  lv_obj_set_style_max_width(mbox, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(mbox, LV_OBJ_FLAG_SCROLLABLE);
  
  // Find the content area and make it unscrollable too
  lv_obj_t *content = lv_msgbox_get_content(mbox);
  if (content) {
    lv_obj_clear_flag(content, LV_OBJ_FLAG_SCROLLABLE);
  }
  
  // Add text and title
  lv_msgbox_add_text(mbox, "Turn OFF all AC units?");
  lv_msgbox_add_title(mbox, "Confirmation");
  
  // Add buttons individually
  lv_msgbox_add_footer_button(mbox, "Cancel");
  lv_msgbox_add_footer_button(mbox, "Confirm");
  
  // Add event callback to return to main screen when closed
  lv_obj_add_event_cb(mbox, all_off_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_center(mbox);
}

void all_off_msgbox_event_cb(lv_event_t *e) {
  lv_obj_t *mbox = (lv_obj_t *)lv_event_get_current_target(e);
  lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
  uint16_t btn_id = lv_obj_get_index(btn);
  
  if (btn_id == 1) { // Confirm button
    setAllACPower(false);
    
    // Show notification with fixed width
    lv_obj_t *notification = lv_obj_create(lv_layer_top());
    lv_obj_set_size(notification, 220, 50);
    lv_obj_set_style_bg_color(notification, lv_color_hex(0xFF5757), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(notification, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(notification, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(notification, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(notification, LV_OBJ_FLAG_SCROLLABLE); // Make unscrollable
    lv_obj_center(notification);
    
    lv_obj_t *notifLabel = lv_label_create(notification);
    lv_label_set_text(notifLabel, "All units turned OFF");
    lv_obj_center(notifLabel);
    
    // Return to main screen
    lv_scr_load(mainScreen);
    
    // Auto-close notification after 1.5 seconds
    lv_timer_t *notifTimer = lv_timer_create(notification_timer_cb, 1500, notification);
    lv_timer_set_repeat_count(notifTimer, 1);
  }
  
  lv_msgbox_close(mbox);
}

// Timer callback for auto-closing notifications
void notification_timer_cb(lv_timer_t *timer) {
  lv_obj_t *notification = (lv_obj_t *)lv_timer_get_user_data(timer);
  lv_obj_del(notification);
}
