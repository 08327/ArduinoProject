// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "pitches.h"

// Define pin connections
const int buttonPin = 1;  // Pin for the button
const int ledPins[] = {3, 5, 7};  // Pins for the LEDs
const int ledCount = 6;  // Number of LEDs
#define BUZZER_PIN 9  // Pin for the buzzer

// Melody and durations
int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

  NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
  NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
  NOTE_C6
};

int durations[] = {
  8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,

  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 16,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 8, 8, 8, 8,

  8, 16, 2, 8, 8,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 4, 8, 4, 8, 4, 8,
  1
};

// Flash frequencies for each LED in three unique groups (in milliseconds)
const int flashFrequencies[] = {200, 300, 400};

// Variables to track LED flashing states and song playing
bool isFlashing = false;
bool isSongPlaying = false;
unsigned long flashStartTime = 0;
unsigned long songStartTime = 0;

void setup() {
  // Initialize the button pin as input with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize all LED pins as output and ensure they are off initially
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Initialize the buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Read the state of the button
  static bool lastButtonState = HIGH;  // Start assuming button is not pressed
  bool currentButtonState = digitalRead(buttonPin);

  // Detect if the button was just pressed
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Start the song and flashing
    isFlashing = true;
    isSongPlaying = true;
    songStartTime = millis();
    flashStartTime = millis();
  }

  // Update the last button state for the next iteration
  lastButtonState = currentButtonState;

  // Handle LED flashing logic while the song is playing
  if (isSongPlaying) {
    int size = sizeof(durations) / sizeof(int);
    for (int note = 0; note < size; note++) {
      int duration = 1000 / durations[note];
      tone(BUZZER_PIN, melody[note], duration);

      // Flash LEDs based on the current note's duration
      for (int i = 0; i < ledCount; i++) {
        digitalWrite(ledPins[i], (millis() % flashFrequencies[i]) < (flashFrequencies[i] / 2) ? HIGH : LOW);
      }

      // Pause between notes
      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);

      // Stop the tone playing after each note
      noTone(BUZZER_PIN);

      // Stop flashing if song has ended
      if (millis() - songStartTime >= 60000) {
        isSongPlaying = false;
        isFlashing = false;
        for (int i = 0; i < ledCount; i++) {
          digitalWrite(ledPins[i], LOW);
        }
      }
    }
  }

  // Ensure LEDs are off when not flashing
  if (!isFlashing) {
    for (int i = 0; i < ledCount; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }

  // Small delay for stability
  delay(10);
}
