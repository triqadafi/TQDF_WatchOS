#include <Arduino.h>
/*  
 * REAL: Displaying time using LED 
 *
 * API:
 * WATCH_display_time
 * 
 */
/* USER DEFINE */

#define DEBUG_F_ENABLE true



/* END USER DEFINE*/

// #define APP_COMPATIBILITY // ENABLE ONLY IF YOU HAVE BETA TEST BOARD
#include "TQDF_WatchOS.h"
#include "config_AEONF.h"

/* USER VARIABLE and PROTOTYPES */




/* END USER VARIABLE and PROTOTYPES*/

void setup() {
  WatchOS.config(
    /* Serial Baudrate:  */ 115200, // 9600 or 115200
    /* RTC Preserved     */ true, 
    /* RTC Callibration: */ REPLACE_WITH_YOUR_CALIBRATION_VALUE, // From manufacturer (back of the PCB) fill with 0x0 to disable calibration value
    /* WATCHDOG Routine  */ WatchOS.WAKE_DISABLED,
    /* GPIO -------------------------------------------------------------------*/ 
    /* Clock pin (1-12): */ led_clock_pins, 
    /* LED Power pin:    */ led_power_pin, 
    /* Button pin:       */ button_pin, 
    /* Beeper pin:       */ beeper_pin, 
    /* Unused pins:      */ unused_pins
  );
  /* USER SETUP CODE 1*/
  // put your setup code here, to run once:

  WatchOS.RTC_initialTime(10, 14, 30);
  WatchOS.RTC_initialDate(17, 8, 23);

  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  WATCH_display_time();

  delay(3000);
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
  WatchOS.LED_write(result_hour, LED_ON);
  delay(500);

  // show minute hand
  if(result_minute == result_hour){ // if minute sama as hour just turn off a little to see the transition
    WatchOS.LED_write(result_minute, LED_OFF);
    delay(200);
  }
  WatchOS.LED_write(result_minute, LED_ON);
  delay(2000);
  WatchOS.LED_clear();

  // show plus hand
  for (int i = 0; i < result_minute_remainder; i++){
    int _plusminute = result_minute+i+1;
    if(_plusminute > 12) _plusminute -= 12;
    
    WatchOS.LED_write(result_minute, LED_ON);
    WatchOS.LED_write(_plusminute, LED_ON);
    delay(250);
    WatchOS.LED_write(result_minute, LED_ON);
    WatchOS.LED_write(_plusminute, LED_OFF);
    delay(250);
  }
  WatchOS.LED_clear();
  delay(1000);

  // show second
  // int result_second = ((clock_second/10) * 2) + 2; // +2 means the maximum tolerenace of the clock 
  // if(result_second > 0){
  //   WatchOS.LED_write(result_second, LED_ON);
  // }else{
  //   WatchOS.LED_write(12, LED_ON);
  // }
  // delay(1000);
  // WatchOS.LED_clear();

  delay(1000);

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