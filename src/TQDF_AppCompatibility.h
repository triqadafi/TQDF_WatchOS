#include <Arduino.h>

// ------------------------------------------------------------------
// PIN Configuration
// ------------------------------------------------------------------
#ifdef APP_COMPATIBILITY
  // CONFIG: Hardware Related
  #define PIN_BUTTON PA0
  #define PIN_BEEPER PA10
  #define PIN_LED_POWER PB7
  #define PIN_UNUSED_F PA1
  // ------------------------------------------------------------------
  // PIN Configuration
  // ------------------------------------------------------------------
  int button_pin = PIN_BUTTON;
  int beeper_pin = PIN_BEEPER;
  int led_power_pin = PIN_LED_POWER;
  int led_clock_pins[12] = {PC13, PB9, PB6, PB5, PB4, PB3, PB12, PB10, PA7, PA6, PA5, PA4};
  int unused_pins[12] = {PA1, PA2, PA3, PA8, PA8, PB0, PB1, PB8, PB8, PB13, PB14, PB15};
  int special_pins[6] = {PIN_BUTTON, PIN_LED_POWER, PA9, PA10, PA11, PA12};
#endif