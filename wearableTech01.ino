#include <Adafruit_CircuitPlayground.h>
#include "Keyboard.h"

// Connecting the arrow keys with each pin 
int upButton = A2;       //Up arrow key with A2
int downButton = A3;     //Down arrow key with A3
int leftButton = A4;     //Left arrow key with A4
int rightButton = A5;    //Right arrow key with A5

void setup() {
    Serial.begin(9600);
    CircuitPlayground.begin();

    //Setting up buttons 
    pinMode(upButton, INPUT_PULLUP);
    pinMode(downButton, INPUT_PULLUP);
    pinMode(leftButton, INPUT_PULLUP);
    pinMode(rightButton, INPUT_PULLUP);
    
    Keyboard.begin();
}

void loop() {

  CircuitPlayground.clearPixels();
  Keyboard.releaseAll();

    //Making sure if the buttons are pressed 
    if (digitalRead(upButton) == LOW) {
        Keyboard.press(KEY_UP_ARROW);     //Pressing the key 
        delay(100);                       //Adding delay to avoid multiple presses
        Keyboard.release(KEY_UP_ARROW);   //Releasing the key after the delay 
    }
    if (digitalRead(downButton) == LOW) {
        Keyboard.press(KEY_DOWN_ARROW);
        delay(100);
        Keyboard.release(KEY_DOWN_ARROW);
    }
    if (digitalRead(leftButton) == LOW) {
        Keyboard.press(KEY_LEFT_ARROW);
        delay(100);
        Keyboard.release(KEY_LEFT_ARROW);
    }
    if (digitalRead(rightButton) == LOW) {
        Keyboard.press(KEY_RIGHT_ARROW);
        delay(100);
        Keyboard.release(KEY_RIGHT_ARROW);
    }
      Serial.println(); //For serial Monitor-debug
}
