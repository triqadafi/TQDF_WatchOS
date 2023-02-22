#include <Arduino.h>
/*  
 * EXAMPLE: Native Arduino
 *
 * The program below just same demontration about led blink
 * You can use any arduino related function
 * 
 * But sometimes you need an adaption. Because of the hardware specification.
 * In this example the LED will turn on when the voltage level is LOW
 * And turned OFF when the voltage level is HIGH
 * 
 */

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED ON by making the voltage LOW
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED OFF
  delay(1000);                      // wait for a second
}