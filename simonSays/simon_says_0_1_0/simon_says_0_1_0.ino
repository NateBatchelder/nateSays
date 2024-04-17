#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Initialize the LCD
hd44780_I2Cexp lcd;

const int buzzerPin = 10; // Buzzer connected to digital pin 10
const int ledPins[] = {3, 5, 7, 9}; // Pins for Yellow, Blue, White, Green LEDs
const int buttonPins[] = {2, 4, 6, 8}; // Corresponding button pins
const int tones[] = {262, 294, 330, 349}; // Frequencies for C4, D4, E4, F4

const int modeSwitchPin = 11; // SPDT switch connected to digital pin 11
int gameMode = -1; // Initial state to force update
int lives = 3; // Start with 3 lives
int lastModeState = -1; // Last state of the game mode switch
int sequenceLength = 1; // Initial sequence length
int sequenceSpeed = 1000; // Initial speed in milliseconds
int currentSequence[10]; // Array to store the current sequence
int userInputIndex = 0; // Index for user inputs

void setup() {
  pinMode(modeSwitchPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // Using internal pull-up resistors
  }
  lcd.begin(16, 2);
  randomSeed(analogRead(0)); // Initialize random seed
  updateDisplay();
}

void loop() {
  int switchState = digitalRead(modeSwitchPin);

  if (switchState != lastModeState) {
    lastModeState = switchState;
    gameMode = (switchState == LOW) ? 0 : 1;
    tone(buzzerPin, 1000, 200); // Play a chime
    adjustGameSettings();
    updateDisplay();
    generateSequence();
  }

  if (gameMode != -1) {
    playSequence();
    readPlayerInput();
  }
  
  delay(100); // Add a small delay to reduce CPU load
}

void generateSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    currentSequence[i] = random(0, 4); // Generate a random index for LEDs
  }
}

void playSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    digitalWrite(ledPins[currentSequence[i]], HIGH);
    tone(buzzerPin, tones[currentSequence[i]], 250);
    delay(250 + 250); // On time + Off time
    digitalWrite(ledPins[currentSequence[i]], LOW);
    noTone(buzzerPin);
    delay(250); // Time between signals
  }
}

void readPlayerInput() {
  for (int i = 0; i < sequenceLength; i++) {
    bool inputReceived = false;
    while (!inputReceived) {
      for (int j = 0; j < 4; j++) {
        if (digitalRead(buttonPins[j]) == LOW) { // Button press detected
          inputReceived = true;
          digitalWrite(ledPins[j], HIGH);
          tone(buzzerPin, tones[j], 250);
          delay(250);
          digitalWrite(ledPins[j], LOW);
          noTone(buzzerPin);
          if (j != currentSequence[i]) {
            // Incorrect input
            handleMistake();
            return;
          }
          break;
        }
      }
    }
  }
  // If completed successfully
  sequenceLength = min(sequenceLength + 1, 10); // Increase sequence length, cap at 10
}

void handleMistake() {
  if (gameMode == 0) { // Normal mode
    // Show correct sequence and repeat
    playSequence();
  } else { // Hard mode
    lives--;
    if (lives <= 0) {
      gameOver();
      return;
    }
    updateDisplay();
  }
}

void gameOver() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Game Over");
  delay(2000);
  resetGame();
}

void resetGame() {
  lives = 3;
  sequenceLength = 1;
  updateDisplay();
  generateSequence();
}

void adjustGameSettings() {
  if (gameMode == 0) {
    sequenceLength = 1;
    sequenceSpeed = 1000;
  } else {
    sequenceLength = 5;
    sequenceSpeed = 500;
  }
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Simon Says");

  lcd.setCursor(0, 1);
  lcd.print("Lives:");
  for (int i = 0; i < lives; i++) {
    lcd.print('*');
  }
  
  lcd.setCursor(8, 1);
  lcd.print((gameMode == 0 ? "N" : "H") + String(sequenceLength));
}
