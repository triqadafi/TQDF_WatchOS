<!-- src=" -->
# Getting Started

[TOC]
<b>`AEONF_mk6`</b> is short of digital watch with a 12-bit LED arranged to act as an analog watch. It's actually a development board, but it serves the purpose of performing like ***"an ordinary wristwatch"***. The device is `Arduino compatible board`. This means, the firmware can be reprogrammed by Arduino IDE. Most of the process follows the Arduino IDE standard.

## Are you sure it can be used as a normal watch?
**Yes**, of course, it meets the standard of a wristwatch. For a more detailed story, check out this [Instagram post](https://www.instagram.com/p/Cn9BrDTrsU-/?utm_source=ig_web_copy_link).
Here is some summary:
 - Ofcourse it can work as normal wristwatch. 🥳
 - The device can do ***Lato-lato*** things. 😁
 - This watch is **built with a custom bootloader**. In simple words, you can reprogram it just by connecting it to the USB port 😮
 - **Optimized deep sleep**. It can reach lower than 1 mA!!! It never dies. 😴
 - The RTC is **calibrated with Rubidium Oscillator**. It can reach **10-30 seconds/month!** 😎
 - It has a watchdog! 🐺 😈 🤖 👽
 - Standby LED for more attraction!

 <!-- 
 - [v] Beauty shot
 - [] Open Source with Arduino IDE
 - [v] Standby Mode
 - [] Original design
 - [] Micro USB Port
 - [] Easy to maintenance
 - [] Hardcore programing with ST-Link
 - [v] Calibrated
 - -->

That such amazing result is packed in this `WatchOS` library. This is a kind of Operating System library used by `AEONF_mk6` to accelerate the development process. It manages various initialization and functions required to develop the "Watch".

## Wait, Do I Need to develop the software by myself?
Back in the day, when I started developing this device, The main goal was to  "acquaint" with the hardware programming environment, i.e., Arduino IDE. Now the plan remains the same. You must try to... or at least do "Upload the program". Believe me, there are just some simple steps you need to understand. 

Take some advantages for example, It is possible to modify any part of the entire code. Like making simple delay adjustments or modifying the LED animation!. Actually, I could make a simple method to update the firmware, but I choose to not do that.

> **In simple words**, all of the full implementations are available in the Example folder. In case there is a firmware update, just load the Example, and you can keep up with current development.
<hr/>

# Compatible Hardware
In this period, the Library only supported the `AEONF_mark_6`.

## AEONF mark 6


Top Side  | Back Side  | <div style="width:150px">Legend</div>
------------- | ------------- | -------------
<img style="filter: none;" src="hardware_top.jpg" alt="Hardware Top">  | <img style="filter: none;" src="hardware_bottom.jpg" alt="Hardware Bottom">  | 1. MCU <br/>2. LED<br/> 3. Test<br/> 4. Test<br/><br/><br/>

Specification:
- Case: Acrylic 
- Case Diameter: `38mm` (green line)
- Strap Width: `22mm` (yellow line)
  - The watch is not shipped with the strap. It is up to the user. 
  - Recommended: Rubber-type watch strap.

Electronics:
- PCB Marking: `AEONF_6`
- MCU: `STM32f401CC` (48-UFQFPN) ARM® Cortex®-M4
- Voltage Supply: 2.5V ~ 3.6V (Coin cell CR2032)
- LED Display: 12-bit Standard 0805 Single Color LED
- Programming Interface: `USB` (MicroUSB)
- Button: 3 Button `User`, `Boot`, `Reset`
- RTC Calibration: `YES` (Standard Rubidium Oscilator)
- Bootloader: `YES` (HID/Serial Bootloader)

Firmware:
- Core: `TQDF_Core`
- Operating System: `TQDF_WatchOS`
- Board Name: Watch AEONF mk6 `watch_aeonf_mk6`
- Exposed `UART1` (TX `PA9`/RX `PA10`)
- SWD Programming `enabled` (for backup and recovery)

<hr/>

# First things to Check
The 3 things below are the recommended steps to try before we are going further. If there are failures, please let me know. Just do it, it takes few seconds.
## Powering up the device
1. Open the back cover
2. Insert the coin cell battery (beware of the polarity)

## Entering the Bootloader
1. Connect the device to any PC by using MicroUSB
2. Hold the button until the device is doing Lato-lato things.
3. Keep holding the button!
4. Go to Device Manager
5. Make sure the Serial COM port appear on the PC.
6. Release if you feel tired 😅

## Configuring the device
1. If you interested in firmware development, please follow [Get Ready to Upload the Program](#get-ready-to-upload-the-program)
2. If not please continue below.

<hr/>

# How to read the Time?
- Just press the button.
- The LEDs will turn up with the following sequences

## MODE 0: The standard
The time follows 12 hour format `HH:MM`. Here is the example showing `10:18`. I think, this is pretty intuitive, try to understand figure it out **without reading** the detailed step below.


<!-- https://www.oliverboorman.biz/projects/tools/clocks.php -->

AEONF mk6  | Analog Watch
------------- | -------------
<img style="filter: none;" src="watch_animated_no_second.gif" alt="Current Device" width="300">  | <img src="clock_example.png" alt="Current Device" width="175">

Easy isn't it? Here is the detailed step:

<div style="width:250px">First LED</div> | <div style="width:250px">Second LED</div> | <div style="width:250px">Third LED (Sqeuence)</div>
------------- | ------------- | -------------
<center><img style="filter: none;" src="watch_normal_step1.jpg" alt="Current Device" width="200"></center> | <center><img style="filter: none;" src="watch_normal_step2.jpg" alt="Current Device" width="200"></center>  | <center><img style="filter: none;" src="watch_normal_step3.jpg" alt="Current Device" width="200"></center>
First LED represents the HOUR. <br/><br/><br/><br/><br/><br/>Read as `10`. So it is `10:MM`<br/>. | Second LED represents the MINUTE. To be clear, it's actually **represent the clock long needle**. So the value shown is a **multiple of 5**. <br/><br/>Read as `3` then become `3*5=15`. So it is `10:15`. | The next LED sequence is to show the minute accuracy. There are 3 extra steps after the MINUTE LED. So the time is `10:15` <b>`+3`</b>.<br/><br/><br/><br/>

<b><center>The result it `10:18`</center></b>


<!-- ## MODE 2: The Decimal -->
<!-- Decimal Mode still on development. -->
<!-- First LED  | Second LED | Third LED -->
<!-- ------------- | ------------- | ------------- -->
<!-- <img src="https://i.imgur.com/pCiKton.png" width="280">  | <img src="https://i.imgur.com/5aKLRp0.png" width="280">  | <img src="https://i.imgur.com/679DBWF.png" width="280">
Content Cell  | Content Cell  | Content Cell -->
## MODE 1: Decimal Easy

Coming Soon!


## MODE 2: Decimal Fast

Coming Soon!

<hr/>

# Adjusting the time?

## The button modes

There are two button modes. They used to configure the time.
  - `short press` = the button is pressed less than 1 second
  - `long press` = the button is pressed and held for more than 1 second

The intuitive  way is 
  - `short-press` to adjust the value
  - `long-press` to confirm. 

## Menu entry point
Menu entry point is located right after the watch is diplaying the time.

1. So **press the button** to show the time. 
2. **While displaying the time** just **press and hold** the button.
3. **Wait** until all LEDs turn on.
4. **Release the button** after first LEDs blink.
5. Now you are in the configuration menu (Indicated by LED numbe 1 turn on).
6. Short-press to select menu 1-5. For time configuration **select LED number 1**.
7. Long-press to confirm

Other configuration menu are listed below.
Number of Blink  | Menu Section
------------- | -------------
1 | Time configuration menu
2 | Time DEBUG mode
3 | Bootloader mode
4 | N/A
5 | N/A


## Time configuration menu

The configuration is divided into 4 section:
  - `Hour` configuration
  - `Minute` configuration
  - `Minute Accuracy` configuration

About the `Second`...
> Actually there is a configuration to enable the `Second`. It DISABLED by default. It has been implemented on the software. It can be enable by using define macros #USE_SECOND.Second` is "dynamic", If we are pursuing seconds accuracy. It need to follow the detailed step. On this early version we simply ignore it.

## Preparing the parameters:
Make sure you have enough time, before configuring the target time. The watch is configured the same way as how the time is displayed.
We want to set the target time is `8:22` so the parameters is
- `Hour`: `8`
- `Minute`: `4` (22 divided by 5 is 4 the remainder is 2)
- `Minute Accuracy`: `2`

Remember, the time configuration menu order is `Hour > Minute > Minute Accuracy`.
<!-- - Second` is "dynamic"` -->



## Here are the detailed steps:

Hour configuration
1. Short press to adjust the `Hour` value
2. Long press to confirm and continue to `Minute` configuration. 

Minute configuration

3. Sort press to adjust the `Minute` value
4. Long press to confirm and continue to `Minute Accuracy` configuration

Minute Accuracy configuration

5. Sort press to adjust the `Minute Accuracy` value
6. Long press to confirm and finish the configuration

<!-- Second configuration.
Basically you need to match the real time second with
The second value is the value displayed multiplied by 5. Example: 4 is 20 second, 9 is 45 second.

7. Sort press to adjust the `Second` value. 
8. Long press to confirm and continue to `Second` configuration -->


<hr/>

# Firmware Development
Before we are getting deeper into the implementation. We need to understand some terms that use in this project. 
- `Firmware` - refers to the program developed by the user. It can be programmed into the device.
- `Core` - means the low-level program. It's like a foundation that supports the user `firmware`. `Core` makes the `Firmware` to be developed far lot easier.

## Firmware Pakage
Here some quick introduction to bring you a lot more familiar with the term 
- `Bootloader` is a small piece of firmware that allows to transfer the sketches program into this device. It is driverless easy as connecting the USB. You don't need to worry much about it. This device is shipped with the pre-installed bootloader. Still, you need to understand [How to enter the bootloader](#how-to-enter-the-bootloader) in case "you need to develop" or "there is" a firmware update.
- `TQDFCore` Is a core package developed to work with a specific microcontroller, for example, the STM32 MCU that is used in this device.
This core was built to work best with its custom bootloader.
- `TQDF_WatchOS` is a miniature of the "Operating System" packed as Library. Like the other Library, the purpose is to accelerate the development time with predefine functions, API, and various initialization processes. It was explicitly built only for this device. These functions, features, and different examples are well-documented. Check it out!

I guess, you stats to become familiar with the terms, Let's gooo!

<hr/>

# Get Ready to Upload the Program

If you haven't done so already, download Arduino IDE from [the software page](https://www.arduino.cc/en/software). You can find [installation instructions here](https://support.arduino.cc/hc/en-us/articles/360019833020). If you already have Arduino IDE installed, uploading a sketch is quick and easy, but let's take a look at what we need to do in just a few minutes.
1. You need to Setting Up the Arduino IDE
    - [Installing Core](#installing-core-via-board-manager)
    - [Installing Library](#installing-library)
2. Then you can do the [Upload](#lets-upload-some-example) process

## Installing Core (via Board Manager)
This installation method requires Arduino IDE version 1.6.4 or greater.
* Open the Arduino IDE.
* Open the **File > Preferences** menu item.
* Enter the following URL in **Additional Boards Manager URLs**:

    ```
    https://triqadafi.github.io/tqdfcore/package_tqdfcore_index.json
    ```

* Open the **Tools > Board > Boards Manager...** menu item.
* Wait for the platform indexes to finish downloading.
* Search for **TRQDFCore_STM32_ArduinoCore** and click on it.
* Click **Install**.
* After installation is complete close the **Boards Manager** window.

> By installing this core, you can run any Arduino-like program or some STM32duino program. You can start to build your own firmware. But wait, we may use the library to make the development super fast. 

## Installing Library

The library can be installed using the [standard Arduino library install procedure](http://arduino.cc/en/Guide/Libraries)

**OR** here are some simple direction

- Save the [.zip file](https://github.com/triqadafi/Triqadafi_FrequencyCounter/releases/download/v1.0/Triqadafi_FrequencyCounter-1.0.zip) to your desktop,
- Open Arduino IDE
- Go to Menu > `Sketch > Include Library > Add .ZIP library...`
- Run some [example files](https://github.com/triqadafi/Triqadafi_FrequencyCounter/tree/master/examples) if you wish.

And we are ready to program some example.

<hr/>

## Let's Upload some Example

1. Write a sketch, or use an Example such as Blink (**File > Examples > 01.Basics > Blink**).
2. Connect the device to any PC by using MicroUSB.
3. **Enter the bootloader** by holding the button until the device is doing Lato-lato things.
4. Serial COM port will appear on the PC.
5. Select Board and the related COM Port.
6. Click the **Upload** button to program the board with the sketch.
7. Wait the process.

You're done! It's Easy, isn't it?
Once you complete these steps, you don't need to re-do the Core and Library installation. Just select the board, then hit the upload button.

<hr/>


# Danger Zone!

The section below mostly about the function documentation. Feel free to access the. If you are not brave enough better just leave it 😅

<hr/>

# The Development

The explanation below brings you deep into the system, as deep as the hardware configuration. If you are not interested in developing the firmware, you are safe to ignore it.


## The Operating System
The `WatchOS` is a `TQDF_WatchOS` C++ Class, which works like low-level firmware. It handles all of the peripherals required by the device to operate smoothly. Any function members can be called through the `WatchOS` variable in any part of the code. The WatchOS is configured in this way.

Any hardware-related configurations are declared in `config_<board_name>.h`. For example, `config_AEONF_mk6.h` is a hardware config for AEONF_mk6 PCB.

The main *.ino file follows the Arduino standard with `setup()` and `loop()`. The user program should be written on between the comment provided. Besides `setup()` and `loop()`, two `TQDF_WatchOS` functions must exist in the program. They are for the hardware configuration and to run the main program.
- `TQDF_WatchOS.config()`  is used to configure the hardware-related configuration. The WatchOS needs to know this information to work with specific hardware. It also can be configured by the user.
- `TQDF_WatchOS.run()` is used to handle the main program execution. It regulates the `loop()`. This function never returns. 

> If this function does not exist in `setup()`, simply, you are not using the WatchOS. It will run as a regular Arduino-like program. No device initialization will be performed, such as configuring the pin and RTC.


## LED
LED works as an indicator. It is the only way to display the time. The LEDs are arranged in a circle, resembling the digits on an analog clock.

To keep the minimal power, the LED's cathode is tied together and powered with a separate source. This power source is controlled by a pin labelled `PIN_LED_POWER`.
Especially in this board, two steps are required to turn on a single LED.
First, the power pin needs to be powered, and then set the led pin as a current sink. Seems complicated, but it has been handled by the TQDF_WatchOS. Just use `LED_write()` or `LED_toggle()` to play with the LED. Even the LED brightness can be set using `LED_setBrightness()`.

## Button
In simple terms, the button will connect two points in a circuit when pressed. This behaviour results in 2 different states. LOW on normal and HIGH when pressed. This behaviour is detected by `BUTTON_isPressed()`. Some advanced features have been implemented by `WatchOS`. from this simple behaviour. 
- `BUTTON_isSafePressed()` to safely detect the button was pressed.
- `BUTTON_getResult()` to detect short and long presses.

## Real Time Clock (RTC)
RTC contains the standard function to read and write the date and time. RTCs must accurately keep time, even when the device is not operating. The RTC on this device has been calibrated with a standard Rubidium Oscillator, so they must have a specific calibration value written on the PCB. Besides, the user could do the fine-tune calibration by calling the `RTC_fineTune()`. 

Function `RTC_initialTime()` is only executed on the first time the device is powered up (Reset or wake up will not trigger this function. While the other `RTC_setTime()` is executed directly.
To read the time the `RTC_update()` should be called first before calling `RTC_getHour()`, `RTC_getMinute()`, `RTC_getSecond()`.


# Advanced Peripheral
Most of the time, the functions below are optional. But, accessing these functions opens up the possibility of adjusting or tweaking the operating system. You may use this function or just leave it to the `WatchOS` to manage them. 

## Register
The purpose is to keep some value while the watch enters a `shutdown` state. The value is saved on the register until the device is fully turned off or out of power. This is useful in the wake-up process to read some variables stored from the previous state. These write and read processes are done by `REGISTER_write()` or `REGISTER_read()`.

## Watchdog
The Watchdog is responsible for monitoring every process running in the watch. Make sure every process follows the rules. The rules are to optimize power consumption. 
For example, keeping the watch always in shutdown mode when there is no activity.

The Watchdog has a special rule. It may wake up the device without any user intervention. It's also possible to wake up the device for every period to create a standby LED animation. Believe me, It is a really cool feature! 

<hr/>

# Bootloader
The Bootloader used was specifically built for the **TQDFCore** and **WatchOS**. It may have a different standard compared with the usual STM32 Bootloader. It's reliable and versatile. I always found it to succeed. So **I need your help to see it fail** 😝

## How to enter the Bootloader?
1. Make sure WatchOS is implemented on the device.
2. Connect the device to any PC by using MicroUSB
3. Hold the button until the device is doing Lato-lato things.
4. Serial COM port will appear on the PC.
5. Upload program just like usual day using Arduino IDE (Make sure to configure the COM Port correctly)

<hr/>

# Frequently Asked Question (FAQ)

## How to recover the "bricked" device?
On paper, there is no way you can brick the device when using this custom bootloader, but who knows? There is always a possibility. Contact me for some workaround to fix this. There is a way, but it still needs to be documented.



<hr/>

# Other Things
- My Instagram: [@triqadafi](http://instagram.com/triqadafi)
- My Website: [triqada.fi](http://triqada.fi)

<hr/>
<!-- https://unicode.org/emoji/charts/full-emoji-list.html -->
