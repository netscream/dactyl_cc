#include QMK_KEYBOARD_H
#include "print.h"
#include "hal.h"


enum netscream_kb_layers
{
    layer_base,
    layer_symbols,
    layer_media,
    layer_custom,
};
/****************************************************************************************************
*
* Keymap: Base Layer in Qwerty
*
* ,-------------------------------------------------------------------------------------------------------------------.
* | MOD(=+)|  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | Tab    |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  | \|     |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | ESC    |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  | '"     |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Shift  |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  ,.  |  .>  |  /?  | Shift  |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          | `~   | INS  | Left | Right|                                         | Up   | Down |  [{  |  ]}  |
*          `---------------------------'                                         `---------------------------'
*                                        ,-------------.         ,-------------.
*                                        | Ctrl | Alt  |         | Gui  | Ctrl |
*                                 ,------|------|------|         |------+------+------.
*                                 |      |      | Home |         | PgUp |      |      |
*                                 |Space | Del  |------|         |------|Return| Bcksp|
*                                 |      |      | End  |         | PgDn |      |      |
*                                 `--------------------'         `--------------------'
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[layer_base] = LAYOUT(
           LT(layer_custom, KC_EQL), KC_1   ,KC_2  ,KC_3   ,KC_4   ,KC_5   ,
           KC_TAB, KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,
           KC_ESC, KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,
           KC_LSFT,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,
                   KC_GRV ,KC_INS ,KC_LEFT,KC_RGHT,
			                        KC_LALT,KC_LCTL,
                                            KC_HOME,
                           KC_SPC,  KC_DEL ,KC_END ,
	KC_6   ,KC_7   ,KC_8   ,KC_9   ,KC_0   ,KC_MINS,
	KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,KC_BSLS,
	KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,KC_QUOT,
	KC_N   ,KC_M   ,KC_COMM,KC_DOT ,KC_SLSH,KC_RSFT,
		KC_UP  ,KC_DOWN,KC_LBRC,KC_RBRC,
           KC_RGUI,KC_RCTL,
           KC_PGUP,
           KC_PGDN,KC_ENTER ,KC_BSPC
    ),
/****************************************************************************************************
*
* Keymap: Symbol Layer
*
* ,-------------------------------------------------------------------------------------------------------------------.
* | MOD()  |  F1  |  F2  |  F3  |  F4  |  F5  |                           |  F6  |  F7  |  F8  |  F9  |  F10 | F11    |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* |        |   !  |   @  |   {  |   }  |   |  |                           |   UP |   7  |   8  |   9  |   *  | F12    |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | ESC    |   #  |   $  |   (  |   )  |   `  |                           |   DN |   4  |   5  |   6  |   +  |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |   %  |   X  |   C  |   V  |   ~  |                           |   &  |   1  |   2  |   3  |   /  |        |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |      |      |      |      |                                         |      |   .  |   0  |  =   |
*          `---------------------------'                                         `---------------------------'
*                                        ,-------------.         ,-------------.
*                                        | ALT  | Ctrl |         |      |      |
*                                 ,------|------|------|         |------+------+------.
*                                 |      |      |      |         |      |      |      |
*                                 |      |      |------|         |------|      |      |
*                                 |      |      |      |         |      |      |      |
*                                 `--------------------'         `--------------------'
*/
[layer_symbols] = LAYOUT(
            LT(layer_custom, KC_TRNS), KC_F1  , KC_F2   , KC_F3  , KC_F4  , KC_F5  ,
            KC_TRNS, KC_EXLM, KC_AT  , KC_LCBR, KC_RCBR, KC_PIPE,
            KC_ESC,  KC_HASH, KC_DLR , KC_LPRN, KC_RPRN, KC_GRV ,
            KC_TRNS, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,
                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                       KC_LALT, KC_LCTL,
                                                KC_TRNS,
                             KC_TRNS, KC_TRNS, KC_TRNS,
            KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,  KC_F11,
            KC_UP,  KC_7,  KC_8,  KC_9, KC_ASTR, KC_F12,
            KC_DOWN,KC_4,  KC_5,  KC_6, KC_PLUS, KC_TRNS,
            KC_AMPR,KC_1,  KC_2,  KC_3, KC_BSLS, KC_TRNS,
                    KC_TRNS,KC_DOT,KC_0,KC_EQL,
                KC_TRNS, KC_TRNS,
                KC_TRNS,
                KC_TRNS, KC_TRNS,KC_TRNS
),
/****************************************************************************************************
*
* Keymap: Media Layer
*
* ,-----------------------------------------------------------------------------------------------------------------------.
* | MOD()  |      |      |      |      |      |                           |      |      |      |      |      |            |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+------------|
* |        |      |      |MS_UP |      |      |                           |      |      |      |      |      |            |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+------------|
* |        |      |MS_LFT|MS_DN |MS_RGT|      |                           |      |      |      |      |      | Play/Pause |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+------------|
* |        |      |      |      |MS_BT1|MS_BT2|                           |      |      | Prev | Next |      |            |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+------------'
*          |      |      |      |      |                                         | VolU | VolD | Mute |      |
*          `---------------------------'                                         `---------------------------'
*                                        ,-------------.         ,-------------.
*                                        |      |      |         |      |      |
*                                 ,------|------|------|         |------+------+------.
*                                 |      |      |      |         |      |      |      |
*                                 |      |      |------|         |------|      | Back |
*                                 |      |      |      |         |      |      |      |
*                                 `--------------------'         `--------------------'
*/
[layer_media] = LAYOUT(
        LT(layer_custom, KC_TRNS), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                                  KC_TRNS, KC_TRNS,
                                           KC_TRNS,
                         KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY,
        KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                 KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS,
                 KC_TRNS, KC_TRNS,
                 KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_WBAK
),
/****************************************************************************************************
*
* Keymap: Media Layer
*
* ,-------------------------------------------------------------------------------------------------------------------.
* |        | TO(1)| TO(2)| TO(3)|      |      |                           |      |      |      |      |      |        |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* |        |      |      |      |      |      |                           |      |      |      |      |      |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |      |      |      |      |      |                           |      |      |      |      |      |        |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* |        |      |      |      |      |      |                           |      |      |      |      |      |        |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          |      |      |      |      |                                         |      |      |      |      |
*          `---------------------------'                                         `---------------------------'
*                                        ,-------------.         ,-------------.
*                                        |      |      |         |      |      |
*                                 ,------|------|------|         |------+------+------.
*                                 |      |      |      |         |      |      |      |
*                                 |      |      |------|         |------|      |      |
*                                 |      |      |      |         |      |      |      |
*                                 `--------------------'         `--------------------'
*/
[layer_custom] = LAYOUT(
        KC_TRNS, TO(layer_base), TO(layer_symbols), TO(layer_media), KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        RESET, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                DEBUG, KC_TRNS, KC_TRNS, KC_TRNS,
                                  KC_TRNS, KC_TRNS,
                                           KC_TRNS,
                         KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS,
                 KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_TRNS
)
};

void keyboard_post_init_user(void)
{

}

void matrix_init_user(void)
{

}

void matrix_scan_user(void)
{

}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void led_set_user(uint8_t usb_led) {

}



