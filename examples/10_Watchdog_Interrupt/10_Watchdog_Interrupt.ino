#include <Arduino.h>

/*  
 *  EXAMPLE: Using watchdog interrupt
 *
 * API:
 * WatchOS.WATCHDOG_wake
 * 
 * WatchOS.LED_toggle
 * 
 * WatchOS.WATCHDOG_isElapsedFlag
 * WatchOS.WATCHDOG_clearElapsedFlag
 * 
 * WatchOS.BUTTON_isSafePressed
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
    /* WATCHDOG Routine  */ WatchOS.WAKE_EVERY_5S,
    /* GPIO -------------------------------------------------------------------*/ 
    /* Clock pin (1-12): */ led_clock_pins, 
    /* LED Power pin:    */ led_power_pin, 
    /* Button pin:       */ button_pin, 
    /* Beeper pin:       */ beeper_pin, 
    /* Unused pins:      */ unused_pins
  );
  /* USER SETUP CODE 1*/
  // put your setup code here, to run once:

  // Make sure the wake up flag is true.
  // You comment this after the first running. The value will be save in the register.
  WatchOS.WATCHDOG_setWakeUpFlag(true);
  // Disable the reconfiguration trough lato-lato menu. So, it;s always true
  WatchOS.WATCHDOG_reconfigureWakeUp(false);

  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  if (WatchOS.WATCHDOG_isInterruptFlag()){
    WatchOS.LED_toggle(NUMBER_F(12));
    WatchOS.LED_toggle(NUMBER_F(9));
    WatchOS.LED_toggle(NUMBER_F(6));
    WatchOS.LED_toggle(NUMBER_F(3));
    WatchOS.WATCHDOG_clearInterruptFlag();
  }

  if(WatchOS.BUTTON_isSafePressed(50)){
    WatchOS.shutdown();
  }
}
/* END USER APP CODE 1*/