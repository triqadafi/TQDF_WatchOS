#include <Arduino.h>

/*  
 *  EXAMPLE: Standby LED #1 
 *           Using WATCHDOG wake up interrupt to create a standby LED every 5 second
 *
 * API:
 * WatchOS.WATCHDOG_wake
 * 
 * WatchOS.LED_write
 * 
 * WatchOS.WATCHDOG_isWakeUp
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
    /* RTC Callibration: */ 0x6EF, // From manufacturer (back of the PCB)
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
  // Allow to reconfigure trough lato-lato menu.
  WatchOS.WATCHDOG_reconfigureWakeUp(true);

  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  if (WatchOS.WATCHDOG_isInterruptFlag()){
    WatchOS.LED_write(NUMBER_F(3), LED_ON);
    WatchOS.LED_write(NUMBER_F(9), LED_ON);
    delay(25);
    WatchOS.LED_write(NUMBER_F(3), LED_OFF);
    WatchOS.LED_write(NUMBER_F(9), LED_OFF);
    delay(25);
    WatchOS.shutdown();
  }

  WatchOS.LED_write(NUMBER_F(3), LED_ON);
  WatchOS.LED_write(NUMBER_F(9), LED_ON);
  delay(25);
  WatchOS.LED_write(NUMBER_F(3), LED_OFF);
  WatchOS.LED_write(NUMBER_F(9), LED_OFF);
  delay(25);

  if(WatchOS.BUTTON_isSafePressed(50)){
    WatchOS.shutdown();
  }
}
/* END USER APP CODE 1*/