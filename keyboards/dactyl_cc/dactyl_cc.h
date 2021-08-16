#ifndef DACTYL_CC_H
#define DACTYL_CC_H

#ifdef KEYBOARD_dactyl_cc_teensy40
	#include "teensy40.h"
#endif

#include "quantum.h"


void all_led_off(void);
void all_led_on(void);
void num_lock_led_on(void);
void caps_lock_led_on(void);
void scroll_lock_led_on(void);
void keypad_led_on(void);



#endif
