#include <Arduino.h>
//-------------------------------------------------------------------

// Debug Flag
#ifndef DEBUG_F_ENABLE
#define DEBUG_F_ENABLE false
#endif
#define DEBUG_F if(DEBUG_F_ENABLE) 


// CONFIG: Hardware Related
#define PIN_BUTTON PA0
#define PIN_LED_POWER PB7
#define PIN_UNUSED_F PA1

// ------------------------------------------------------------------
// PIN Configuration
// ------------------------------------------------------------------
int button_pin = PIN_BUTTON;
int led_power_pin = PIN_LED_POWER;
int led_clock_pins[12] = {
  PB9, // #1
  PB6, // #2
  PB5, // #3
  PB4, // #4
  PB3, // #5
  PB12, // #6
  PB10, // #7
  PA7, // #8
  PA6, // #9
  PA5, // #10
  PA4, // #11
  PC13 // #12
};
int unused_pins[12] = {
  PA1, // #9
  PA2, // #5
  PA3, // #7
  PA8, // #8
  PA15, // #10
  PB0, // #12
  PB1, // #11
  PB8, // #2
  PB8, // #12 JUST A PLACEHOLDER besause PB11 missing
  PB13, // #4
  PB14, // #4
  PB15 // #6
};
int special_pins[6] = {
  PIN_BUTTON, // Button
  PIN_LED_POWER, // LED SWITCH !! Ssame as PIN_LED_POWER
  PA9, // SERIAL TX
  PA10, // SERIAL RX
  PA11, // USB D+
  PA12 // USB D+
};


// ------------------------------------------------------------------
// MISCELLANEOUS
// ------------------------------------------------------------------
// #define USE_STM32
// #define USE_STM32F401CC

// #define GPIO_LED_INVERT 1 // pin as sink
// #define GPIO_LED_POWER_INVERT 1  // Pchannel
// #define GPIO_BUTTON_INVERT 1  // High-side


// LED CONFIG
// leave this, just edit CONFIG_LED_INVERT
#define LED_ON WatchOS.LED_ON_logic
#define LED_OFF WatchOS.LED_OFF_logic

#define LED_POWER_ON WatchOS.LED_POWER_ON_logic
#define LED_POWER_OFF WatchOS.LED_POWER_OFF_logic

// BUTTON CONFIG
#define BUTTON_PRESSED WatchOS.BUTTON_PRESSED_logic
#define BUTTON_NORMAL WatchOS.BUTTON_NORMAL_logic