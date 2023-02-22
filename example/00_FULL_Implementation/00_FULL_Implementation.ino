#include <Arduino.h>

/*
 * REAL IMPLEMENTATION (FULL)
 */
/* USER DEFINE */
// #define DISPLAY_MODE 0 // NORMAL
// #define DISPLAY_MODE 1 // DECIMAL 1
#define DISPLAY_MODE 2 // DECIMAL 2
// #define USE_SECOND

#define DEBUG_F_ENABLE true

// FSM CONFIG
#define FSM_ALIVE_MS 30000 // when entering config exit after 30s inactivity

/* END USER DEFINE*/

#include "TQDF_WatchOS.h"
#include "config_AEONF_mk6.h"

/* USER VARIABLE */
// ------------------------------------------------------------------
// Finite State Machine (FSM) Variable
// ------------------------------------------------------------------
int FSM_STATE = 0;
unsigned long FSM_ALIVE_millis = 0;

// ------------------------------------------------------------------
// CONFIGURATION Variable
// ------------------------------------------------------------------
// CONFIG Parameter
int CONFIG_state = 0;
int CONFIG_value = 0;
unsigned long CONFIG_millis = 0;

int CONFIG_MENU_index = 0;
int CONFIG_CLOCK_hour = 0;
int CONFIG_CLOCK_minute = 0;
int CONFIG_CLOCK_plusminute = 0;
int CONFIG_CLOCK_second = 0;
/* END USER VARIABLE*/

void setup() {
  WatchOS.config(
    /* Serial Baudrate:  */ 115200, // 9600 or 115200
    /* RTC Preserved     */ true, 
    /* RTC Callibration: */ 0x6EF, // From manufacturer (back of the PCB)
    /* WATCHDOG Routine  */ WatchOS.WAKE_EVERY_10S,
    /* GPIO -------------------------------------------------------------------*/ 
    /* Clock pin (1-12): */ led_clock_pins, 
    /* LED Power pin:    */ led_power_pin, 
    /* Button pin:       */ button_pin, 
    /* Unused pins:      */ unused_pins
  );
  /* USER SETUP CODE 1*/
  WatchOS.WATCHDOG_reconfigureWakeUp(true);
  WatchOS.LED_setBrightnessMax(1);

  WatchOS.RTC_initialTime(11,25,30);

  /* END USER SETUP CODE 1*/
}
/* USER APP CODE 1*/

void loop() {
  // ------------------------------------------------------------
  // STANDBY LED
  // ------------------------------------------------------------
  if (WatchOS.WATCHDOG_isInterruptFlag()){
    // generate a beautiful random value. just 0 to 5
    // int val = WatchOS.REGISTER_read(1, WatchOS.SOURCE); // read from the register
    // int val_old = val;
    // while (abs(val-val_old) <= 1) val+=analogRead(PADC_TEMP); val = val%6; // just make sure it's not the same number and not close to previous number
    // WatchOS.REGISTER_write(1, val); // save for later wake up cycle
    
    // // display the LED
    // WatchOS.LED_setPowerLimit(WatchOS.POWER_MAXIMUM);
    // WatchOS.LED_write((val+0)%12, LED_ON);
    // WatchOS.LED_write((val+6)%12, LED_ON);
    // delay(50);

      
    WatchOS.RTC_update();
    int clock_minute = WatchOS.RTC_getMinute();
    int result_minute = clock_minute / 5;
    // replace zero with 12
    if(result_minute == 0) result_minute = 12;
    WatchOS.LED_setPowerLimit(WatchOS.POWER_MAXIMUM);
    WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
    delay(50);

    WatchOS.shutdown();
  }
  // ------------------------------------------------------------
  // START THE FSM
  // ------------------------------------------------------------
  FSM_begin();

  // ------------------------------------------------------------
  // DISPLAY TIME
  // ------------------------------------------------------------
  if(FSM_STATE == 1){
    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    WATCH_display_mode(DISPLAY_MODE);

    if(WatchOS.BUTTON_isPressed()){
      LED_all_delay(LED_ON, 100);
      LED_all_delay(LED_OFF, 100);
      LED_all_delay(LED_ON, 100);

      // make sure the button released
      do{delay(200);} while(WatchOS.BUTTON_isPressed());
      FSM_STATE = 2;
    }
  }
  if(FSM_STATE == 2){
    CONFIG_MENU();
  }
  if(FSM_STATE == 11){
    CONFIG_TIME();
  }
  if(FSM_STATE == 12){
    DEBUG_FIME();
  }
  if(FSM_STATE == 13){
    WatchOS.bootloader();
  }
  if(FSM_STATE == 14){
    for(int i=0; i<5; i++){
      WatchOS.LED_write(NUMBER_F(4), LED_ON);
      delay(100);
      WatchOS.LED_write(NUMBER_F(4), LED_OFF);
      delay(100);
    }
  }
  if(FSM_STATE == 15){
    for(int i=0; i<5; i++){
      WatchOS.LED_write(NUMBER_F(5), LED_ON);
      delay(100);
      WatchOS.LED_write(NUMBER_F(5), LED_OFF);
      delay(100);
    }
  }
  WatchOS.shutdown();
}

// ------------------------------------------------------------------
#pragma region FSM // FINITE STATE MACHINE
// ------------------------------------------------------------------
void FSM_begin(){
  FSM_STATE = 1;
  FSM_ALIVE_millis = millis();
}

bool FSM_timeout(unsigned long ms){
  // unused guard, check device is alive
  if(millis() - FSM_ALIVE_millis > ms){
    FSM_STATE = 0;
    DEBUG_F Serial.println("[DEBUG] CONFIG TIMEOUT");
    return true;
  }
  return false;
}


#pragma endregion FSM
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region LED
// ------------------------------------------------------------------
void LED_all_delay(bool led_state, unsigned long delay_ms){
  for (int i = 0; i < 12; i++){
    WatchOS.LED_write(i, led_state);
  }
  delay(delay_ms);
}
#pragma endregion LED
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region CONFIG_MENU
// ------------------------------------------------------------------
void CONFIG_MENU(){
    DEBUG_F Serial.println("[DEBUG] CONFIG MENU");
    CONFIG_value = 1;
    CONFIG_MENU_index = 1;
    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    //Setup
    WatchOS.LED_clear();
    WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON);

    // loop
    CONFIG_millis = millis();
    CONFIG_state = 0;

    while (1) {
      // avoid using loop
      if(CONFIG_state == 0){
        // BUTTON
        int _result = WatchOS.BUTTON_getResult();
        if(_result > 0){
          FSM_ALIVE_millis = millis();
          WatchOS.LED_clear();
          if(_result == 1){
            // CONFIG_value++; // 0-5
            CONFIG_value = CONFIG_value % (4+1); // 0-5 only
            CONFIG_value++; // 1-6

            DEBUG_F Serial.print("[DEBUG] CONFIG_value: ");
            DEBUG_F Serial.println(CONFIG_value);

            WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON);
          }else if(_result == 2){
            CONFIG_MENU_index = CONFIG_value;

            DEBUG_F Serial.print("[DEBUG] CONFIG_MENU: ");
            DEBUG_F Serial.println(CONFIG_MENU_index);
            
            CONFIG_state = 1;

            WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON);
          }
        }
      }
      if(CONFIG_state == 1){
        for (int i = 0; i < 2; i++){
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_OFF);
          delay(100);
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON); 
          delay(100);
        }
        CONFIG_state = 2;
      }
      if(CONFIG_state == 2){
        FSM_STATE = 10+CONFIG_MENU_index;
        break;
      }
      // unused guard, check device is alive
      if(FSM_timeout(FSM_ALIVE_MS)) break;
    }
}
#pragma endregion CONFIG_MENU
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region CONFIG_TIME
// ------------------------------------------------------------------
void CONFIG_TIME(){
    FSM_ALIVE_millis = millis();

    // setup
    WatchOS.RTC_update();
    int hourvalue = WatchOS.RTC_getHour();
    if(hourvalue > 12) hourvalue -= 12;

    int minutevalue = WatchOS.RTC_getMinute();
    minutevalue = minutevalue / 5;
    
    int secondvalue = WatchOS.RTC_getSecond();
    secondvalue = secondvalue / 5;
#pragma region CONFIG_HOUR
    // ------------------------------------------------------------
    // CONFIG HOUR
    // ------------------------------------------------------------
    DEBUG_F Serial.println("[DEBUG] CONFIG HOUR HAND");
    CONFIG_value = hourvalue;
    CONFIG_CLOCK_hour = hourvalue;

    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    //Setup
    WatchOS.LED_clear();
    WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_hour), LED_ON);

    // loop
    CONFIG_millis = millis();
    CONFIG_state = 0;

    while (1) {
      // avoid using loop
      if(CONFIG_state == 0){
        // BUTTON
        int _result = WatchOS.BUTTON_getResult();
        if(_result > 0){
          FSM_ALIVE_millis = millis();
          if(_result == 1){
            // CONFIG_value++; // 0-11
            CONFIG_value = CONFIG_value % 12; // 0-11 only
            CONFIG_value++;
          }else if(_result == 2){
            CONFIG_CLOCK_hour = CONFIG_value;
            CONFIG_state = 1;
          }

          WatchOS.LED_clear();
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON);

          DEBUG_F Serial.print("[DEBUG] CONFIG_value: ");
          DEBUG_F Serial.println(CONFIG_value);
          DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_hour: ");
          DEBUG_F Serial.println(CONFIG_CLOCK_hour);
        }
      }
      if(CONFIG_state == 1){
        for (int i = 0; i < 2; i++){
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_OFF);
          delay(100);
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON); 
          delay(100);
        }

        CONFIG_state = 2;
      }
      if(CONFIG_state == 2){
        break;
      }
      // unused guard, check device is alive
      if(FSM_timeout(FSM_ALIVE_MS)) break;
    }
#pragma endregion CONFIG_HOUR

#pragma region CONFIG_MINUTE
    // ------------------------------------------------------------
    // CONFIG MINUTE HAND
    // ------------------------------------------------------------
    DEBUG_F Serial.println("[DEBUG] CONFIG MINUTE HAND");
    CONFIG_value = minutevalue;
    CONFIG_CLOCK_minute = minutevalue;

    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    // Setup
    WatchOS.LED_clear();
    WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_hour), LED_ON);
    WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON);

    // loop
    CONFIG_millis = millis();
    CONFIG_state = 0;

    while (1) {
      if(CONFIG_state == 0){
        // BUTTON
        int _result = WatchOS.BUTTON_getResult();
        if(_result > 0){
          FSM_ALIVE_millis = millis();
          if(_result == 1){
            CONFIG_value++; // 0-11
            CONFIG_value = CONFIG_value % 12; // 0-11 only
            // CONFIG_value++; // 1-12
          }else if(_result == 2){
            CONFIG_CLOCK_minute = CONFIG_value;
            CONFIG_state = 1;
          }

          WatchOS.LED_clear();
          WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_hour), LED_ON);
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON);

          DEBUG_F Serial.print("[DEBUG] CONFIG_value: ");
          DEBUG_F Serial.println(CONFIG_value);
          DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_minute: ");
          DEBUG_F Serial.println(CONFIG_CLOCK_minute);
        }
      }
      if(CONFIG_state == 1){
        for (int i = 0; i < 3; i++){
          WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_hour), LED_ON); 
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_OFF);
          delay(100);
          WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_hour), LED_ON); 
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON); 
          delay(100);
        }
        CONFIG_state = 2;
      }
      if(CONFIG_state == 2){
        break;
      }
      // unused guard, check device is alive
      if(FSM_timeout(FSM_ALIVE_MS)) break;
    }
#pragma endregion CONFIG_MINUTE

#pragma region CONFIG_MINUTE_ACCURACY
    // ------------------------------------------------------------
    // CONFIG PLUS MINUTE HAND
    // ------------------------------------------------------------
    DEBUG_F Serial.println("[DEBUG] CONFIG MINUTE ACCURACY");
    CONFIG_value = 0;
    CONFIG_CLOCK_plusminute = 0;

    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    // Setup
    WatchOS.LED_clear();
    WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON);
    delay(500);
    for (int i = 1; i < 5; i++) {
      WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON);
      WatchOS.LED_write(NUMBER_F((CONFIG_CLOCK_minute+i) % (12+1)), LED_ON);
      delay(100);
      WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON);
      WatchOS.LED_write(NUMBER_F((CONFIG_CLOCK_minute+i) % (12+1)), LED_OFF);
      delay(100);
    }

    // loop
    CONFIG_millis = millis();
    CONFIG_state = 0;

    while (1) {
      if(CONFIG_state == 0){
        // BUTTON
        int _result = WatchOS.BUTTON_getResult();
        if(_result > 0){
          FSM_ALIVE_millis = millis();
          if(_result == 1){
            CONFIG_value++; // 0-4
            CONFIG_value = CONFIG_value % (4+1); // 0-4 only
            // CONFIG_value++; // 1-5
          }else if(_result == 2){
            CONFIG_CLOCK_plusminute = CONFIG_value;
            CONFIG_state = 1;
          }
          WatchOS.LED_clear();
          // LED_write(PIXEL_COLOR_HOUR, CONFIG_CLOCK_hour-1, LED_ON);
          WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON);
          if (CONFIG_value>0)
            WatchOS.LED_write(NUMBER_F((CONFIG_CLOCK_minute+CONFIG_value) % (12+1)), LED_ON);

          DEBUG_F Serial.print("[DEBUG] CONFIG_value: ");
          DEBUG_F Serial.println(CONFIG_value);
          DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_plusminute: ");
          DEBUG_F Serial.println(CONFIG_CLOCK_plusminute);
        }
      }
      if(CONFIG_state == 1){
        for (int i = 0; i < 2; i++){
          WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON); 
          WatchOS.LED_write(NUMBER_F((CONFIG_CLOCK_minute+CONFIG_value) % (12+1)), LED_OFF);
          delay(100);
          WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_minute), LED_ON); 
          WatchOS.LED_write(NUMBER_F((CONFIG_CLOCK_minute+CONFIG_value) % (12+1)), LED_ON); 
          delay(100);
        }
        CONFIG_state = 2;
      }
      if(CONFIG_state == 2){
        break;
      }
      // unused guard, check device is alive
      if(FSM_timeout(FSM_ALIVE_MS)) break;
    }
#pragma endregion CONFIG_MINUTE

#pragma region CONFIG_SECOND
    // ------------------------------------------------------------
    // CONFIG SECOND HAND
    // ------------------------------------------------------------
    CONFIG_CLOCK_second = 6;
#ifdef USE_SECOND
    CONFIG_value = 6;
    DEBUG_F Serial.println("[DEBUG] CONFIG SECOND HAND");

    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    // Setup
    WatchOS.LED_clear();
    WatchOS.LED_write(NUMBER_F(CONFIG_CLOCK_second), LED_ON);

    // loop
    CONFIG_millis = millis();
    CONFIG_state = 0;

    while (1) {
      if(CONFIG_state == 0){
        // BUTTON
        int _result = WatchOS.BUTTON_getResult();
        if(_result > 0){
          FSM_ALIVE_millis = millis();
          if(_result == 1){
            CONFIG_value++; // 0-11
            CONFIG_value = CONFIG_value % 12; // 0-11 only
            // CONFIG_value++; // 1-12
          }else if(_result == 2){
            CONFIG_CLOCK_second = CONFIG_value;
            CONFIG_state = 1;
          }
          WatchOS.LED_clear();
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON);

          DEBUG_F Serial.print("[DEBUG] CONFIG_value: ");
          DEBUG_F Serial.println(CONFIG_value);
          DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_second: ");
          DEBUG_F Serial.println(CONFIG_CLOCK_second);
        }
      }
      if(CONFIG_state == 1){
        for (int i = 0; i < 2; i++){
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_OFF);
          delay(100);
          WatchOS.LED_write(NUMBER_F(CONFIG_value), LED_ON); 
          delay(100);
        }
        CONFIG_state = 2;
      }
      if(CONFIG_state == 2){
        // BUTTON
        int _result = WatchOS.BUTTON_getResult();
        if(_result > 0){
          FSM_ALIVE_millis = millis();
          WatchOS.LED_clear();
          if(_result == 1){
            break;
          }
        }
      }
    
      
      // unused guard, check device is alive
      if(FSM_timeout(FSM_ALIVE_MS)) break;
    }
#endif
#pragma endregion CONFIG_SECOND

    // ------------------------------------------------------------
    // SAVE CONFIG
    // ------------------------------------------------------------
#pragma region CONFIG_SAVE
    while (1) {
      DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_hour: ");
      DEBUG_F Serial.println(CONFIG_CLOCK_hour);
      DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_minute: ");
      DEBUG_F Serial.println(CONFIG_CLOCK_minute);
      DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_plusminute: ");
      DEBUG_F Serial.println(CONFIG_CLOCK_plusminute);
      DEBUG_F Serial.print("[DEBUG] CONFIG_CLOCK_second: ");
      DEBUG_F Serial.println(CONFIG_CLOCK_second);

      int result_hour = CONFIG_CLOCK_hour;
      int result_minutes = CONFIG_CLOCK_minute*5 + CONFIG_CLOCK_plusminute;
      int result_seconds = CONFIG_CLOCK_second*5 + 1; // +1 button press delay
      
      DEBUG_F Serial.print("[DEBUG] RESULT: ");
      DEBUG_F Serial.print(result_hour);
      DEBUG_F Serial.print(":");
      DEBUG_F Serial.print(result_minutes);
      DEBUG_F Serial.print(":");
      DEBUG_F Serial.print(result_seconds);
      DEBUG_F Serial.println();

      WatchOS.RTC_setTime(result_hour, result_minutes, result_seconds);
      
      LED_all_delay(LED_ON, 100);
      LED_all_delay(LED_OFF, 100);
      LED_all_delay(LED_ON, 100);
      LED_all_delay(LED_OFF, 100);
      WatchOS.LED_clear();
      break;
    }
#pragma endregion CONFIG_SAVE


}
#pragma endregion CONFIG_TIME
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region DEBUG_FIME
// ------------------------------------------------------------------
void DEBUG_FIME(){
    int DEBUG_RTC_led_active = 0;
    unsigned long DEBUG_FIME_millis = 0;
    unsigned long DEBUG_FIME_led_millis = 0;

    int DEBUG_led_index = 22;
    unsigned long DEBUG_millis = 0;
    while(1){
      if(millis() - DEBUG_FIME_millis > 1000){
        WatchOS.LED_write(NUMBER_F(3), LED_ON);
        WatchOS.LED_write(NUMBER_F(8), LED_ON);
        WatchOS.LED_write(NUMBER_F(9), LED_ON);
        WatchOS.LED_write(NUMBER_F(12), LED_ON);
        DEBUG_RTC_led_active = 1;
        DEBUG_FIME_led_millis = millis();

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
        DEBUG_FIME_millis = millis();
      }

      if(DEBUG_RTC_led_active && (millis() - DEBUG_FIME_led_millis > 100)){
        DEBUG_RTC_led_active = 0;
        WatchOS.LED_write(NUMBER_F(3), LED_OFF);
        WatchOS.LED_write(NUMBER_F(8), LED_OFF);
        WatchOS.LED_write(NUMBER_F(9), LED_OFF);
        WatchOS.LED_write(NUMBER_F(12), LED_OFF);
      }

      if((millis() - DEBUG_millis > 75)){
        if(DEBUG_led_index >= 24) DEBUG_led_index = 0;
        WatchOS.LED_write(DEBUG_led_index++/2, !(DEBUG_led_index%2));
        DEBUG_millis = millis();
      }

      if(WatchOS.BUTTON_isSafePressed(50)) break;

    }
}
#pragma endregion DEBUG_FIME
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region DISPLAY_TIME
// ------------------------------------------------------------------
void WATCH_display_mode(int mode){
  WatchOS.RTC_update();
  int clock_hour = WatchOS.RTC_getHour();
  int clock_minute = WatchOS.RTC_getMinute();
  int clock_second = WatchOS.RTC_getSecond();
  // int clock_hour = random(0, 11);
  // int clock_minute = random(0, 59);
  // int clock_second = random(0, 59);

  int clock_hour_tens = (clock_hour/10)%10; 
  int clock_hour_unit = clock_hour%10;
  int clock_minute_tens = (clock_minute/10)%10; 
  int clock_minute_unit = clock_minute%10;
  int clock_second_tens = (clock_second/10)%10; 
  int clock_second_unit = clock_second%10;


  DEBUG_F Serial.print("[DEBUG] DISPLAYING RAW: ");
  DEBUG_F Serial.print(clock_hour);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_minute);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_second);
  DEBUG_F Serial.println();

  DEBUG_F Serial.print("[DEBUG] DISPLAYING DECIMAL: ");
  DEBUG_F Serial.print(clock_hour_tens);
  DEBUG_F Serial.print("-");
  DEBUG_F Serial.print(clock_hour_unit);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_minute_tens);
  DEBUG_F Serial.print("-");
  DEBUG_F Serial.print(clock_minute_unit);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_second_tens);
  DEBUG_F Serial.print("-");
  DEBUG_F Serial.print(clock_second_unit);
  DEBUG_F Serial.println();

  // replace 0 with 12 before displaying
  if(clock_hour_tens == 0) clock_hour_tens = 12; 
  if(clock_hour_unit == 0) clock_hour_unit = 12; 
  if(clock_minute_tens == 0) clock_minute_tens = 12; 
  if(clock_minute_unit == 0) clock_minute_unit = 12; 
  if(clock_second_tens == 0) clock_second_tens = 12; 
  if(clock_second_unit == 0) clock_second_unit = 12; 

  if(mode == 0){
    // adjust based on watch
    if(clock_hour > 12) clock_hour -= 12;
    int result_hour = clock_hour;

    int result_minute = clock_minute / 5;
    int result_minute_remainder = clock_minute % 5;

    // show hour hand
    WatchOS.LED_write(NUMBER_F(result_hour), LED_ON);
    delay(500);

    // show minute hand
    if(result_minute == result_hour){ // if minute sama as hour just turn off a little to see the transition
      WatchOS.LED_write(NUMBER_F(result_minute), LED_OFF);
      delay(200);
    }
    WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
    delay(2000);
    WatchOS.LED_clear();

    // show plus hand
    for (int i = 0; i < result_minute_remainder; i++){
      int _plusminute = result_minute+i+1;
      if(_plusminute > 12) _plusminute -= 12;
      
      WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
      WatchOS.LED_write(NUMBER_F(_plusminute), LED_ON);
      delay(250);
      WatchOS.LED_write(NUMBER_F(result_minute), LED_ON);
      WatchOS.LED_write(NUMBER_F(_plusminute), LED_OFF);
      delay(250);
    }
    WatchOS.LED_clear();
    delay(1000);

    // show second
    int result_second = ((clock_second/10) * 2) + 2; // +2 means the maximum tolerenace of the clock 
#ifdef USE_SECOND
    if(result_second > 0){
      WatchOS.LED_write(NUMBER_F(result_second), LED_ON);
    }else{
      WatchOS.LED_write(NUMBER_F(12), LED_ON);
    }
    delay(2000);
#endif

    WatchOS.LED_clear();

    DEBUG_F Serial.print("[DEBUG] DISPLAYING LED: ");
    DEBUG_F Serial.print(result_hour);
    DEBUG_F Serial.print(":");
    DEBUG_F Serial.print(result_minute);
    DEBUG_F Serial.print("+");
    DEBUG_F Serial.print(result_minute_remainder);
    DEBUG_F Serial.print(":");
    DEBUG_F Serial.print(result_second);
    DEBUG_F Serial.println();
  }
  if(mode == 1){
    WatchOS.LED_write(NUMBER_F(clock_hour_tens), LED_ON);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_hour_tens), LED_OFF);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_hour_unit), LED_ON);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_hour_unit), LED_OFF);
    delay(500);

    WatchOS.LED_write(NUMBER_F(clock_minute_tens), LED_ON);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_minute_tens), LED_OFF);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_minute_unit), LED_ON);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_minute_unit), LED_OFF);
    delay(500);

#ifdef USE_SECOND
    WatchOS.LED_write(NUMBER_F(clock_second_tens), LED_ON);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_second_tens), LED_OFF);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_second_unit), LED_ON);
    delay(500);
    WatchOS.LED_write(NUMBER_F(clock_second_unit), LED_OFF);
    delay(500);
#endif
  }
  if(mode == 2){
    WatchOS.LED_write(NUMBER_F(clock_hour_tens), LED_ON);
    for (int i = 0; i < 10; i++){
      WatchOS.LED_write(NUMBER_F(clock_hour_unit), LED_ON);
      delay(50);
      WatchOS.LED_write(NUMBER_F(clock_hour_unit), LED_OFF);
      delay(50);
    }
    WatchOS.LED_write(NUMBER_F(clock_hour_tens), LED_OFF);
    // delay(500);
    

    WatchOS.LED_write(NUMBER_F(clock_minute_tens), LED_ON);
    for (int i = 0; i < 10; i++){
      WatchOS.LED_write(NUMBER_F(clock_minute_unit), LED_ON);
      delay(50);
      WatchOS.LED_write(NUMBER_F(clock_minute_unit), LED_OFF);
      delay(50);
    }
    WatchOS.LED_write(NUMBER_F(clock_minute_tens), LED_OFF);
    // delay(500);

#ifdef USE_SECOND
    WatchOS.LED_write(NUMBER_F(clock_second_tens), LED_ON);
    for (int i = 0; i < 10; i++){
      WatchOS.LED_write(NUMBER_F(clock_second_unit), LED_ON);
      delay(50);
      WatchOS.LED_write(NUMBER_F(clock_second_unit), LED_OFF);
      delay(50);
    }
    WatchOS.LED_write(NUMBER_F(clock_second_tens), LED_OFF);
    // delay(500);
#endif
  }
}
#pragma endregion DISPLAY_TIME
// ------------------------------------------------------------------
/* END USER APP CODE 1*/