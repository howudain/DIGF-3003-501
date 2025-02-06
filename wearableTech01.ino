#include <Adafruit_CircuitPlayground.h>
#include "Keyboard.h"

// Connecting the arrow keys with each pin 
int upButton = A2;   
int downButton = A3;  
int leftButton = A4; 
int rightButton = A5;

void setup() {
    Serial.begin(9600);
    CircuitPlayground.begin();

    pinMode(upButton, INPUT_PULLUP);
    pinMode(downButton, INPUT_PULLUP);
    pinMode(leftButton, INPUT_PULLUP);
    pinMode(rightButton, INPUT_PULLUP);
    
    Keyboard.begin();
}

void loop() {
  
  CircuitPlayground.clearPixels();
  Keyboard.releaseAll();

    if (digitalRead(upButton) == LOW) {
        Keyboard.press(KEY_UP_ARROW);
        delay(100);
        Keyboard.release(KEY_UP_ARROW);
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
      Serial.println();
}