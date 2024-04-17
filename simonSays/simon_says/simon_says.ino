#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Initialize the LCD
hd44780_I2Cexp lcd;

// Constants for game modes and gameplay
const int numRounds = 10; // Number of rounds to win the game
int lives; // Lives based on game mode
int currentRound = 0; // Current round number
boolean gameRunning = false; // Game state
const int modeSwitchPin = 12; // SPDT switch pin (adjust as needed)
const int buzzerPin = 10; // Buzzer connected to digital pin 10
const int ledPins[] = {3, 5, 7, 9}; // Pins for Yellow, Blue, White, Green LEDs
const int buttonPins[] = {2, 4, 6, 8}; // Corresponding button pins
const int tones[] = {262, 294, 330, 349}; // Frequencies for C4, D4, E4, F4

void setup() {
  pinMode(modeSwitchPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // Using internal pull-up resistors
  }
  lcd.begin(16, 2);
  randomSeed(analogRead(0)); // Initialize random seed
  updateDisplay(); // Initial display update
  // Determine initial game mode
  int mode = digitalRead(modeSwitchPin);
  if(mode == LOW) {
    lives = 5; // Normal mode
  } else {
    lives = 3; // Hard mode
  }
}

void loop() {
  if (!gameRunning && checkAnyButtonPressed()) {
    gameRunning = true;
    currentRound = 0;
    startGame();
  }

  if (gameRunning) {
    playRound();
  }

  if (gameRunning && lives <= 0) {
    lcd.clear();
    lcd.print("Game Over!");
    delay(2000);
    gameRunning = false; // Reset game state
    updateDisplay();
  }
}

bool checkAnyButtonPressed() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      return true;
    }
  }
  return false;
}

void startGame() {
  lcd.clear();
  lcd.print("Game started!");
  delay(1000);
}

void playRound() {
  int sequence[10]; // array to hold sequence for current round
  for (int i = 0; i <= currentRound; i++) {
    sequence[i] = random(0, 4); // generate a random index for LEDs and tones
    digitalWrite(ledPins[sequence[i]], HIGH);
    tone(buzzerPin, tones[sequence[i]], 500); // Play tone for 500ms
    delay(600); // Wait for 600ms
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(400);
  }

  for (int i = 0; i <= currentRound; i++) {
    int buttonState = waitForButtonPress();
    if (buttonState != sequence[i]) {
      lives--;
      lcd.clear();
      lcd.print("Wrong!");
      delay(1000);
      return; // Exit current round early
    }
  }
  currentRound++;
  if (currentRound >= numRounds) {
    lcd.clear();
    lcd.print("You win!");
    delay(2000);
    gameRunning = false;
    updateDisplay();
    return;
  }
  lcd.clear();
  lcd.print("Correct!");
  delay(1000);
}

int waitForButtonPress() {
  int buttonState = -1;
  while (buttonState == -1) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        while (digitalRead(buttonPins[i]) == LOW); // Wait for button release
        buttonState = i;
        break;
      }
    }
  }
  return buttonState;
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Simon Says");
  lcd.setCursor(0, 1);
  lcd.print("Select mode, press button");
}

void scrollDisplay() {
  lcd.scrollDisplayLeft(); // Add this function call where needed
}
