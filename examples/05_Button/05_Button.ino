#include <Arduino.h>
/*  
 * EXAMPLE: Using the Button
 *
 * API
 * WatchOS.BUTTON_isPressed
 * WatchOS.BUTTON_isSafePressed
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
  // WatchOS.LED_write(NUMBER_F(12), LED_ON); // just tell the whatchdog that we are aware

  if(WatchOS.BUTTON_isPressed()){
  // if(WatchOS.BUTTON_isSafePressed(50)){ // this function is using debaoucer with time window of 50 ms
    Serial.println("Button is pressed!");
    WatchOS.LED_write(NUMBER_F(3), LED_ON);
    WatchOS.LED_write(NUMBER_F(9), LED_ON);
    delay(200);
  }else{
    Serial.println("Press the button!");
    WatchOS.LED_write(NUMBER_F(3), LED_OFF);
    WatchOS.LED_write(NUMBER_F(9), LED_OFF);
    delay(200);
  }
}
/* END USER APP CODE 1*/