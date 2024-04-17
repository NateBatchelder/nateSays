#include <Arduino.h>

const int modeSwitchPin = 11; // Switch connected to digital pin 11
int gameMode = 0; // 0 for Easy, 1 for Normal

const int buzzerPin = 10; // Buzzer connected to digital pin 10
const int ledPins[] = {3, 5, 7, 9}; // Pins for Yellow, Blue, White, Green LEDs
const int buttonPins[] = {2, 4, 6, 8}; // Corresponding button pins
const int tones[] = {262, 294, 330, 349}; // Frequencies for C4, D4, E4, F4
const int maxRounds = 10; // Maximum number of rounds in the game
int sequence[maxRounds]; // Array to store the sequence
int currentRound = 0; // Current round number
bool gameActive = false; // Track if the game is active

int debounceReadPin(int pin) {
  int firstReading = digitalRead(pin);
  delay(10); // short delay
  int secondReading = digitalRead(pin);
  if (firstReading == secondReading) {
    return firstReading;
  }
  return -1; // Return -1 if readings are inconsistent
}

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(modeSwitchPin, INPUT_PULLUP); // Setup the switch pin with pull-up resistor
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // Setting button pins as input with pull-up
  }
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Initialize random number generator
}

void loop() {
  int switchState = debounceReadPin(modeSwitchPin);

  if (switchState == LOW) {
    if (gameMode != 0) {
      gameMode = 0; // Set to Easy mode
      Serial.println("Game Mode: Easy"); // Print new mode to the serial monitor
    }
  } else if (switchState == HIGH) {
    if (gameMode != 1) {
      gameMode = 1; // Set to Normal mode
      Serial.println("Game Mode: Normal"); // Print new mode to the serial monitor
    }
  } else {
    Serial.println("Error: Switch in undefined position");
  }

  if (!gameActive) {
    for (int pin : ledPins) {
      digitalWrite(pin, HIGH); // Turn all LEDs on to indicate ready to start
    }
    for (int pin : buttonPins) {
      if (digitalRead(pin) == LOW) {
        gameActive = true;
        for (int led : ledPins) {
          digitalWrite(led, LOW); // Turn all LEDs off when the game starts
        }
        delay(500); // Debounce delay
        break;
      }
    }
  } else {
    if (currentRound < maxRounds) {
      addRandomStep();
      playSequence();
      if (!getPlayerInput()) {
        loseGame();
        return;
      }
      currentRound++; // Move to the next round if input is correct
    } else {
      winGame();
    }
  }
}

void addRandomStep() {
  int index = random(0, 4);
  sequence[currentRound] = index;
}

void playSequence() {
  for (int i = 0; i <= currentRound; i++) {
    int index = sequence[i];
    digitalWrite(ledPins[index], HIGH);
    tone(buzzerPin, tones[index], 500);
    delay(600);
    digitalWrite(ledPins[index], LOW);
  }
}

bool getPlayerInput() {
    int i = 0;
    while (i <= currentRound) {
        int expectedButton = buttonPins[sequence[i]];
        bool buttonPressed = false;
        long startTime = millis();
        int timeLimit = gameMode == 0 ? 10000 : 5000; // Extended time for Easy mode

        while (!buttonPressed && (millis() - startTime < timeLimit)) {
            for (int j = 0; j < 4; j++) {
                if (digitalRead(buttonPins[j]) == LOW) {
                    digitalWrite(ledPins[j], HIGH);
                    delay(200); // Visual feedback for button press
                    digitalWrite(ledPins[j], LOW);
                    delay(300); // Debounce delay

                    if (buttonPins[j] == expectedButton) {
                        tone(buzzerPin, tones[sequence[i]], 200);
                        delay(500);
                        noTone(buzzerPin);
                        buttonPressed = true; // Move to the next button in sequence
                        break;
                    } else {
                        if (gameMode == 0) { // Easy mode: Correct by showing the right button
                            promptForCorrectButton(expectedButton);
                        } else { // Normal mode: Incorrect button leads to game over
                            playWrongInputFeedback();
                            return false;
                        }
                    }
                }
            }
        }

        if (!buttonPressed && gameMode == 0) { // Timeout in Easy mode, prompt for the right button
            promptForCorrectButton(expectedButton);
        } else if (!buttonPressed) { // Timeout in Normal mode, lose the game
            playWrongInputFeedback();
            return false;
        }

        if (gameMode == 0 && !buttonPressed) { // If still in Easy mode and no button was pressed correctly, repeat the sequence
            i = 0; // Restart the sequence from the beginning
        } else {
            i++; // Next step in the sequence
        }
    }
    return true; // Sequence completed successfully
}

void promptForCorrectButton(int correctButton) {
    while (digitalRead(correctButton) != LOW) { // Wait until the correct button is pressed
        slowBlink(correctButton);
    }
    tone(buzzerPin, tones[sequence[currentRound]], 200);
    delay(500);
    noTone(buzzerPin);
}

void slowBlink(int pin) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[pin], HIGH);
    delay(700); // Slow blink
    digitalWrite(ledPins[pin], LOW);
    delay(700);
  }
}

void playWrongInputFeedback() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  tone(buzzerPin, 100, 1000); // Low and long tone to indicate error
  delay(1000);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  noTone(buzzerPin);
}

void winGame() {
  Serial.println("Congratulations! You've completed all rounds.");
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    tone(buzzerPin, tones[i], 500);
    delay(500);
    digitalWrite(ledPins[i], LOW);
  }
  resetGame();
}

void loseGame() {
  Serial.println("Wrong input. Game over!");
  for (int i = 3; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    tone(buzzerPin, tones[i], 500);
    delay(500);
    digitalWrite(ledPins[i], LOW);
  }
  resetGame();
}

void resetGame() {
  currentRound = 0; // Reset the round counter
  gameActive = false; // Mark the game as inactive
  Serial.println("Game reset. Ready to start a new game.");
  delay(2000); // Delay before reactivating the start condition
}
