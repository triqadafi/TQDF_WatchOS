#include <Arduino.h>
/*  
 *  EXAMPLE: Using Shutdown
 *
 *  Shutdown means to save the power
 *  Just press the button to wake up and press again to shutdown
 *
 * API:
 * WatchOS.LED_write
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