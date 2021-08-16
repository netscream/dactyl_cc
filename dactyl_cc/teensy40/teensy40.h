#pragma once

#include "quantum.h"

#define ___ KC_NO

#define mcp23017_i2c_addr 0x20

#define IODIRA          0x00            // i/o direction register
#define IODIRB          0x01
#define GPPUA           0x0C            // GPIO pull-up resistor register
#define GPPUB           0x0D
#define GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define GPIOB           0x13
#define OLATA           0x14            // output latch register
#define OLATB           0x15


extern msg_t expander_status;
extern uint8_t expander_input_pin_mask;
extern bool i2c_initialized;

void init_expander(void);

// This a shortcut to help you visually see your layout.
// The first section contains all of the arguments as on the physical keyboard
// The second converts the arguments into the 2-D scanned array

#define LAYOUT(                                   \
    k00, k01, k02, k03, k04, k05,                 \
    k10, k11, k12, k13, k14, k15,                 \
    k20, k21, k22, k23, k24, k25,                 \
    k30, k31, k32, k33, k34, k35,                 \
         k41, k42, k43, k44,                      \
                             k55, k54,            \
                                  k53,            \
                        k50, k51, k52,            \
                                                  \
                   k06, k07, k08, k09, k0A, k0B,  \
                   k16, k17, k18, k19, k1A, k1B,  \
                   k26, k27, k28, k29, k2A, k2B,  \
                   k36, k37, k38, k39, k3A, k3B,  \
                        k47, k48, k49, k4A,       \
              k57, k56,                           \
              k58,                                \
              k59, k5A, k5B                       \
)                                                 \
{                                                 \
    { k00,  k01,  k02,  k03,  k04,  k05, k06,  k07,  k08,  k09,  k0A,  k0B }, \
    { k10,  k11,  k12,  k13,  k14,  k15, k16,  k17,  k18,  k19,  k1A,  k1B }, \
    { k20,  k21,  k22,  k23,  k24,  k25, k26,  k27,  k28,  k29,  k2A,  k2B }, \
    { k30,  k31,  k32,  k33,  k34,  k35, k36,  k37,  k38,  k39,  k3A,  k3B }, \
    { ___,  k41,  k42,  k43,  k44,  ___, ___,  k47,  k48,  k49,  k4A,  ___ }, \
    { k50,  k51,  k52,  k53,  k54,  k55, k56,  k57,  k58,  k59,  k5A,  k5B }, \
}


/*  ---------------- LEFT HAND -----------------   ---------------- RIGHT HAND ---------------- */
#define LAYOUT_pretty(                                                                           \
    k00, k01, k02, k03, k04, k05,                            k06, k07, k08, k09, k0A, k0B,       \
    k10, k11, k12, k13, k14, k15,                            k16, k17, k18, k19, k1A, k1B,       \
    k20, k21, k22, k23, k24, k25,                            k26, k27, k28, k29, k2A, k2B,       \
    k30, k31, k32, k33, k34, k35,                            k36, k37, k38, k39, k3A, k3B,       \
         k41, k42, k43, k44,                                      k47, k48, k49, k4A,            \
                             k55, k54,                  k57, k56,                                \
                                  k53,                  k58,                                     \
                        k50, k51, k52,                  k59, k5A, k5B                            \
)                                                                                                \
{                                                                                                \
        { k00,  k01,  k02,  k03,  k04,  k05, k06,  k07,  k08,  k09,  k0A,  k0B }, \
        { k10,  k11,  k12,  k13,  k14,  k15, k16,  k17,  k18,  k19,  k1A,  k1B }, \
        { k20,  k21,  k22,  k23,  k24,  k25, k26,  k27,  k28,  k29,  k2A,  k2B }, \
        { k30,  k31,  k32,  k33,  k34,  k35, k36,  k37,  k38,  k39,  k3A,  k3B }, \
        { ___,  k41,  k42,  k43,  k44,  ___, ___,  k47,  k48,  k49,  k4A,  ___ }, \
        { k50,  k51,  k52,  k53,  k54,  k55, k56,  k57,  k58,  k59,  k5A,  k5B }, \
}
