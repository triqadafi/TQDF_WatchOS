#include <Arduino.h>
/*  
 *  EXAMPLE: Using RTC, get the time every second
 *
 *  API
 * WatchOS.LED_toggle
 * 
 * WatchOS.RTC_update
 * WatchOS.RTC_getHour
 * WatchOS.RTC_getMinute
 * WatchOS.RTC_getSecond
 * 
 */
/* USER DEFINE */

#define DEBUG_F_ENABLE false



/* END USER DEFINE*/

// #define APP_COMPATIBILITY // ENABLE ONLY IF YOU HAVE BETA TEST BOARD
#include "TQDF_WatchOS.h"
#include "config_AEONF_mk6.h"

/* USER VARIABLE and PROTOTYPES */




/* END USER VARIABLE and PROTOTYPES*/

void setup() {
  WatchOS.config(
    /* Serial Baudrate:  */ 115200, // 9600 or 115200
    /* RTC Preserved     */ false, 
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

  // We change RTC Preserved: false; to discard running RTC date and time 
  WatchOS.RTC_initialTime(11, 11, 11);
  WatchOS.RTC_initialDate(17, 8, 23);

  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  WatchOS.LED_toggle(NUMBER_F(12)); // just tell the whatchdog that we are aware
  
  WatchOS.RTC_update();
  int hourvalue = WatchOS.RTC_getHour();
  int minutevalue = WatchOS.RTC_getMinute();
  int secondvalue = WatchOS.RTC_getSecond();

  if (hourvalue<10) Serial.print('0');
  Serial.print(hourvalue);
  Serial.print(":");
  if (minutevalue<10) Serial.print('0');
  Serial.print(minutevalue);
  Serial.print(":");
  if (minutevalue<10) Serial.print('0');
  Serial.print(secondvalue);
  Serial.println();

  delay(1000);
}
/* END USER APP CODE 1*/