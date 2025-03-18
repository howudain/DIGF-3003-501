/*
Voicelume Controller 
Dain Jung
DIGF-3003-501 Wearable Technology
OCAD University
March 9th 2025
*/ 

#include <Adafruit_CircuitPlayground.h>

#define SAMPLES 64
#define ACCEPTABLE_RANGE 3 
//Setting the acceptable decibel range +-3 
#define RESPONSE_INTERVAL 200 

int wearerButton = A2;  // Button for wearer's decibel detection
int partnerButton = A3;  // Button for partner's decibel detection
int powerButton = A4;  // Power button
int buzzer = A5; // Setting the buzzer 
int microphone = A1;   // Setting the microphone

const float DEFAULT_SPEECH_LEVEL = 50;       //Setting the wearer's expected decibel 
const float DEFAULT_PARTNER_SPEECH_LEVEL = 55; //Setting the partner's expected decibel

bool powerOn = true; // If the power's on 
int mode = 0; // Making the mode selection available 

//Setting the melody for the wearer - when the volume is too low 
int melody[] = {300, 300, 444, 444, 500, 500, 440}; // Setting up the note 
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4};        //Duration for each melody 

//Setting the melody for the wearer - when the volume is too high
int loudWarningMelody[] = {800, 600, 500, 400, 300}; //" " " "
int loudNoteDurations[] = {4, 4, 4, 4, 4};           //" " " "

void setup() {
  CircuitPlayground.begin();
  pinMode(wearerButton, INPUT_PULLUP);
  pinMode(partnerButton, INPUT_PULLUP);
  pinMode(powerButton, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  Serial.println("Voice Feedback System Ready..."); // For serial Monitor
}

void loop() {
  if (digitalRead(powerButton) == LOW) { // If the power button is pressed
    delay(200); // Pause to prevent multiple toggles
    while(digitalRead(powerButton) == LOW);
    powerOn = !powerOn; // Toggle power state
    Serial.println(powerOn ? "Device ON" : "Device OFF"); // For serial Monitor
    if (!powerOn) { // If the device is off
      CircuitPlayground.clearPixels(); // Clear LEDs 
      noTone(buzzer); // Clear sounds from buzzer 
    }
  }
  
  if (!powerOn) { // If the device is off
    delay(RESPONSE_INTERVAL); // Pause 
    return; // Exit the loop 
  }

  if (digitalRead(wearerButton) == LOW) { // If the button for wearer is pressed 
    delay(200); // Pause
    while (digitalRead(wearerButton) == LOW);
    mode = 1; // Mode 1 turns on 
    Serial.println("Wearer speaking");
    CircuitPlayground.clearPixels(); // Clear LEDs 
    noTone(buzzer); // Clear sounds 
  }
  if (digitalRead(partnerButton) == LOW) { // If the button for the partner is pressed 
    delay(200); // Pause
    while (digitalRead(partnerButton) == LOW);
    mode = 2; // Mode 2 turns on 
    Serial.println("Partner speaking");
    CircuitPlayground.clearPixels(); // Clear LEDs
    noTone(buzzer); // Clear sounds 
  }

  if (mode == 1) {  // Setting button for wearer 
    float decibelCurrent = getDecibelLevel(); // Getting the decibel level through microphone
    Serial.print("Wearer's decibel: ");
    Serial.println(decibelCurrent);
    
    if (decibelCurrent > DEFAULT_SPEECH_LEVEL + ACCEPTABLE_RANGE) { // If the currenct decibel is louder
      Serial.println("Too loud! Playing the warning melody");
      playLoudWarningMelody(); // Plays the warning melody 
    } else if (decibelCurrent < DEFAULT_SPEECH_LEVEL - ACCEPTABLE_RANGE) { // If the decibel's smaller
      Serial.println("Too small! Playing the melody");
      playMelody(); // Plays the melody
    } else { // If its just right, it doesn't play any melody
      Serial.println("Just right!");
      noTone(buzzer); // Clear Sounds
    }
  } 
  else if (mode == 2) { // Setting button for partner 
    float decibelCurrent = getDecibelLevel(); // Getting the decibel level through microphone
    Serial.print("Partner's decibel: ");
    Serial.println(decibelCurrent);

    if (decibelCurrent > DEFAULT_PARTNER_SPEECH_LEVEL + ACCEPTABLE_RANGE) { // If the partner's decibel is too loud
      Serial.println("Too loud!");
      for (int i = 0; i < 10; i++) 
        CircuitPlayground.setPixelColor(i, 255, 0, 0); // LEDs turn red

    } else if (decibelCurrent < DEFAULT_PARTNER_SPEECH_LEVEL - ACCEPTABLE_RANGE) { // If the partner's decibel is too small 
      Serial.println("Too small!");
      for (int i = 0; i < 10; i++) 
        CircuitPlayground.setPixelColor(i, 255, 165, 0); // LEDs turn orange

    } else {// If the partner's decibel is just right - LEDs turn green
      Serial.println("Just right!");
       for (int brightness = 50; brightness < 255; brightness += 5) {
      for (int i = 0; i < 10; i++) { 
        CircuitPlayground.setPixelColor(i, 0, brightness, 0);// Increasing the brightness 
      }
      delay(30);
      }
      for (int brightness = 255; brightness >= 50; brightness -= 5) { 
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, brightness, 0); // Decreasing the brightness
      }
      delay(30); // Pause
      }
    }
  } 
  else { // Turns off the buzzer sound and leds when the modes are not active 
    CircuitPlayground.clearPixels();
    noTone(buzzer);
  }

  delay(RESPONSE_INTERVAL); // Pausing before the next loop 
}

float getDecibelLevel() {//Getting the decibel through microphone 
  int samples = 160; //Setting the samples to 160
  float sum = 0;
  for (int i = 0; i < samples; i++) { // Starts loop
    sum += analogRead(microphone); // Reading the values from the microphone and adding them into sum 
  }
  float avg = sum / samples; // Calculating the average value
  return 20 * log10(avg); // Converting it to decibels 
}

void playMelody() { // Setting the melody for the wearer
  for (int thisNote = 0; thisNote < 7; thisNote++) { // Loop 7 melodies 
    int noteDuration = 1000 / noteDurations[thisNote]; // Calculating duration
    tone(buzzer, melody[thisNote], noteDuration); // Playing the melody on the buzzer
    delay(noteDuration * 1.30); // Pause for it to finish 
    noTone(buzzer); // Clearing the melody 
  }
}

void playLoudWarningMelody() { // Setting the warning melody
  for (int thisNote = 0; thisNote < 5; thisNote++) { //Loop 5 melodies
    int noteDuration = 1000 / loudNoteDurations[thisNote]; // Calculating duration
    tone(buzzer, loudWarningMelody[thisNote], noteDuration); // Playing the melody on buzzer
    delay(noteDuration * 1.30); //Pausing for it to finish
    noTone(buzzer); // clearing the melody 
  }
}
