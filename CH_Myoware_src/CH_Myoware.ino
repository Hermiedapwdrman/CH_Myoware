/**
MIT License

Copyright (c) 2018 Patrick W.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#include <Mouse.h>
#include <Keyboard.h>

/** This pin when connected to GND will enable the keyboard output
 * WARNING: This will print characters to your computer when active **/
#define ENABLE_PIN 12

/** This analog pin Ax is connected to the SIG output from the Myoware. **/
#define MYO_READ_PIN A0

/** This is the number of millisecond between each myo measurement 
 *  You can calculate the samples per second by:
 *      Samples/sec = 1000 / LOOP_DELAY_MS
 * **/
#define LOOP_DELAY_MS 10 

/** The threshold value for activating a positive muscle activation
 *  Possible values are 0 - 1023
 * 
 *  Note: If you are powering the Myoware board with a 3.3V source, the maximum reading is 633.
 *      If you are using 5V, the maximum reading is 972.
 *      Theshold values larger than the maximum will gauruntee there will never be an activation. So DONT do that.
 *  **/
unsigned int myo_threshold = 300;  

/** The character to send to the computer when an activation occurs. **/
char positive_test_letter = 'x';  //Printed character should be between single quotes, Ex: Space = ' ' ; X = 'X' etc. 

/**Number of milliseconds to delay before allowing another positive reading, known as DEBOUNCE **/
unsigned int debounce_ms = 500;

/** Testing variable to blink the led to act as a heartbeat, making sure the program is running. **/
bool led_toggle = 0;


/**NO TOUCHY! Below variables are calculated to allow for control flow and to simplify some loop() programming **/
unsigned int debounce_counter = 0;
unsigned int debounce_incycles = debounce_ms/LOOP_DELAY_MS;


//////// Arduino Library Functions ///////////

/** The setup function runs once when the leonardo leaves the bootloader ~7 seconds after turning on the device **/
void setup() {
  pinMode(ENABLE_PIN,INPUT_PULLUP);     //Configure pin to enable keyboard output
  pinMode(LED_BUILTIN,OUTPUT);          //Configure LED output pin.
  Keyboard.begin();                     //Initialize the keyboard class.
  Mouse.begin();                        //Initialize the mouse class.
  Serial.begin(115200);                 //Start the serial communication at 115200 bps.
}  //END of setup()
 
/**  The loop function runs from top to bottom as quickly as it can for whole duration of the program execution.
 *   
 *  Note: The delay function call at the top of code block acts as a pause to make sure we take x samples per second and no more.
 * 
 * **/
void loop() {
    delay(LOOP_DELAY_MS); // Make sure to only read 1000/LOOP_DELAY_MS times a second
    digitalWrite(13, ~led_toggle);
 
    int enable_keyboard = !digitalRead(ENABLE_PIN);  //If the enable pin is connected to ground, then print keyboard functions.
    int myo_value = analogRead(MYO_READ_PIN); //Read the value of the myo output.  Note: value is 0-1024.

    //NOTE: This will work with the arduino grapher tool.
    Serial.println(myo_value); //Test print out the value read by the myo, comment out of release code but useful for debugging.

    //If the value measured is greater than the threshold, then execute the following code.
    if(myo_value > myo_threshold){

        //If the keyboard enable pin is low, this will print the desired key to the screen
        //    Note: Only occurs once per debounce_ms milliseconds.
        if(enable_keyboard && (debounce_counter >= debounce_incycles)){
                Keyboard.write(positive_test_letter);
                Keyboard.write(KEY_RETURN);  //TEST Return to increase readability.
                debounce_counter = 0;
        }
            //Code here will execute when the threshold is measured, regardless if the keyboard enable is active.
            Serial.print(F("ACT!"));  //Test print on activation, comment out of release code.
    }
    
    //This is a once per loop counter which increments debounce_counter, marking time before the next event can happen.
    if (enable_keyboard){ 
        debounce_counter++;
    }
    
}//END OF loop()
 
