#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <lvgl.h>

// UI Layout Configuration

// Screen dimensions (should match hardware)
#define UI_SCREEN_WIDTH 240
#define UI_SCREEN_HEIGHT 320

// Page configuration
#define UI_UNITS_PER_PAGE 4

// Main Screen Layout
#define UI_HEADER_HEIGHT 35
#define UI_PAGINATION_HEIGHT 35
#define UI_MASTER_CONTROL_HEIGHT 40

// Unit Card Configuration
#define UI_CARD_WIDTH 220
#define UI_CARD_HEIGHT 50
#define UI_CARD_SPACING 58
#define UI_CARD_START_Y 45
#define UI_CARD_PADDING 5

// Unit Screen Layout
#define UI_UNIT_HEADER_HEIGHT 35
#define UI_UNIT_STATUS_HEIGHT 35
#define UI_UNIT_TEMP_HEIGHT 50
#define UI_UNIT_MODE_HEIGHT 70
#define UI_UNIT_CONTROL_HEIGHT 65
#define UI_UNIT_NAV_HEIGHT 30

// Button Sizes
#define UI_BUTTON_SMALL_WIDTH 50
#define UI_BUTTON_SMALL_HEIGHT 25
#define UI_BUTTON_MEDIUM_WIDTH 90
#define UI_BUTTON_MEDIUM_HEIGHT 30
#define UI_BUTTON_LARGE_WIDTH 160
#define UI_BUTTON_LARGE_HEIGHT 35

// Modal Configuration
#define UI_MODAL_WIDTH 200
#define UI_MODAL_MODE_HEIGHT 220
#define UI_MODAL_FAN_HEIGHT 220
#define UI_MODAL_SWING_HEIGHT 250
#define UI_MODAL_BUTTON_HEIGHT 30
#define UI_MODAL_BUTTON_SPACING 35

// Color Scheme - Main Colors
#define UI_COLOR_BACKGROUND 0x1a2639
#define UI_COLOR_PANEL 0x364156
#define UI_COLOR_ACCENT 0x3FC1C9
#define UI_COLOR_ACCENT_PRESSED 0x2AA1A9

// Color Scheme - Status Colors
#define UI_COLOR_SUCCESS 0x3FC1C9
#define UI_COLOR_ERROR 0xFF5757
#define UI_COLOR_WARNING 0xFFAA00
#define UI_COLOR_DISABLED 0x666666
#define UI_COLOR_TEXT_PRIMARY 0xFFFFFF
#define UI_COLOR_TEXT_SECONDARY 0xAAAAAA

// Color Scheme - AC Mode Colors
#define UI_COLOR_MODE_COOL 0x2B9AF9      // Blue
#define UI_COLOR_MODE_HEAT 0xFF8100      // Orange
#define UI_COLOR_MODE_FAN 0x8A8A8A       // Grey
#define UI_COLOR_MODE_AUTO 0x008000      // Green
#define UI_COLOR_MODE_DRY 0xEFBD07       // Ocher/Yellow

// Color Scheme - Power Button Colors
#define UI_COLOR_POWER_ON 0xFF0000       // Red
#define UI_COLOR_POWER_OFF 0x666666      // Grey

// Color Scheme - Special Colors
#define UI_COLOR_TEMPERATURE 0x3FC1C9    // Cyan for temperature displays
#define UI_COLOR_MODAL_OVERLAY 0x000000  // Black overlay for modals

// Font Configuration
#define UI_FONT_SMALL &lv_font_montserrat_12
#define UI_FONT_MEDIUM &lv_font_montserrat_14
#define UI_FONT_LARGE &lv_font_montserrat_16

// Opacity Values
#define UI_OPACITY_FULL LV_OPA_COVER
#define UI_OPACITY_SEMI LV_OPA_50
#define UI_OPACITY_MODAL 180
#define UI_OPACITY_DISABLED 128
#define UI_OPACITY_TRANSPARENT LV_OPA_TRANSP

// Border and Radius Configuration
#define UI_RADIUS_SMALL 4
#define UI_RADIUS_MEDIUM 6
#define UI_RADIUS_LARGE 10
#define UI_RADIUS_ROUND 15
#define UI_BORDER_WIDTH_THIN 1
#define UI_BORDER_WIDTH_MEDIUM 2
#define UI_BORDER_WIDTH_NONE 0

// Padding and Margins
#define UI_PADDING_NONE 0
#define UI_PADDING_SMALL 2
#define UI_PADDING_MEDIUM 5
#define UI_PADDING_LARGE 10
#define UI_MARGIN_SMALL 5
#define UI_MARGIN_MEDIUM 8
#define UI_MARGIN_LARGE 10

// Animation Configuration
#define UI_ANIM_TIME_FAST 150
#define UI_ANIM_TIME_NORMAL 300
#define UI_ANIM_TIME_SLOW 500

// Status Icon Configuration
#define UI_STATUS_ICON_SIZE 16
#define UI_STATUS_ICON_SPACING 3

// Helper Macros for Common UI Operations
#define UI_SET_BACKGROUND_COLOR(obj, color) \
    lv_obj_set_style_bg_color(obj, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT)

#define UI_SET_TEXT_COLOR(obj, color) \
    lv_obj_set_style_text_color(obj, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT)

#define UI_SET_BORDER_COLOR(obj, color, width) \
    do { \
        lv_obj_set_style_border_color(obj, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT); \
        lv_obj_set_style_border_width(obj, width, LV_PART_MAIN | LV_STATE_DEFAULT); \
    } while(0)

#define UI_SET_RADIUS(obj, radius) \
    lv_obj_set_style_radius(obj, radius, LV_PART_MAIN | LV_STATE_DEFAULT)

#define UI_SET_PADDING(obj, padding) \
    lv_obj_set_style_pad_all(obj, padding, LV_PART_MAIN | LV_STATE_DEFAULT)

#define UI_MAKE_UNSCROLLABLE(obj) \
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE)

#define UI_MAKE_CLICKABLE(obj) \
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE)

// Theme helper functions
inline lv_color_t ui_get_mode_color(uint8_t mode) {
    switch(mode) {
        case 0: return lv_color_hex(UI_COLOR_MODE_COOL);
        case 1: return lv_color_hex(UI_COLOR_MODE_HEAT);
        case 2: return lv_color_hex(UI_COLOR_MODE_FAN);
        case 3: return lv_color_hex(UI_COLOR_MODE_AUTO);
        case 4: return lv_color_hex(UI_COLOR_MODE_DRY);
        default: return lv_color_hex(UI_COLOR_MODE_COOL);
    }
}

inline lv_color_t ui_get_power_color(bool isOn) {
    return lv_color_hex(isOn ? UI_COLOR_POWER_ON : UI_COLOR_POWER_OFF);
}

inline lv_color_t ui_get_status_color(bool connected) {
    return lv_color_hex(connected ? UI_COLOR_SUCCESS : UI_COLOR_ERROR);
}

#endif // UI_CONFIG_H