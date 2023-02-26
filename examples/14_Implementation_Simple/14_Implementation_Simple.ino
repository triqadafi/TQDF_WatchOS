#include <Arduino.h>

/*
 * REAL IMPLEMENTATION (SIMPLE)
 * 
 * ON SHUTDOWN
 *  it will wake up every 5S with double LED blink
 * 
 * ON WAKE (single press to wake) 
 *  Show the ime using LED then shutdown
 * 
 * API"
 * WatchOS.LED_write
 * WATCH_display_time
 * 
 * WatchOS.REGISTER_read
 * WatchOS.REGISTER_write
 * 
 * WatchOS.WATCHDOG_wake
 * WatchOS.WATCHDOG_isWakeUp
 * 
 * WatchOS.POWER_shutdown
 * 
 */
/* USER DEFINE */

#define DEBUG_F_ENABLE false



/* END USER DEFINE*/

#include "TQDF_WatchOS.h"
#include "config_AEONF_mk6.h"

/* USER VARIABLE and PROTOTYPES */




/* END USER VARIABLE and PROTOTYPES*/

void setup() {
  WatchOS.config(
    /* Serial Baudrate:  */ 115200, // 9600 or 115200
    /* RTC Preserved     */ true, 
    /* RTC Callibration: */ 0x0, // From manufacturer (back of the PCB) fill with 0x0 to disable calibration value
    /* WATCHDOG Routine  */ WatchOS.WAKE_EVERY_10S,
    /* GPIO -------------------------------------------------------------------*/ 
    /* Clock pin (1-12): */ led_clock_pins, 
    /* LED Power pin:    */ led_power_pin, 
    /* Button pin:       */ button_pin, 
    /* Beeper pin:       */ beeper_pin, 
    /* Unused pins:      */ unused_pins
  );
  /* USER SETUP CODE 1*/
  // put your setup code here, to run once:

// WatchOS.WATCHDOG_setWakeUpFlag(false);
  WatchOS.WATCHDOG_reconfigureWakeUp(true);
  WatchOS.BEEPER_play(12500, 50);

  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  if (WatchOS.WATCHDOG_isInterruptFlag()){
    // display the LED
    WatchOS.LED_setBrightnessMax(100);
    WatchOS.LED_write(NUMBER_F(3), LED_ON);
    WatchOS.LED_write(NUMBER_F(9), LED_ON);
    delay(50);
    WatchOS.LED_clear();
    WatchOS.WATCHDOG_clearInterruptFlag();
    WatchOS.shutdown();
  }

  WATCH_display_time();

  WatchOS.shutdown();
}


// ------------------------------------------------------------------
#pragma region DISPLAY_TIME
// ------------------------------------------------------------------
void WATCH_display_time(){
  WatchOS.RTC_update();
  int clock_hour = WatchOS.RTC_getHour();
  int clock_minute = WatchOS.RTC_getMinute();
  int clock_second = WatchOS.RTC_getSecond();

  // useful of you need to learn
  // int clock_hour = random(0, 11);
  // int clock_minute = random(0, 59);
  // int clock_second = random(0, 59);

  DEBUG_F Serial.print("[DEBUG] DISPLAYING RAW: ");
  DEBUG_F Serial.print(clock_hour);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_minute);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_second);
  DEBUG_F Serial.println();

  // adjust based on watch
  if(clock_hour > 12) clock_hour -= 12;
  int result_hour = clock_hour;

  int result_minute = clock_minute / 5;
  int result_minute_remainder = clock_minute % 5;

  // show hour hand
  WatchOS.LED_write(NUMBER_F(result_hour), LED_ON);
  delay(500);

  // show minute hand
  if(result_minute == result_hour){ // if minute sama as hour just turn off a little to see the transition
    WatchOS.LED_write(NUMBER_F(result_minute), LED_OFF);
    delay(200);
  }
  WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
  delay(2000);
  WatchOS.LED_clear();

  // show plus hand
  for (int i = 0; i < result_minute_remainder; i++){
    int _plusminute = result_minute+i+1;
    if(_plusminute > 12) _plusminute -= 12;
    
    WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
    WatchOS.LED_write(NUMBER_F(_plusminute), LED_ON);
    delay(250);
    WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
    WatchOS.LED_write(NUMBER_F(_plusminute), LED_OFF);
    delay(250);
  }
  WatchOS.LED_clear();
  // delay(1000);

  // show second
  // int result_second = ((clock_second/10) * 2) + 2; // +2 means the maximum tolerenace of the clock 
  // if(result_second > 0){
  //   WatchOS.LED_write(NUMBER_F(result_second), LED_ON);
  // }else{
  //   WatchOS.LED_write(NUMBER_F(12), LED_ON);
  // }
  // delay(1000);
  // WatchOS.LED_clear();
  // delay(1000);

  DEBUG_F Serial.print("[DEBUG] DISPLAYING LED: ");
  DEBUG_F Serial.print(result_hour);
  DEBUG_F Serial.print(":[");
  DEBUG_F Serial.print(result_minute);
  DEBUG_F Serial.print("*5 + ");
  DEBUG_F Serial.print(result_minute_remainder);
  DEBUG_F Serial.print("]");
  // DEBUG_F Serial.print(":");
  // DEBUG_F Serial.print(result_second);
  DEBUG_F Serial.println();
}

#pragma endregion DISPLAY_TIME
// ------------------------------------------------------------------

/* END USER APP CODE 1*/