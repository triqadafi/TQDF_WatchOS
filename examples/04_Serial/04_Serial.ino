#include <Arduino.h>
/*  
 * EXAMPLE: Print to serial every second
 *
 * API
 * WatchOS.LED_write
 * 
 */
/* USER DEFINE */
/* USER DEFINE */

#define DEBUG_F_ENABLE false



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



  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  // WATCHDOG: 10 second of inactivity will shutdown the device!!!
  // uncomment the program below if you dont want catched by the WATCHDOG
  WatchOS.LED_write(0, LED_ON); // just tell the whatchdog that we are aware
  
  Serial.println("Hello from watch!");
  delay(1000);
}
/* END USER APP CODE 1*/
