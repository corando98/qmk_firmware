#include QMK_KEYBOARD_H

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, LGUI(KC_BSPC), KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, LCA(KC_NO), LT(2,KC_CAPS), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, LCAG(KC_RGHT), KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, LCAG(KC_LEFT), KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RGUI, MO(1), KC_LEFT, KC_DOWN, KC_RGHT),
	[1] = LAYOUT_65_ansi_blocker(KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_MUTE, KC_TRNS, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, KC_TRNS, KC_TRNS, KC_TRNS, DBG_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_END, KC_TRNS, RGB_RMOD, RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, KC_TRNS, VLK_TOG, DBG_KBD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_TRNS, RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, MD_BOOT, NK_TOGG, DBG_MTRX, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_VOLD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END),
	[2] = LAYOUT_65_ansi_blocker(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______
    ),
    */
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))
#define IDLE_TIMER_DURATION 20000 //how many milliseconds before RGB turns off

//static uint32_t idle_timer; //custom timer to check if keyboard is idled.
bool rgbkeyIdle = false; //flag for keyboard idling, nil keys for set
bool Jelocikey_toggle = true;
static uint16_t falloff_timer; //Custom timer to check if velocikey can decrease speed


int16_t currentWPM = 1;
bool consecutive_key = false;  //Flag to know if velocikey should start decreasing after a key has not been pressed for Xms
bool    KeyDown         = false;
bool    process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;
    // idle_timer = timer_read();
    falloff_timer = timer_read();

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                        rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    case LED_FLAG_UNDERGLOW: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_disable_noeeprom();
                    } break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        rgb_matrix_enable_noeeprom();
                    } break;
                }
            }
            return false;
        case VLK_TOG:
            if (record->event.pressed) {
                Jelocikey_toggle =! Jelocikey_toggle;
                rgb_matrix_set_speed_noeeprom(0);
                currentWPM = 0;
            } else {
                rgb_matrix_set_speed_noeeprom(127);
            }
            return false;
        default:

            if (rgbkeyIdle) {       //check if the keyboards already idle and if it is, turn it back on as key is pressed.
                rgbkeyIdle = false;
                rgb_matrix_set_suspend_state(false);
                rgb_matrix_enable_noeeprom();
            }

            if (Jelocikey_toggle) {
                if(KeyDown){
                    printf("KeyDown -> False\n");

                    KeyDown = false;
                } else { //Only increase speed on keydown status
                    printf("KeyDown -> True\n");
                    KeyDown = true;

                    //Different implementations of WPM, using a linear equation
                    //currentWPM = currentWPM + 3;
                    //Using QMK default WPM estimator
                    currentWPM = 2 * get_current_wpm();

                    printf("Current QMK WPM -> %d\n", get_current_wpm());
                    printf("Increasing speed to %d\n", currentWPM);
                    //currentWPM = currentWPM + 2 * currentWPM;
                    if (currentWPM > 255) { currentWPM = 255; }
                    printf("Speed -> %d\n", currentWPM);

                    rgb_matrix_set_speed_noeeprom(currentWPM);
                    consecutive_key = true;
                }

            }



            return true; //Process all other keycodes normallysadasdsds
    }
}

bool deccelerating;
void matrix_scan_user(void) {
    // custom idle rbg switch off function
    // idle_sleep();
    // if (timer_elapsed(idle_timer) > IDLE_TIMER_DURATION) {
    //     idle_timer = 0;
    //     timer_clear();
    //     rgbkeyIdle = true;
    //     rgb_matrix_set_suspend_state(true);
    //     rgb_matrix_disable_noeeprom();
    // }

    if(Jelocikey_toggle && currentWPM > 1){
        if(consecutive_key){

            //2 seconds timer to press another key otherwise slowing down
                if (timer_elapsed(falloff_timer) > 2000) {  // i.e Two keys are not struck within 2 seconds; not consecutive
                    falloff_timer      = 0;
                    timer_clear();
                    printf("Consecutive keys\n");
                    consecutive_key = false;
                    deccelerating    = true;
                }


        } else if(deccelerating) {

                if (timer_elapsed(falloff_timer) > (1020 * 255 / currentWPM) && deccelerating) {  // i.e Two keys are not struck within 1 seconds; not consecutive
                    falloff_timer      = 0;
                    timer_clear();

                    currentWPM -= 40;
                    if (currentWPM < 0) {
                        currentWPM = 0;
                        deccelerating = false;
                    }
                    printf("Decreasing speed -> %d\n", currentWPM);
                    printf("Time to decrease -> %d\n", (1000 * 255 / currentWPM));

                    rgb_matrix_set_speed_noeeprom(currentWPM);
                }

        }
    }





}

layer_state_t layer_state_setuser(layer_state_t state) {
    switch (get_highest_layer(state))
    {
    case 1: //Main layer
        printf("On main layer");
        break;
    case 2: //Keyboard functions layer
        printf("On keyboard functions layer");
        break;
    case 3: //Macro layer
        printf("On macro layer");
        break;
    default:
        printf("On switch default layer");
        break;
    }
    return state;
}

void rgb_matrix_indicators_user(void) {
    switch(get_highest_layer(layer_state)) {
        case 0:
            if(rgb_matrix_get_flags() == LED_FLAG_UNDERGLOW) {

                for (int i = 1; i < 14; i++) rgb_matrix_set_color(i, 75, 255, 75); //Num row + (-) + (=) + BCKS
                for (int i = 15; i < 29; i++) rgb_matrix_set_color(i, 75, 255, 75); //QWERT row
                for (int i = 30; i < 43; i++) rgb_matrix_set_color(i, 75, 255, 75);
                for (int i = 44; i < 57; i++) rgb_matrix_set_color(i, 75, 255, 75);
                for (int i = 58; i < 61; i++) rgb_matrix_set_color(i, 75, 255, 75);
                rgb_matrix_set_color(60, 255, 0, 0); //LGUI
                rgb_matrix_set_color(62, 255, 0, 0); //RGUI
                rgb_matrix_set_color(0, 255, 0, 0); //ESC
                for (int i = 63; i < 67; i++) rgb_matrix_set_color(i, 75, 255, 75);
                //MACRO KEYS
                rgb_matrix_set_color(14, 255, 0, 0);
                rgb_matrix_set_color(29, 255, 0, 0);
                rgb_matrix_set_color(43, 255, 0, 0);
                rgb_matrix_set_color(57, 255, 0, 0);
            }
            break;
        case 1:
            for (int i = 0; i < 67; i++) rgb_matrix_set_color(i, 75, 255, 75);

            rgb_matrix_set_color(63, 255, 0, 0);
            for (int i = 0; i < 13; i++) {  // Function row
                rgb_matrix_set_color(i, 255, 128, 0);
            }
            for (int i = 16; i < 21; i++){
            rgb_matrix_set_color(i, 255, 128, 0);
            }
            for (int i = 31; i < 40; i++){
            rgb_matrix_set_color(i, 255, 128, 0);
            }
            rgb_matrix_set_color(45, 255, 128, 0);
            for (int i = 49; i < 52; i++){
            rgb_matrix_set_color(i, 255, 128, 0);
            }


            rgb_matrix_set_color(24, 255, 128, 0);

            break;
        case 2:
            for (int i = 0; i < 67; i++) rgb_matrix_set_color(i, 75, 255, 75);
            rgb_matrix_set_color(30, 255, 0, 0);
            rgb_matrix_set_color(17, 255, 128, 0);
            for (int i = 31; i < 34; i++) rgb_matrix_set_color(i, 255, 128, 0);
            for (int i = 44; i < 47; i++) rgb_matrix_set_color(i, 255, 128, 0);
            for (int i = 58; i < 61; i++) rgb_matrix_set_color(i, 255, 128, 0);

            break;
        default:
                break;
    }
}

void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
}

