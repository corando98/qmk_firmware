/* Copyright 2021 Kyle McCreery
 * Copyright 2021 Jonavin Eng
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "jonavin.h"
#include "layout_landscape.h"

#define LANDSCAPE_MODE

// Defines names for use in layer keycodes and the keymap
enum layer_names {
	_FN2 = 2,
	_FN3,
    _FN4,
    _RGB
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_landscape(
        TT(_FN1), TT(_FN2), KC_MUTE,

        KC_NLCK,  KC_PSLS,  KC_PAST, KC_PMNS, KC_PPLS,      KC_BSPC,
        KC_TAB,   KC_P7,    KC_P8,   KC_P9,   KC_PDOT,      KC_RGUI,
        KC_RSFT,  KC_P4,    KC_P5,   KC_P6,   KC_COMMA,     KC_RCTL,
        KC_P0,    KC_P1,    KC_P2,   KC_P3,   KC_PENT,      KC_RALT,

                 TT(_FN3), TT(_FN4), TT(_RGB)
    ),
    [_FN1] = LAYOUT_landscape(
        _______,  _______, ENCFUNC,

        _______,  KC_F10,   KC_F11,  KC_F12,  KC_PSCR,      _______,
        _______,  KC_F7,    KC_F8,   KC_F9,   KC_SLCK,      _______,
        _______,  KC_F4,    KC_F5,   KC_F6,   KC_PAUS,      _______,
        _______,  KC_F1,    KC_F2,   KC_F3,   _______,      _______,

                 _______, _______, _______
    ),
	[_FN2] = LAYOUT_landscape(
        _______,  _______, RESET,

        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,

                  _______,  _______, _______
    ),
	[_FN3] = LAYOUT_landscape(
        _______,  _______, _______,

        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,

                  _______,  _______, _______
   ),
	[_FN4] = LAYOUT_landscape(
        _______,  _______, _______,

        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,
        _______,  _______,  _______,  _______, _______,     _______,

                  _______,  _______, _______
   ),
	[_RGB] = LAYOUT_landscape(
        _______,  _______, _______,

        _______,  _______,  _______,  _______, _______,     _______,
        _______,  RGB_HUD,  RGB_SPI, RGB_HUI,  RGB_SAI,     _______,
        _______,  RGB_RMOD, RGB_TOG, RGB_MOD,  RGB_SAD,     _______,
        _______,  RGB_VAD,  RGB_SPD, RGB_VAI,  KC_NO,       _______,

                  _______,  _______, _______
   ),
};

typedef struct {
     char keydesc[6];    // this will be displayed on OLED
    uint16_t keycode;   // this is the keycode that will be sent when activted
} keycodedescType;

static const keycodedescType PROGMEM keyselection[] = {
    // list of key codes that will be scrolled through by encoder and description
        {"TASK",    KC_TASK},
        {"INS",     KC_INS},
        {"DEL",     KC_DEL},
        {"PrtSc",   KC_PSCR},
        {"ScrLk",   KC_SCLN},
        {"Break",   KC_PAUS},
        {"C-A-D",   KC_CAD},  // Ctrl-Alt-Del
        {"AltF4",   KC_AF4},
        {"PLAY",    KC_MEDIA_PLAY_PAUSE},
        {"RESET",   RESET},   // firmware flash mode
};

#define MAX_KEYSELECTION sizeof(keyselection)/sizeof(keyselection[0])

static uint8_t selectedkey_idx = 0;
static keycodedescType selectedkey_rec;

static void set_selectedkey(uint8_t idx) {
    // make a copy from PROGMEM
    memcpy_P (&selectedkey_rec, &keyselection[idx], sizeof selectedkey_rec);

    //selectedkey_rec = keyselection[idx];

}

void keyboard_post_init_keymap(void) {
  // Call the keyboard post init code.
    set_selectedkey(selectedkey_idx);
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case ENCFUNC:
        if (record->event.pressed) {
            selectedkey_rec.keycode == RESET ? reset_keyboard() : tap_code16(selectedkey_rec.keycode); // handle RESET code
        } else {
            // when keycode is released
        }
        break;
    }
    return true;
};


#ifdef ENCODER_ENABLE       // Encoder Functionality
    void encoder_action_selectkey(bool clockwise) {
        if ( clockwise ) {
            if ( selectedkey_idx  < MAX_KEYSELECTION-1) {
                selectedkey_idx ++;
            } else {
                // do nothing
            }
        } else if ( !clockwise ) {
            if ( selectedkey_idx  > 0){
                selectedkey_idx --;
            } else {
                // do nothing
            }
        }
        set_selectedkey(selectedkey_idx);
    }

    bool encoder_update_user(uint8_t index, bool clockwise) {
        #ifdef OLED_ENABLE
            oled_clear();
            oled_render();
        #endif
        uint8_t mods_state = get_mods();
        switch (index) {
            case 0:         // This is the only encoder right now, keeping for consistency
            switch(get_highest_layer(layer_state)){  // special handling per layer
            case _FN1:  // on Fn layer select what the encoder does when pressed
                if (!mods_state) {
                    encoder_action_selectkey(clockwise);
                    break;
                } else {
                        // continue to default
                }
            default:   // all other layers
                if (mods_state & MOD_BIT(KC_RSFT) ) { // If you are holding R shift, encoder changes layers
                    encoder_action_layerchange(clockwise);
                } else if (mods_state & MOD_BIT(KC_RCTL)) {  // if holding Right Ctrl, navigate next/prev word
                    encoder_action_navword(clockwise);
                } else if (mods_state & MOD_BIT(KC_RALT)) {  // if holding Right Alt, change media next/prev track
                    encoder_action_mediatrack(clockwise);
                } else  {
                    encoder_action_volume(clockwise);   // Otherwise it just changes volume
                }
                break;
            }
            break;
        }
        return true;
    }
#endif


#ifdef OLED_ENABLE
	static void render_logo(void) {     // Render MechWild "MW" Logo
		static const char PROGMEM logo_1[] = {0x8A, 0x8B, 0x8C, 0x8D, 0x00};
		static const char PROGMEM logo_2[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0x00};
		static const char PROGMEM logo_3[] = {0xCA, 0xCB, 0xCC, 0xCD, 0x00};
		static const char PROGMEM logo_4[] = {0x20, 0x8E, 0x8F, 0x90, 0x00};
		oled_set_cursor(0,0);
		oled_write_P(logo_1, false);
		oled_set_cursor(0,1);
		oled_write_P(logo_2, false);
		oled_set_cursor(0,2);
		oled_write_P(logo_3, false);
		oled_set_cursor(0,3);
		oled_write_P(logo_4, false);
	}

    #ifdef LANDSCAPE_MODE
    void oled_task_user(void) {

        render_logo();
        oled_set_cursor(8,2);
        switch(get_selected_layer()){
            case _BASE:
                oled_write_P(PSTR("BASE"), false);
                break;
            case _FN1:
                oled_write_P(PSTR("FN "), false);
                oled_write(selectedkey_rec.keydesc, false);
                break;
            case _FN2:
                oled_write_P(PSTR("FN2 "), false);
                break;
            case _FN3:
                oled_write_P(PSTR("FN3 "), false);
                break;
            case _FN4:
                oled_write_P(PSTR("FN4 "), false);
                break;
            case _RGB:
                oled_write_P(PSTR("RGB "), false);
                break;
            default:
                oled_write_P(PSTR(" ?? "), false);    // Should never display, here as a catchall
        }
        oled_write_P(keymap_config.no_gui ? PSTR(" WL") : PSTR("   "), false);
        oled_set_cursor(8,3);
        if (get_highest_layer(layer_state) == get_selected_layer()) {
            oled_write_P(PSTR("             "), false);
        } else {
            switch (get_highest_layer(layer_state)) {
                case _BASE:
                    oled_write_P(PSTR("Temp BASE"), false);
                    break;
                case _FN1:
                    oled_write_P(PSTR("Temp FN "), false);
                    oled_write(selectedkey_rec.keydesc, false);
                    break;
                case _FN2:
                    oled_write_P(PSTR("Temp FN2 "), false);
                    break;
                case _FN3:
                    oled_write_P(PSTR("Temp FN3 "), false);
                    break;
                case _FN4:
                    oled_write_P(PSTR("Temp FN4 "), false);
                    break;
                case _RGB:
                    oled_write_P(PSTR("Temp RGB "), false);
                    break;
                default:
                    oled_write_P(PSTR("Temp ????"), false);    // Should never display, here as a catchall
            }
        }
        led_t led_state = host_keyboard_led_state();
        oled_set_cursor(8,0);
        oled_write_P(PSTR(" JONAVIN "), false);
        oled_set_cursor(8,1);
        oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
        oled_write_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("     "), false);
        oled_write_P(led_state.scroll_lock ? PSTR("SCR") : PSTR("   "), false);
    }
    #endif // LANDSCAPE_MODE

    // regular mode
    #ifndef LANDSCAPE_MODE
 	oled_rotation_t oled_init_user(oled_rotation_t rotation) {
		return OLED_ROTATION_270;       // flips the display 270 degrees
	}

    void oled_task_user(void) {
		render_logo();
		oled_set_cursor(0,6);

		oled_write_ln_P(PSTR("Layer"), false);

        switch (get_highest_layer(layer_state)) {
            case _BASE:
                oled_write_ln_P(PSTR("Base"), false);
                break;
            case _FN1:
                oled_write_ln_P(PSTR("FN 1"), false);
                break;
            case _FN2:
                oled_write_ln_P(PSTR("FN 2"), false);
                break;
            case _FN3:
                oled_write_ln_P(PSTR("FN 3"), false);
                break;
            case _FN4:
                oled_write_ln_P(PSTR("FN 4"), false);
                break;
            case _RGB:
                oled_write_ln_P(PSTR("RGB "), false);
                break;
            default:
                oled_write_ln_P(PSTR("Undef"), false);
        }
        oled_write_ln_P(PSTR(""), false);
        // Host Keyboard LED Status
        led_t led_state = host_keyboard_led_state();
        oled_write_ln_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
        oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
        oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    }
    #endif // !LANDSCAPE_MODE

    void suspend_power_down_user(void) {  // shutdown oled when powered down to prevent OLED from showing Mercutio all the time
      oled_off();
    }
#endif
