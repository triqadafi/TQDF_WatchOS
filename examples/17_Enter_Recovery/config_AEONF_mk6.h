#include <Arduino.h>
#ifndef APP_COMPATIBILITY
// ------------------------------------------------------------------
// PIN Configuration
// ------------------------------------------------------------------
int button_pin = PBUTTON;
int beeper_pin = PBEEPER;
int led_power_pin = PLED_POWER;

int led_clock_pins[12] = {
  PLED0,
  PLED1,
  PLED2,
  PLED3,
  PLED4,
  PLED5,
  PLED6,
  PLED7,
  PLED8,
  PLED9,
  PLED10,
  PLED11
};
int unused_pins[12] = {
  PUNUSED1,
  PUNUSED2,
  PUNUSED3,
  PUNUSED4,
  PUNUSED5,
  PUNUSED6,
  PUNUSED7,
  PUNUSED8,
  PUNUSED9,
  PUNUSED10,
  PUNUSED11,
  PUNUSED12
};
int special_pins[6] = {
  PSPECIAL1, // Button
  PSPECIAL2, // LED SWITCH !! Ssame as PIN_LED_POWER
  PSPECIAL3, // SERIAL TX
  PSPECIAL4, // SERIAL RX
  PSPECIAL5, // USB D-
  PSPECIAL6 // USB D+
};

#endif