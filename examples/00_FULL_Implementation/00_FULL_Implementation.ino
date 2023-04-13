#include <Arduino.h>

/*
 * REAL IMPLEMENTATION (FULL)
 */
/* USER DEFINE */
#define DISPLAY_MODE 0 // NORMAL
// #define DISPLAY_MODE 1 // DECIMAL 1
// #define DISPLAY_MODE 2 // DECIMAL 2
// #define USE_SECOND

#define DEBUG_F_ENABLE true

// FSM CONFIG
#define FSM_ALIVE_MS 30000 // when entering config exit after 30s inactivity

/* END USER DEFINE*/
// #define APP_COMPATIBILITY // ENABLE ONLY IF YOU HAVE BETA TEST BOARD
#include "TQDF_WatchOS.h"
#include "config_AEONF_mk6.h"

/* USER VARIABLE */
// ------------------------------------------------------------------
// Finite State Machine (FSM) Variable
// ------------------------------------------------------------------
int FSM_STATE = 0;
unsigned long FSM_ALIVE_millis = 0;
int FSM_CLICK_count = 0;
unsigned long FSM_CLICK_millis = 0;
int FSM_DISPLAY_mode = 0;

// ------------------------------------------------------------------
// CONFIGURATION Variable
// ------------------------------------------------------------------
// CONFIG Parameter
int CONFIG_state = 0;
int CONFIG_value = 0;
unsigned long CONFIG_millis = 0;

int CONFIG_MENU_index = 0;
int CONFIG_MENU_max = 5;
int CONFIG_CLOCK_hour = 0;
int CONFIG_CLOCK_minute = 0;
int CONFIG_CLOCK_plusminute = 0;
int CONFIG_CLOCK_second = 0;

int CONFIG_DATE_year = 0;
int CONFIG_DATE_month = 0;
int CONFIG_DATE_day = 0;

/* END USER VARIABLE*/

void setup() {
  WatchOS.config(
    /* Serial Baudrate:  */ 115200, // 9600 or 115200
    /* RTC Preserved     */ true, 
    /* RTC Callibration: */ REPLACE_WITH_YOUR_CALIBRATION_VALUE, // From manufacturer (back of the PCB)
    /* WATCHDOG Routine  */ WatchOS.WAKE_EVERY_15M,
    /* GPIO -------------------------------------------------------------------*/ 
    /* Clock pin (1-12): */ led_clock_pins, 
    /* LED Power pin:    */ led_power_pin, 
    /* Button pin:       */ button_pin, 
    /* Beeper pin:       */ beeper_pin, 
    /* Unused pins:      */ unused_pins
  );
  /* USER SETUP CODE 1*/
  WatchOS.LED_setBrightnessMax(50); // brightness: 1-100 percent
  WatchOS.RTC_initialTime(7,54,30);
  WatchOS.RTC_initialDate(20,03,23);
  WatchOS.WATCHDOG_reconfigureWakeUp(true);

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
    WatchOS.LED_write(result_minute, LED_ON);
    WatchOS.BEEPER_play(12500, 50);
    delay(50);
    WatchOS.LED_write(result_minute, LED_OFF);
    delay(50);
    WatchOS.BEEPER_play(12500, 50);
    delay(50);

    WatchOS.WATCHDOG_clearInterruptFlag();
    WatchOS.shutdown();
  }
  // ------------------------------------------------------------
  // START THE FSM
  // ------------------------------------------------------------
  FSM_begin();

  // ------------------------------------------------------------
  // DISPLAY TIME
  // ------------------------------------------------------------
  // detect double click
  if(FSM_STATE == 0){
    while (1){
      if(millis() - FSM_CLICK_millis < 200){
        if(WatchOS.BUTTON_isPressed()){
          FSM_CLICK_count++;
        }
      }else{
        if(FSM_CLICK_count < 1){
          FSM_DISPLAY_mode = 0;
        }else{
          FSM_DISPLAY_mode = 1;
        }
        FSM_STATE = 1;
        break;
      }
    }
    
  }

  // display the time
  if(FSM_STATE == 1){
    // make sure the button released
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    if(FSM_DISPLAY_mode == 0) WATCH_TIME_mode(DISPLAY_MODE);
    if(FSM_DISPLAY_mode == 1) WATCH_DATE_mode(0);

    if(WatchOS.BUTTON_isPressed()){
      LED_cycle_up(LED_ON, 50);
    }

    // Enter the menu 
    // if the button still pressed after the led animation
    if(WatchOS.BUTTON_isPressed()){
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
    CONFIG_DATE();
  }
  if(FSM_STATE == 13){
    for(int i=0; i<5; i++){
      WatchOS.LED_write(5, LED_ON);
      delay(100);
      WatchOS.LED_write(5, LED_OFF);
      delay(100);
    }
  }
  if(FSM_STATE == 14){
    for(int i=0; i<5; i++){
      WatchOS.LED_write(5, LED_ON);
      delay(100);
      WatchOS.LED_write(5, LED_OFF);
      delay(100);
    }
  }
  if(FSM_STATE == 15){
    DEBUG_FIME();
  }
  if(FSM_STATE == 16){
    WatchOS.bootloader();
  }
  WatchOS.shutdown();
}

// ------------------------------------------------------------------
#pragma region FSM // FINITE STATE MACHINE
// ------------------------------------------------------------------
void FSM_begin(){
  FSM_STATE = 0;
  FSM_ALIVE_millis = millis();
  FSM_CLICK_millis = millis();
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
void LED_cycle_up(bool led_state, unsigned long delay_ms){
  /*
  // beginner way
  WatchOS.LED_write(6), led_state);
  delay(delay_ms);
  WatchOS.LED_write(5), led_state);
  WatchOS.LED_write(7), led_state);
  delay(delay_ms);
  WatchOS.LED_write(4), led_state);
  WatchOS.LED_write(8), led_state);
  delay(delay_ms);
  WatchOS.LED_write(3), led_state);
  WatchOS.LED_write(9), led_state);
  delay(delay_ms);
  WatchOS.LED_write(2), led_state);
  WatchOS.LED_write(10), led_state);
  delay(delay_ms);
  WatchOS.LED_write(1), led_state);
  WatchOS.LED_write(11), led_state);
  delay(delay_ms);
  WatchOS.LED_write(12), led_state);
  */
  
  // an expert
  int led_number = 6;
  for (int i = 0; i <= 12/2; i++){
    WatchOS.LED_write(led_number-i, led_state);
    WatchOS.LED_write(led_number+i, led_state);
    delay(delay_ms);
  }
  
}
void LED_cycle_down(bool led_state, unsigned long delay_ms){
  int led_number = 12;
  for (int i = 0; i <= 12/2; i++){
    WatchOS.LED_write(led_number-i, led_state);
    WatchOS.LED_write(i, led_state);
    delay(delay_ms);
  }
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
    WatchOS.LED_write(CONFIG_value, LED_ON);

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
            WatchOS.LED_write(CONFIG_value, LED_OFF);
            // CONFIG_value++; // 0-5
            CONFIG_value = CONFIG_value % (CONFIG_MENU_max + 1); // 0-6 only
            CONFIG_value++; // 1-6

            DEBUG_F Serial.print("[DEBUG] CONFIG_value: ");
            DEBUG_F Serial.println(CONFIG_value);

            LED_cycle_up(LED_ON, 30);
            LED_cycle_down(LED_OFF, 30);

            WatchOS.LED_write(CONFIG_value, LED_ON);
          }else if(_result == 2){
            CONFIG_MENU_index = CONFIG_value;

            DEBUG_F Serial.print("[DEBUG] CONFIG_MENU: ");
            DEBUG_F Serial.println(CONFIG_MENU_index);
            
            CONFIG_state = 1;

            WatchOS.LED_write(CONFIG_value, LED_ON);
          }
        }
      }
      if(CONFIG_state == 1){
        for (int i = 0; i < 2; i++){
          WatchOS.LED_write(CONFIG_value, LED_OFF);
          delay(100);
          WatchOS.LED_write(CONFIG_value, LED_ON); 
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
#pragma region CONFIG_DECIMAL
// ------------------------------------------------------------------
void CONFIG_DECIMAL(int* variable, int value_min, int value_max){

  // loop
  CONFIG_millis = millis();
  CONFIG_state = 0;
  int config_value_last = 0;
  int config_value = 0;
  int result_value = 0;
  int decimal_state = 0;
  int decimal_max = 0;
  int decimal_min = 0;

  while (1) {
    // avoid using loop
    if(CONFIG_state == 0){
      if(decimal_state == 0){
        config_value = *variable/10;
      }else if(decimal_state == 0){
        if(result_value < *variable){
          config_value = *variable % 10;
        }else{
          config_value = 0;
        }
      }

      WatchOS.LED_write(config_value_last, LED_OFF);
      WatchOS.LED_write(config_value, LED_ON);
      config_value_last = config_value;
      
      CONFIG_state = 1;
    }
    if(CONFIG_state == 1){
      // BUTTON
      int _result = WatchOS.BUTTON_getResult();
      if(_result > 0){
        FSM_ALIVE_millis = millis();
        if(_result == 1){

          if(decimal_state == 0){
            decimal_max = value_max / 10;
            decimal_min = 0;
          } else
          if(decimal_state == 1){
             decimal_max = min(value_max - result_value, 9);
             decimal_min = 0;
          }

          config_value++; // 0-11
          if(config_value > decimal_max){
            config_value = decimal_min;
          }
        }else if(_result == 2){
          CONFIG_state = 2;
        }

        WatchOS.LED_write(config_value_last, LED_OFF);
        WatchOS.LED_write(config_value, LED_ON);
        config_value_last = config_value;

        DEBUG_F Serial.print("[DEBUG] config_value: ");
        DEBUG_F Serial.println(config_value);
      }
    }
    if(CONFIG_state == 2){
      for (int i = 0; i < 2; i++){
        WatchOS.LED_write(config_value, LED_OFF);
        delay(100);
        WatchOS.LED_write(config_value, LED_ON); 
        delay(100);
      }

      CONFIG_state = 3;
    }
    if(CONFIG_state == 3){
      while (1){
        if(decimal_state == 0){
          result_value = config_value * 10;
          decimal_state++;
          CONFIG_state = 0;
          break;
        }
        if(decimal_state == 1){
          result_value += config_value;

          DEBUG_F Serial.print("[DEBUG] result_value: ");
          DEBUG_F Serial.println(result_value);
          *variable = result_value;
          CONFIG_state = 4;
          break;
        }
      }
    }
    if(CONFIG_state == 4){
      break;
    }
    // unused guard, check device is alive
    if(FSM_timeout(FSM_ALIVE_MS)) break;
  }

}

#pragma endregion CONFIG_DECIMAL
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region CONFIG_TIME
// ------------------------------------------------------------------
void CONFIG_TIME(){
  do{delay(200);} while(WatchOS.BUTTON_isPressed());

  FSM_ALIVE_millis = millis();

  // setup
  WatchOS.RTC_update();
  int hourvalue = WatchOS.RTC_getHour();
  int minutevalue = WatchOS.RTC_getMinute();
  int secondvalue = WatchOS.RTC_getSecond();
#pragma region CONFIG_HOUR
  // ------------------------------------------------------------
  // CONFIG HOUR
  // ------------------------------------------------------------
  DEBUG_F Serial.println("[DEBUG] CONFIG HOUR HAND");
  CONFIG_CLOCK_hour = hourvalue;
  WatchOS.LED_clear();
  WatchOS.LED_write(10, LED_ON);
  CONFIG_DECIMAL(&CONFIG_CLOCK_hour, 0, 23);
#pragma endregion CONFIG_HOUR
  
#pragma region CONFIG_MINUTE
  // ------------------------------------------------------------
  // CONFIG MINUTE
  // ------------------------------------------------------------
  DEBUG_F Serial.println("[DEBUG] CONFIG HOUR HAND");
  CONFIG_CLOCK_minute = minutevalue;
  WatchOS.LED_clear();
  WatchOS.LED_write(11, LED_ON);
  CONFIG_DECIMAL(&CONFIG_CLOCK_minute, 0, 59);
#pragma endregion CONFIG_MINUTE

  // ------------------------------------------------------------
  // CONFIG SECOND (NOT IMPLEMENTED)
  // ------------------------------------------------------------
  CONFIG_CLOCK_second = 30;

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

      WatchOS.RTC_setTime(CONFIG_CLOCK_hour, CONFIG_CLOCK_minute, CONFIG_CLOCK_second);
      
      LED_cycle_down(LED_ON, 75);
      WatchOS.LED_clear();
      break;
    }
#pragma endregion CONFIG_SAVE
}
#pragma endregion CONFIG_TIME
// ------------------------------------------------------------------


// ------------------------------------------------------------------
#pragma region CONFIG_DATE
// ------------------------------------------------------------------
void CONFIG_DATE(){
  do{delay(200);} while(WatchOS.BUTTON_isPressed());

  FSM_ALIVE_millis = millis();

  // setup
  WatchOS.RTC_update();
  int yearvalue = WatchOS.RTC_getYear();
  int monthvalue = WatchOS.RTC_getMonth();
  int dayvalue = WatchOS.RTC_getDay();
  // ------------------------------------------------------------
  // CONFIG YEAR
  // ------------------------------------------------------------
  CONFIG_DATE_year = yearvalue;

  // ------------------------------------------------------------
  // CONFIG MONTH
  // ------------------------------------------------------------
  DEBUG_F Serial.println("[DEBUG] CONFIG MONTH");
  CONFIG_DATE_month = monthvalue;
  WatchOS.LED_clear();
  WatchOS.LED_write(10, LED_ON);
  CONFIG_DECIMAL(&CONFIG_DATE_month, 0, 12);
  
  // ------------------------------------------------------------
  // CONFIG DAY
  // ------------------------------------------------------------
  DEBUG_F Serial.println("[DEBUG] CONFIG DAY");
  CONFIG_DATE_day = dayvalue;
  WatchOS.LED_clear();
  WatchOS.LED_write(11, LED_ON);
  CONFIG_DECIMAL(&CONFIG_DATE_day, 0, 31);

    // ------------------------------------------------------------
    // SAVE CONFIG
    // ------------------------------------------------------------
#pragma region CONFIG_SAVE
    while (1) {
      DEBUG_F Serial.print("[DEBUG] CONFIG_DATE_year: ");
      DEBUG_F Serial.println(CONFIG_DATE_year);
      DEBUG_F Serial.print("[DEBUG] CONFIG_DATE_month: ");
      DEBUG_F Serial.println(CONFIG_DATE_month);
      DEBUG_F Serial.print("[DEBUG] CONFIG_DATE_day: ");
      DEBUG_F Serial.println(CONFIG_DATE_day);

      WatchOS.RTC_setDate(CONFIG_DATE_day, CONFIG_DATE_month, CONFIG_DATE_year);
      
      LED_cycle_down(LED_ON, 75);
      WatchOS.LED_clear();
      break;
    }
#pragma endregion CONFIG_SAVE
}
#pragma endregion CONFIG_DATE
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

    // wait button press
    do{delay(200);} while(WatchOS.BUTTON_isPressed());

    while(1){
      if(millis() - DEBUG_FIME_millis > 1000){
        WatchOS.LED_write(0, LED_ON);
        WatchOS.LED_write(3, LED_ON);
        WatchOS.LED_write(6, LED_ON);
        WatchOS.LED_write(9, LED_ON);
        WatchOS.BEEPER_play(12500, 50);

        DEBUG_RTC_led_active = 1;
        DEBUG_FIME_led_millis = millis();

        WatchOS.RTC_update();
        int hourvalue = WatchOS.RTC_getHour();
        int minutevalue = WatchOS.RTC_getMinute();
        int secondvalue = WatchOS.RTC_getSecond();

        // print the Device UID
        char tmp[16];
        Serial.print("Device UID: ");
        sprintf(tmp, "%.8X", WatchOS.getUID(0));
        Serial.print(tmp);
        Serial.print("-");
        sprintf(tmp, "%.8X", WatchOS.getUID(1));
        Serial.print(tmp);
        Serial.print("-");
        sprintf(tmp, "%.8X", WatchOS.getUID(2));
        Serial.print(tmp);
        Serial.println();

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
        WatchOS.LED_write(0, LED_OFF);
        WatchOS.LED_write(3, LED_OFF);
        WatchOS.LED_write(6, LED_OFF);
        WatchOS.LED_write(9, LED_OFF);
        WatchOS.BEEPER_play(12500, 50); delay(50);
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
void WATCH_TIME_mode(int mode){
  WatchOS.RTC_update();
  int clock_hour = WatchOS.RTC_getHour();
  int clock_minute = WatchOS.RTC_getMinute();
  int clock_second = WatchOS.RTC_getSecond();
  // int clock_hour = random(0, 11);
  // int clock_minute = random(0, 59);
  // int clock_second = random(0, 59);

  DEBUG_F Serial.print("[DEBUG] RAW TIME: ");
  DEBUG_F Serial.print(clock_hour);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_minute);
  DEBUG_F Serial.print(":");
  DEBUG_F Serial.print(clock_second);
  DEBUG_F Serial.println();

  if(mode == 0){
    WATCH_TIME_mode_normal(clock_hour, clock_minute, clock_second);
  }
  // experimental
  if(mode == 1){
    WATCH_TIME_mode_decimal1(clock_hour, clock_minute, clock_second);
  }
  // experimental
  if(mode == 2){
    WATCH_TIME_mode_decimal2(clock_hour, clock_minute, clock_second);
  }
}

void WATCH_TIME_mode_normal(int hour, int minute, int second){
    // adjust based on watch
    // if(hour > 12) hour -= 12;
    int result_hour = hour % 12;

    int result_minute = minute / 5;
    int result_minute_remainder = minute % 5;

    int result_second = ((second/10) * 2) + 2; // +2 means the maximum tolerenace of the clock 

    DEBUG_F Serial.print("[DEBUG] DISPLAYING LED: ");
    DEBUG_F Serial.print(result_hour);
    DEBUG_F Serial.print(":");
    DEBUG_F Serial.print(result_minute);
    DEBUG_F Serial.print("+");
    DEBUG_F Serial.print(result_minute_remainder);
    DEBUG_F Serial.print(":");
    DEBUG_F Serial.print(result_second);
    DEBUG_F Serial.println();

    // show hour hand
    WatchOS.LED_write(result_hour, LED_ON);
    delay(500);

    // show minute hand
    if(result_minute == result_hour){ // if minute sama as hour just turn off a little to see the transition
      WatchOS.LED_write(result_minute, LED_OFF);
      delay(200);
    }
    WatchOS.LED_write(result_minute, LED_ON);
    delay(2000);
    WatchOS.LED_clear();

    // show plus hand
    for (int i = 0; i < result_minute_remainder; i++){
      int _plusminute = (result_minute+i+1) % 12;
      
      WatchOS.LED_write(result_minute, LED_ON);
      WatchOS.LED_write(_plusminute, LED_ON);
      delay(250);
      WatchOS.LED_write(result_minute, LED_ON);
      WatchOS.LED_write(_plusminute, LED_OFF);
      delay(250);
    }
    WatchOS.LED_clear();
    // delay(1000);

    // show second
#ifdef USE_SECOND
    if(result_second > 0){
      WatchOS.LED_write(result_second), LED_ON);
    }else{
      WatchOS.LED_write(12), LED_ON);
    }
    delay(2000);
#endif
    // WatchOS.LED_clear();
}
void WATCH_TIME_mode_decimal1(int hour, int minute, int second){
  if(hour > 12) hour -= 12;
  int clock_hour_tens = (hour/10)%10; 
  int clock_hour_unit = hour%10;
  int clock_minute_tens = (minute/10)%10; 
  int clock_minute_unit = minute%10;
  int clock_second_tens = (second/10)%10; 
  int clock_second_unit = second%10;

  DEBUG_F Serial.print("[DEBUG] DISPLAYING DECIMAL (12-hour format): ");
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

  WatchOS.LED_write(clock_hour_tens, LED_ON);
  delay(500);
  WatchOS.LED_write(clock_hour_tens, LED_OFF);
  delay(500);
  WatchOS.LED_write(clock_hour_unit, LED_ON);
  delay(500);
  WatchOS.LED_write(clock_hour_unit, LED_OFF);
  delay(500);

  WatchOS.LED_write(clock_minute_tens, LED_ON);
  delay(500);
  WatchOS.LED_write(clock_minute_tens, LED_OFF);
  delay(500);
  WatchOS.LED_write(clock_minute_unit, LED_ON);
  delay(500);
  WatchOS.LED_write(clock_minute_unit, LED_OFF);
  delay(500);

#ifdef USE_SECOND
  WatchOS.LED_write(clock_second_tens), LED_ON);
  delay(500);
  WatchOS.LED_write(clock_second_tens), LED_OFF);
  delay(500);
  WatchOS.LED_write(clock_second_unit), LED_ON);
  delay(500);
  WatchOS.LED_write(clock_second_unit), LED_OFF);
  delay(500);
#endif
}
void WATCH_TIME_mode_decimal2(int hour, int minute, int second){
  if(hour > 12) hour -= 12;
  // int clock_hour_tens = (hour/10)%10; 
  // int clock_hour_unit = hour%10;
  int clock_minute_tens = (minute/10)%10; 
  int clock_minute_unit = minute%10;
  int clock_second_tens = (second/10)%10; 
  int clock_second_unit = second%10;

  DEBUG_F Serial.print("[DEBUG] DISPLAYING DECIMAL (12-hour format): ");
  DEBUG_F Serial.print(hour);
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
  // if(clock_hour_tens == 0) clock_hour_tens = 12; 
  // if(clock_hour_unit == 0) clock_hour_unit = 12; 
  if(clock_minute_tens == 0) clock_minute_tens = 12; 
  if(clock_minute_unit == 0) clock_minute_unit = 12; 
  if(clock_second_tens == 0) clock_second_tens = 12; 
  if(clock_second_unit == 0) clock_second_unit = 12; 

  WatchOS.LED_write(hour, LED_ON);
  delay(500);
  WatchOS.LED_write(hour, LED_OFF);
  delay(500);
  

  WatchOS.LED_write(clock_minute_tens, LED_ON);
  for (int i = 0; i < 10; i++){
    WatchOS.LED_write(clock_minute_unit, LED_ON);
    delay(50);
    WatchOS.LED_write(clock_minute_unit, LED_OFF);
    delay(50);
  }
  WatchOS.LED_write(clock_minute_tens, LED_OFF);
  // delay(500);

#ifdef USE_SECOND
  WatchOS.LED_write(clock_second_tens), LED_ON);
  for (int i = 0; i < 10; i++){
    WatchOS.LED_write(clock_second_unit), LED_ON);
    delay(50);
    WatchOS.LED_write(clock_second_unit), LED_OFF);
    delay(50);
  }
  WatchOS.LED_write(clock_second_tens), LED_OFF);
  // delay(500);
#endif
}

#pragma endregion DISPLAY_TIME
// ------------------------------------------------------------------

// ------------------------------------------------------------------
#pragma region DISPLAY_DATE
// ------------------------------------------------------------------
void WATCH_DATE_mode(int mode){
  WatchOS.RTC_update();
  int date_year = WatchOS.RTC_getYear();
  int date_month = WatchOS.RTC_getMonth();
  int date_day = WatchOS.RTC_getDay();


  DEBUG_F Serial.print("[DEBUG] RAW DATE: ");
  DEBUG_F Serial.print(date_year);
  DEBUG_F Serial.print("/");
  DEBUG_F Serial.print(date_month);
  DEBUG_F Serial.print("/");
  DEBUG_F Serial.print(date_day);
  DEBUG_F Serial.println();

  if(mode == 0){
    WATCH_DATE_mode_normal(date_year, date_month, date_day);
  }

}

void WATCH_DATE_mode_normal(int year, int month, int day){ 
    int clock_year_tens = (year/10)%10;
    int clock_year_unit = year%10;
    int clock_month_tens = (month/10)%10;
    int clock_month_unit = month%10;
    int clock_day_tens = (day/10)%10;
    int clock_day_unit = day%10;

    DEBUG_F Serial.print("[DEBUG] DECIMAL DATE: ");
    DEBUG_F Serial.print(clock_year_tens);
    DEBUG_F Serial.print("-");
    DEBUG_F Serial.print(clock_year_unit);
    DEBUG_F Serial.print("/");
    DEBUG_F Serial.print(clock_month_tens);
    DEBUG_F Serial.print("-");
    DEBUG_F Serial.print(clock_month_unit);
    DEBUG_F Serial.print("/");
    DEBUG_F Serial.print(clock_day_tens);
    DEBUG_F Serial.print("-");
    DEBUG_F Serial.print(clock_day_unit);
    DEBUG_F Serial.println();

    
    WatchOS.LED_write(clock_day_tens, LED_ON);
    for (int i = 0; i < 15; i++){
      WatchOS.LED_write(clock_day_unit, LED_ON);
      delay(50);
      WatchOS.LED_write(clock_day_unit, LED_OFF);
      delay(50);
    }
    WatchOS.LED_write(clock_day_tens, LED_OFF);
    // delay(500);

    // DISPLAY THE MONTH
    /*
    WatchOS.LED_write(clock_month_tens, LED_ON);
    for (int i = 0; i < 15; i++){
      WatchOS.LED_write(clock_month_unit, LED_ON);
      delay(50);
      WatchOS.LED_write(clock_month_unit, LED_OFF);
      delay(50);
    }
    WatchOS.LED_write(clock_month_tens, LED_OFF);
    */
}

#pragma endregion DISPLAY_DATE
// ------------------------------------------------------------------
/* END USER APP CODE 1*/