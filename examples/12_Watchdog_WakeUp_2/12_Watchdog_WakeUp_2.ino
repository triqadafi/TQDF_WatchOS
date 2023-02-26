#include <Arduino.h>
/*  
 *  EXAMPLE: Standby LED #2 
 *           Using WATCHDOG wake up interrupt to create a standby LED every 5 second
 *           Random double LED blink.
 *           Save the random value usin register to make sure the turned on LED is not repeated on the next wake up.
 *
 * API:
 * WatchOS.WATCHDOG_wake
 * 
 * WatchOS.LED_write
 * 
 * WatchOS.WATCHDOG_isWakeUp
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



  /* END USER SETUP CODE 1*/
}

/* USER APP CODE 1*/
void loop() {
  if (WatchOS.WATCHDOG_isInterruptFlag()){
    // generate a beautiful random value. just 0 to 5
    int val = WatchOS.REGISTER_read(1, WatchOS.SOURCE); // read from the register
    int val_old = val;
    while (abs(val-val_old) <= 1) val+=analogRead(PADC_TEMP); val = val%6; // just make sure it's not the same number and not close to previous number
    WatchOS.REGISTER_write(1, val); // save for later wake up cycle
    
    // display the LED
    WatchOS.LED_write((val+0)%12, LED_ON);
    WatchOS.LED_write((val+6)%12, LED_ON);
    delay(50);

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