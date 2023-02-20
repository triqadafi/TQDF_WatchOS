/** 
 *  @file   TQDF_WatchOS.h
 *  @brief  TQDF_WatchOS Library by @triqadafi. TRDF is a shorthand of triqadafi
 *  @version 1.0.0
 *  @author Triqadafi
 *  @date   2023-02-05 
 ***********************************************/

#include <Arduino.h>
#include "STM32LowPower.h"
#include "STM32RTC.h"

#ifndef TQDF_WatchOS_h
#define TQDF_WatchOS_h

#define NUMBER_F(x) (x-1)

#define USE_STM32
#define USE_STM32F401CC

// default
#define GPIO_LED_INVERT 1 // pin as sink
#define GPIO_LED_POWER_INVERT 1  // Pchannel
#define GPIO_BUTTON_INVERT 1  // High-side

// default
#define CONFIG_BUTTON_DEBOUNCER_MS 100
#define CONFIG_BUTTON_SHORTPRESS_MS 1250

class TQDF_WatchOS
{
  public:
    /**
     * @brief Represent the possible routine for the WATCHDOG. Used by WATCHDOG_setRoutine()
     * 
     */
    enum WATCHDOG_wake_interval : uint8_t {
      /// 1 Second (Beware if you using 1 second interval with shutdown)
      WAKE_EVERY_1S,
      /// 5 Second
      WAKE_EVERY_5S,
      /// 10 Seconds
      WAKE_EVERY_10S,
      /// 1 Minute
      WAKE_EVERY_1M,
      /// 5 Minutes
      WAKE_EVERY_5M,
      /// 15 Minutes
      WAKE_EVERY_15M
    };

    /**
     * @brief Represent the value returned from the BUTTON press behavior. Used by BUTTON_getResult()
     * 
     */
    enum BUTTON_state : uint8_t {
      /// No result from the button. Just no action or indicate the button on normal state.
      NO_ACTION,
      /// Button has been pressed under CONFIG_BUTTON_SHORTPRESS_MS
      SHORT_PRESSED,
      /// Button has been pressed over CONFIG_BUTTON_SHORTPRESS_MS
      LONG_PRESSED,
    };

    /// Register location
    enum REGISTER_location : uint8_t {
      /// Represents the value update from the REGISTER_write(). BEWARE! It is not always repesents the value stored in the register.
      RUNTIME,
      /// Represent the value from the register itselft.
      SOURCE
    };

    enum LED_power : uint8_t {
      /// Lowest power consumtion possinble, Current consumption based on LED that turned on. Maximum is 4 LED on the same time.
      POWER_DYNAMIC,
      /// Maximum brightness. Recommended for maximum 2 LEDs only
      POWER_MAXIMUM
    };


    TQDF_WatchOS();

    uint8_t LED_ON_logic = 0;
    uint8_t LED_OFF_logic = 0;
    uint8_t LED_POWER_ON_logic = 0;
    uint8_t LED_POWER_OFF_logic = 0;
    uint8_t BUTTON_NORMAL_logic = 0;
    uint8_t BUTTON_PRESSED_logic = 0;

    uint8_t LED_dynamic_curve[13] = {0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

    unsigned long BUTTON_DEBOUNCER_MS = CONFIG_BUTTON_DEBOUNCER_MS;
    unsigned long BUTTON_SHORTPRESS_MS = CONFIG_BUTTON_SHORTPRESS_MS;
    
    /**
     * @brief Configure device basic parameters. Responsible for all peripheral initialization.
     * 
     * @param baudrate UARD baudrate (Recommended 9600 ar 115200)
     * @param preserve_RTC Keep the last value. RTC will not reset even after programmed. Only POWER_OFF resets the RTC.
     * @param calibration_value 
     * Real Time Clock (RTC) calbration value from the manufacturer.
     * Each device has a different calbration value. 
     * This calibration value written on the PCB or any information from the manufacturer.
     * 
     * @param pins Clock pins. Pointer to an array with 12 byte width
     * @param power_pin MOSFET gate pin to power up the LED. (PWM Capable)
     * @param pins_unused Threated differently. Pointer to an array with 20 byte width.
     * Make sure they are no floating. All unused pins should be listed on pins_unused. Floating pins means more power consumtion.
     * @note This function should be called directly right after the setup(). 
     * Make sure there are no initialization function that called before this function to guarantee the Operating System run smoothly
     */
    void config(int baudrate, bool preserve_RTC, long calibration_value, int *pins, int power_pin, int button_pin, int *pins_unused);
    

    void logic(uint8_t logic_led, uint8_t logic_led_power, uint8_t logic_button);
    
    /**
     * @brief Reset device from software just like pressing the reset button
     * @param None
     * @return None
     * 
     * @note This function will not triggering the RTC_initialTime() and RTC_initialDate(). It will keep the RTC datetime same as before.
     */
    void reset();

    /**
     * @brief Main program
     * @param None
     * @return None
     * @note 
     * Just keeps the program clean. 
     * This function uses the loop() as part of the main program iteration.
     * Works like usual Arduino-like programming.
     * This function handles various operating system initialization.
     * There are some initialization in the program and some of them must be done sequentially to avoid deadlock.
     * I found it to be less-possible to be left up to the user as it will only add to the confusion. 
     * 
     * 
     * 
     */
    void main();

    /**
     * @brief Simply just shutdown the device keep the WATCHDOG and Real Time Clock (RTC) running
     * @param None
     * @return None
     * @note 
     * Serial communication and Bootloader upload will not workPress the button to wake up the device.
     * To wake up into Bootloader as Serial deviec, hold the button until the watch doing  Lato-lato thins.
     */
    void shutdown();

    /**
     * @brief Enter bootloader as HID device to download the program.
     * @param None
     * @return None
     * @note Once entered, there is no way to exit the bootloader except pressing the reset button or power cycle the device
     */
    void bootloader();

    void BUTTON_begin(unsigned long debouncer_ms, unsigned long shortpress_max_ms);

    /**
     * @brief Fastest way to read the button state
     * @param None
     * @return None
     * @return FALSE on normal state. TRUE if the button is pressed. 
     */
    bool BUTTON_isPressed();

    /**
     * @brief Noise-free button state detection
     * @param delay_ms Debouncer time window in miliseconds
     * @return FALSE on normal state. TRUE if the button is pressed. 
     */
    bool BUTTON_isSafePressed(unsigned long delay_ms);

    /**
     * @brief Detect button behavior. Shorpress and Longpress
     * @param None
     * @return Default is 0 (normal state). 1 for shorpress 1 for longpress.
     * @note 
     * This function only return after the button release AND FORCED to return
     * after CONFIG_BUTTON_SHORTPRESS_MS has been reached.
     */
    BUTTON_state BUTTON_getResult();


    /**
     * @brief Function to lock the loop. Better for debugging application
     * @param None
     * @return None
     * @note 
     * When the function is called, the program will stuck in the function forever.
     * Inidicated by a rondom LED ON and OFF animation.
     */
    void LED_test_deadlock();

    /**
     * @brief Function to lock the loop. Better for debugging application
     * @param None
     * @return None
     * @note 
     * When the function is called, the program will stuck in the function forever.
     * Inidicated by a rondom LED ON and OFF animation.
     */
    void LED_show();
    
    /**
     * @brief Clear means to turn off all of the LED or any clock pin
     * @param None
     * @return None
     */
    void LED_clear();

    /**
     * @brief Toggle the LED state.
     * @param None
     * @return None
     * @note Toggle means to flip the current state. 
     * When the current LED state is ON then the next state it will OFF. When the current state is OFF then it will ON.
     */
    void LED_toggle(int index);

    /**
     * @brief Write or set the LED state.
     * @param None
     * @return None
     */
    void LED_write(int index, bool state);

    /**
     * @brief Write or set the LED state.
     * @param None
     * @return None
     * @note Same as void LED_toggle(int index); 
     */
    void LED_write(int type, int index, bool state);

    /**
     * @brief Set led power behavior
     * @param power power mode. It can be dynamic or maximum. 
     * Dynamic means the LED power consumtion rely on the number of LED that turn on. 
     * It maintain the same LED brightness. Lower better for power consumption.
     * Maximum means using the maximum power even for single LED. 
     * Cannot maintain the same brightness for more than 1 LED. Best for some attention.
     * @return None
     * @note Dynamic only hold until 4 LEDs
     * @see LED_setBrightnessMax
     */
    void LED_setPowerLimit(LED_power power);

    /**
     * @brief Set led power behavior
     * @param percent related to current consumption management on POWER_DYNAMIC in percent. LED_setPowerLimit() should be set in dynamic to use this config.
     * @return None
     * @note Dynamic only hold until 4 LEDs
     */
    void LED_setBrightnessMax(int percent);

    /**
     * @brief Set Real Time Clock (RTC) initial time.
     * @param None
     * @return None
     * @note This function only executed after the device is POWER OFF, RESET, or PROGRAMMED. WAKE_UP will not trigger this function.
     * When the function is not called. The time set after POWER OFF, RESET, or PROGRAMMED is 10:14:30
     */
    void RTC_initialTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

    /**
     * @brief Set Real Time Clock (RTC) initial date.
     * @param None
     * @return None
     * @note This function only executed after the device is POWER OFF, RESET, or PROGRAMMED. WAKE_UP will not trigger this function.
     * When the function is not called. The date set after POWER OFF, RESET, or PROGRAMMED is Aug 17, 2023
     */
    void RTC_initialDate(uint8_t day, uint8_t month, uint8_t year);

    /**
     * @brief Set Real Time Clock (RTC) timer.
     * @param  hours: 0-23
     * @param  minutes: 0-59
     * @param  seconds: 0-59
     * @return None
     * @note This function will executed directly.
     */
    void RTC_setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

    /**
     * @brief Set Real Time Clock (RTC) date.
     * @param  day: 1-31
     * @param  month: 1-12
     * @param  year: 0-99
     * @return None
     * @note This function will executed directly.
     */
    void RTC_setDate(uint8_t day, uint8_t month, uint8_t year);

    /**
     * @brief Adjust or fine tune the Real Time Clock (RTC) calibration value.
     * @param diff Fine tune adjustment for the user.
     * @return None
     * @note Already set in the config(). 
     * You could fine tune the value just by increasing and decreasing the diff.
     */
    void RTC_fineTune(long diff);

    /**
     * @brief Receive the datetime update from the Real Time Clock (RTC).
     * @param None
     * @return None
     * @note This function should be called before calling any RTC get function i.e RTC_getHour(), RTC_getMinute(), RTC_getSecond().
     * 
     */
    void RTC_update();

    /**
     * @brief Get the HOUR update from the Real Time Clock (RTC). Hour in 0-23 Format.
     * @param None
     * @return None
     * @note RTC_update() should be called before to get the newest or the updated value from the RTC.
     * 
     */
    int RTC_getHour();

    /**
     * @brief Get the MINUTE update from the Real Time Clock (RTC). Hour in 0-59 Format.
     * @param None
     * @return None
     * @note RTC_update() should be called before to get the newest or the updated value from the RTC.
     * 
     */
    int RTC_getMinute();

    /**
     * @brief Get the SECOND update from the Real Time Clock (RTC). Hour in 0-59 Format.
     * @param None
     * @return None
     * @note RTC_update() should be called before to get the newest or the updated value from the RTC.
     * 
     */
    int RTC_getSecond();

    /**
     * @brief Get the DAY update from the Real Time Clock (RTC). Hour in 1-31 Format.
     * @param None
     * @return None
     * @note Actually this function is not make any sense because this only a small watch system.
     * This exist just in case you have enough creativity to implement that >_<
     * RTC_update() should be called before to get the newest or the updated value from the RTC.
     * 
     */
    int RTC_getDay();

    /**
     * @brief Get the MONTH update from the Real Time Clock (RTC). Hour in 1-12 Format.
     * @param None
     * @return None
     * @note Actually this function is not make any sense because this only a small watch system.
     * This exist just in case you have enough creativity to implement that >_<
     * RTC_update() should be called before to get the newest or the updated value from the RTC.
     * 
     */
    int RTC_getMonth();

    /**
     * @brief Get the YEAR update from the Real Time Clock (RTC). Hour in 0-99 Format. Set to 23 means 2023
     * @param None
     * @return None
     * @note Actually this function is not make any sense because this only a small watch system.
     * This exist just in case you have enough creativity to implement that >_<
     * RTC_update() should be called before to get the newest or the updated value from the RTC.
     * 
     */
    int RTC_getYear();

    /**
     * @brief Write value into register.
     * @param None
     * @return None
     * @note This function will keep the value on the register even the device is shutdown or reset. Its not a permanet storage like EEPROM.
     * It will LOST the value when the device is power off.
     * The purpose is to keep the stored value exist in the register. So, 
     * The write process is only executed right before the execution of WhatchOS.shutdown() or WhatchOS.reset() Process.
     * When these two function is not called. The value will never to ve written into the register.
     */
    void REGISTER_write(uint32_t BackupRegister, uint32_t Data);
    
    /**
     * @brief Read value from the register directly.
     * @param location Memory location. It can be the RUNTIME or REGISTER
     * @return uint32_t
     * @note The returned value is the value that stored in the selector location of the register. 
     * RUNTIME represents the value update from the REGISTER_write(). BEWARE! It is not always repesents the value stored in the register.
     * SOURCE represent the value from the register itselft.
     * Please remember, REGISTER_write() is only executed just before the device shutdhown or reset.
     * 
     */
    uint32_t REGISTER_read(uint32_t BackupRegister, REGISTER_location location);


    /**
     * @brief Set the WATCHDOG routine. For example the device is shutdown it will wake up the device base on the interval.
     * @param interval Represents the routine interval.
     * @param forced_on Set the register ON. Usefull to make sure the register is SET on the first time.
     * @return None
     * @note This is the function to create a standby LED. Basically this function tells the WATCHDOG to wake from shutdown state.
     * When the device is waked up by WATCHDOG routing it will enter the loop and execute program normaly.
     * @see shutdown()
     * 
     */
    void WATCHDOG_setRoutine(WATCHDOG_wake_interval interval, bool forced_on);
    
    /**
     * @brief Check if the wake up source comes from WATCHDDOG. For example the device is shutdown it will wake up the device base on the interval.
     * @param interval Represents the routine interval.
     * @return bool. TRUE if the source is the WATCHDOG routine
     * @note 
     * This function may differentciate the wake up source comes from the WATCHDOG or the user BUTTON.
     * This flag will return TRUE if the wake up source is from WATCHDOG itself and return FALSE for the other wake up source.
     * 
     * NORMALY this flag is combined with shutdown to create a standby LED. Basically it wakes up, light up the LED, then finally shutshown to save the power.
     * @see shutdown()
     * 
     */
    bool WATCHDOG_isWakeUp();

    bool WATCHDOG_isInterruptFlag();
    void WATCHDOG_clearInterruptFlag();

    unsigned long getUID(int index);
  private:
    
    // Config
    int *pLED_pins;
    int *pLED_pins_unused;
    int LED_power_pin;
    int BUTTON_pin;
    int RTC_calibration_value;

    void GPIO_begin();
    void GPIO_sleep();

    // LED
    int LED_pins_state[12] = {0};
    LED_power LED_power_config = POWER_DYNAMIC;
    int LED_brightness_max = 50;
    
    void LED_begin();
    void LED_analog(int percent);
    void LED_dynamic();
    void LED_activate();
    void LED_deactivate();

    // BUTTON_isSafePressed
    bool BUTTON_state_last = false;
    bool BUTTON_enable = true;
    // BUTTON_getResult
    int BUTTON_state = 0;
    unsigned long BUTTON_millis = 0;
    unsigned long BUTTON_press_millis = 0;
    int BUTTON_result = 0;


    uint32_t REGISTER_location[10] = {0};
    void REGISTER_begin();

    int RTC_register[6] = {0};
    bool RTC_preserved = false;
    // [0] Hour 
    // [1] Minute
    // [2] Second
    // [3] Day 
    // [4] Month 
    // [5] Year
    void RTC_begin();
    bool RTC_isInitialRun();

    void POWER_begin();

    int WATCHDOG_latolato_counter = 0;
    bool WATCHDOG_latolato_called = false;
    bool WATCHDOG_setRoutine_called = false;
    bool WATCHDOG_wake_isRunning = false;
    WATCHDOG_wake_interval WATCHDOG_interval;
    int WATCHDOG_encrypted_a = 0;
    int WATCHDOG_LED[12] = {0};
    unsigned long WATCHDOG_millis;
    static volatile bool WATCHDOG_elapsed_interrupt_flag;
    void WATCHDOG_begin();
    void WATCHDOG_disableInterrupt();
    static void ISR_WATCHDOG_handler (void *data);
};

extern TQDF_WatchOS WatchOS;

#endif