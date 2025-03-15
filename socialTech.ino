#include <Adafruit_CircuitPlayground.h>

#define SAMPLES 64
#define ACCEPTABLE_RANGE 3
#define RESPONSE_INTERVAL 200

int wearerButton = A2;  // button for wearer's decibel detection
int partnerButton = A3;  // button for partner's decibel detection
int powerButton = A4;  // power button
int buzzer = A5; // setting buzzer 
int microphone = A1;   // setting the microphone

const float DEFAULT_SPEECH_LEVEL = 60;       //Setting the default decibel : 55
const float DEFAULT_LISTENER_SPEECH_LEVEL = 50; //Setting the decibel for wearer: 62

bool powerOn = true;
int mode = 0;

//Setting the melody for the wearer - when the volume is too low 
int melody[] = {300, 300, 444, 444, 500, 500, 440}; // Setting up the sound 
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
  Serial.println("Voice Feedback System Ready...");
}

void loop() {
  if (digitalRead(powerButton) == LOW) {
    delay(200);
    while(digitalRead(powerButton) == LOW);
    powerOn = !powerOn;
    Serial.println(powerOn ? "Device ON" : "Device OFF");
    if (!powerOn) {
      CircuitPlayground.clearPixels();
      noTone(buzzer);
    }
  }
  
  if (!powerOn) {
    delay(RESPONSE_INTERVAL);
    return;
  }

  if (digitalRead(wearerButton) == LOW) {
    delay(200);
    while (digitalRead(wearerButton) == LOW);
    mode = 1;
    Serial.println("Wearer speaking");
    CircuitPlayground.clearPixels();
    noTone(buzzer);
  }
  if (digitalRead(partnerButton) == LOW) {
    delay(200);
    while (digitalRead(partnerButton) == LOW);
    mode = 2;
    Serial.println("Partner speaking");
    CircuitPlayground.clearPixels();
    noTone(buzzer);
  }

  if (mode == 1) { 
    float decibelCurrent = getDecibelLevel();
    Serial.print("Wearer's decibel: ");
    Serial.println(decibelCurrent);
    
    if (decibelCurrent > DEFAULT_SPEECH_LEVEL + ACCEPTABLE_RANGE) {
      Serial.println("Too loud! Playing the warning melody");
      playLoudWarningMelody();
    } else if (decibelCurrent < DEFAULT_SPEECH_LEVEL - ACCEPTABLE_RANGE) {
      Serial.println("Too small! Playing the melody");
      playMelody();
    } else {
      Serial.println("Just right!");
      noTone(buzzer);
    }
  } 
  else if (mode == 2) { 
    float decibelCurrent = getDecibelLevel();
    Serial.print("Partner's decibel: ");
    Serial.println(decibelCurrent);

    if (decibelCurrent > DEFAULT_LISTENER_SPEECH_LEVEL + ACCEPTABLE_RANGE) {
      Serial.println("Too loud!");
      for (int i = 0; i < 10; i++) 
        CircuitPlayground.setPixelColor(i, 255, 0, 0);

    } else if (decibelCurrent < DEFAULT_LISTENER_SPEECH_LEVEL - ACCEPTABLE_RANGE) {
      Serial.println("Too small!");
      for (int i = 0; i < 10; i++) 
        CircuitPlayground.setPixelColor(i, 255, 165, 0);

    } else {
      Serial.println("Just right!");
       for (int brightness = 50; brightness < 255; brightness += 5) { //
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 0, brightness, 0);
      }
      delay(30);
      }
      for (int brightness = 255; brightness >= 50; brightness -= 5) { // 
        for (int i = 0; i < 10; i++) {
          CircuitPlayground.setPixelColor(i, 0, brightness, 0);
      }
      delay(30);
      }
    }
  } 
  else {
    CircuitPlayground.clearPixels();
    noTone(buzzer);
  }

  delay(RESPONSE_INTERVAL);
}

float getDecibelLevel() {
  int samples = 160;
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(microphone);
  }
  float avg = sum / samples;
  return 20 * log10(avg);
}

void playMelody() {
  for (int thisNote = 0; thisNote < 7; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);
    delay(noteDuration * 1.30);
    noTone(buzzer);
  }
}

void playLoudWarningMelody() {
  for (int thisNote = 0; thisNote < 5; thisNote++) {
    int noteDuration = 1000 / loudNoteDurations[thisNote]; 
    tone(buzzer, loudWarningMelody[thisNote], noteDuration);
    delay(noteDuration * 1.30);
    noTone(buzzer);
  }
}
