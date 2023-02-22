#include <Arduino.h>
/*  
 * ADVANCED: Register (capable to save the value even the device is shutdown)
 *    Register length: 32 bit
 *    Register index: 0 to 3
 *    NOTE: only works on shutdown AND lose the value when power off
 *
 *
 * Every button press will shutdown the device. Keep the data to the register
 *
 * This program will increment the number every time it wakes up
 * save the value to register, then use it later after wake up
 * 
 * API
 * WatchOS.LED_write
 * 
 * WatchOS.REGISTER_read
 * WatchOS.REGISTER_write
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
  int val = WatchOS.REGISTER_read(0, WatchOS.SOURCE);
  val++; if(val > 11) val = 0;
  WatchOS.REGISTER_write(0, val);
  WatchOS.LED_write(val, LED_ON);

  while(1){
    if(WatchOS.BUTTON_isSafePressed(50)){
      delay(200);
      WatchOS.shutdown();
    }
  }
}
/* END USER APP CODE 1*/
