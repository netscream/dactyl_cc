# MCU name
MCU = at90usb1286

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = halfkay

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration
MOUSEKEY_ENABLE  = yes        # Mouse keys
EXTRAKEY_ENABLE  = yes        # Audio control and System control
CONSOLE_ENABLE   = yes        # Console for debug
TERMINAL_ENABLE  = no
COMMAND_ENABLE   = yes
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no        # Breathing sleep LED during USB suspend
NKRO_ENABLE      = no        # USB Nkey Rollover
BACKLIGHT_ENABLE = no        # Enable keyboard backlight functionality on B7 by default
MIDI_ENABLE      = no        # MIDI controls
UNICODE_ENABLE   = no        # Unicode
BLUETOOTH_ENABLE = no        # Enable Bluetooth with the Adafruit EZ-Key HID
AUDIO_ENABLE     = no        # Audio output should be port E6, current quantum library hardcodes C6, which we use for programming
#SPLIT_KEYBOARD	 = yes		 # Enables split keyboard in software
#SPLIT_TRANSPORT  = custom    # turn of avr specific split implementation

DEFAULT_FOLDER = dactyl_cc/teensy40
