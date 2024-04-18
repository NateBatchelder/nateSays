#include <stdlib.h> // For random number generation

// Pin setup
const int buttonPins[] = {2, 4, 6, 8};
const int ledPins[] = {3, 5, 7, 9};
const int numControls = 4;

// Game variables
int sequence[10]; // Stores the sequence of LEDs
int sequenceLength = 0; // Current length of the sequence
int inputIndex = 0; // Index for player input tracking

void setup() {
  randomSeed(analogRead(0)); // Initialize random number generator
  for (int i = 0; i < numControls; ++i) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  startGame();
}

void loop() {
  playSequence();
  if (waitForInput()) {
    startGame(); // Restart game after sequence completion or error
  }
}

void startGame() {
  sequenceLength = 1; // Start with one step in the sequence
  generateSequence();
  inputIndex = 0;
}

void generateSequence() {
  for (int i = 0; i < 10; i++) { // Max sequence length of 10 steps
    sequence[i] = random(0, numControls);
  }
}

void playSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);
    delay(500);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(250);
  }
}

bool waitForInput() {
  int playerInputCount = 0;
  while (playerInputCount < sequenceLength) {
    for (int i = 0; i < numControls; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(50); // Debounce
        while (digitalRead(buttonPins[i]) == LOW); // Wait for button release
        if (i == sequence[playerInputCount]) {
          digitalWrite(ledPins[i], HIGH);
          delay(200);
          digitalWrite(ledPins[i], LOW);
          playerInputCount++;
          if (playerInputCount == sequenceLength) {
            sequenceLength++;
            return false; // Sequence correct, extend it
          }
        } else {
          return true; // Incorrect input, restart game
        }
      }
    }
  }
  return false;
}
