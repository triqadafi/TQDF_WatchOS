#include <Arduino.h>
/*  
 * EXAMPLE: Using the LED
 *
 * API
 * NUMBER_F
 * WatchOS.LED_write
 * WatchOS.LED_toggle
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
    /* RTC Callibration: */ 0x6EF, // From manufacturer (back of the PCB)
    /* WATCHDOG Routine  */ WatchOS.WAKE_DISABLED,
    /* GPIO -------------------------------------------------------------------*/ 
    /* Clock pin (1-12): */ led_clock_pins, 
    /* LED Power pin:    */ led_power_pin, 
    /* Button pin:       */ button_pin, 
    /* Unused pins:      */ unused_pins
  );
  /* USER SETUP CODE 1*/
  // put your setup code here, to run once:



  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  for (int i = 0; i < 12; i++){
    WatchOS.LED_write(i, LED_ON);
    delay(100);
    WatchOS.LED_write(i, LED_OFF);
    delay(100);
  }
}
/* END USER APP CODE 1*/