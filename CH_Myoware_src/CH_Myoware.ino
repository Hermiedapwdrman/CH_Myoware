    /**
  
    CH_Myoware: A simple program which reads and analog value from the output
    of a Myoware EMG sensor; compares it to a threshold value; and then initiates
    a keypress based on the users desire.

    Copyright (C) 2018 Patrick Wagner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    **/

#include <Mouse.h>
#include <Keyboard.h>

/** This pin when connected to GND will enable the keyboard output
 * WARNING: This will print characters to your computer when active **/
#define ENABLE_PIN 12

/** This analog pin Ax is connected to the SIG output from the Myoware. **/
#define MYO_READ_PIN A0

/** This is the number number of samples read from the myo output per second.
 * **/
#define MYO_SAMPLES_SEC 50 

/** The threshold value for activating a positive muscle activation
 *  Possible values are 0 - 1023
 * 
 *  Note: If you are powering the Myoware board with a 3.3V source, the maximum reading is 633.
 *      If you are using 5V, the maximum reading is 972.
 *      Theshold values larger than the maximum will gauruntee there will never be an activation. So DONT do that.
 *  **/
unsigned int myo_threshold = 300;  

/** The character or mouse click to send to the computer when an activation occurs. 
 *  
 * Note: You can disable either the character or the left click by changing respective var to 'false'
 *  * **/
bool enable_left_click = true;  // 'true' will send a left mouse click on activation, 'false' will not.
bool enable_keyboard_char = true;  // 'true' will send the char below on activation, 'false' will not.
char positive_test_letter = 'x';  //Printed character should be between single quotes, Ex: Space = ' ' ; X = 'X' etc. 

/**Number of milliseconds to delay before allowing another positive reading, known as DEBOUNCE **/
unsigned int debounce_ms = 500;

/** Testing variable to blink the led to act as a heartbeat, making sure the program is running. **/
bool led_toggle = 0;


/**NO TOUCHY! Below variables are calculated to allow for control flow and to simplify some loop() programming **/
unsigned int debounce_counter = 0;
unsigned int debounce_incycles = debounce_ms/MYO_SAMPLES_SEC;
unsigned int delay_counter_ms = 1000/MYO_SAMPLES_SEC;


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
    delay(delay_counter_ms); // Make sure to only read MYO_SAMPLES_SEC times a second
    digitalWrite(13, ~led_toggle);
 
    int enable_keyboard_mouse = !digitalRead(ENABLE_PIN);  //If the enable pin is connected to ground, then print keyboard functions.
    int myo_value = analogRead(MYO_READ_PIN); //Read the value of the myo output.  Note: value is 0-1024.

    //NOTE: This will work with the arduino grapher tool.
    Serial.print(myo_value); //Test print out the value read by the myo, comment out of release code but useful for debugging.

    //If the value measured is greater than the threshold, then execute the following code.
    if(myo_value > myo_threshold){

        //If the keyboard enable pin is low, this will print the desired key to the screen
        //    Note: Only occurs once per debounce_ms milliseconds.
        if(enable_keyboard_mouse && (debounce_counter >= debounce_incycles)){
                
                if (enable_keyboard_char == true){  //If the keyboard character is not defined,
                    Keyboard.write(positive_test_letter);
                    Keyboard.write(KEY_RETURN);  //TEST Return to increase readability.
                }

                if (enable_left_click == true){
                    Mouse.click(MOUSE_LEFT);
                }

                debounce_counter = 0;  //Reset debounce counter.
        }
            //Code here will execute when the threshold is measured, regardless if the keyboard enable is active.
            Serial.print(F("\t\t\tACT!"));  //Test print on activation, comment out of release code.
    }
    
    //This is a once per loop counter which increments debounce_counter, marking time before the next event can happen.
    if (enable_keyboard_mouse){ 
        debounce_counter++;
    }

    Serial.print('\r'); //New line
    
}//END OF loop()
 
