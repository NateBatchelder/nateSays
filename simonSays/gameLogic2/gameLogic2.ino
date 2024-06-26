#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

const int modeSwitchPin = 12;
const int buzzerPin = 10;  // Buzzer pin
const int vibrationMotorPin = 11; // Vibration motor pin
const int buttonPins[] = {2, 4, 6, 8};  // Buttons pin array
const int ledPins[] = {3, 5, 7, 9};  // LEDs pin array
const String buttonColors[] = {"GREEN", "WHITE", "BLUE", "YELLOW"};
const int buttonNotes[] = {262, 294, 330, 349}; // Frequencies for C4, D4, E4, F4 notes
const int numControls = 4;  // Number of button and LED pairs


class SimonGame {
public:
    SimonGame();  // Constructor
    void startGame();
    void displaySimonsSequence();
    void checkUserInput();
    void updateDisplay();
    void showLED(int index, bool state);
    void playNote(int frequency, int duration); // for a single note
    void playChime(); //for a series of chimes
    void vibrateVibrator(int duration); //haptic feedback

private:
    int sequence[10];
    int sequenceLength;
    int currentStep;
    hd44780_I2Cexp lcd;
};

SimonGame::SimonGame() : lcd(0x27, 16, 2) {  // Adjust 0x27 if different I2C address
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Simon Says");
    for (int i = 0; i < numControls; ++i) {
        pinMode(ledPins[i], OUTPUT);
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
    pinMode(buzzerPin, OUTPUT);
    pinMode(vibrationMotorPin, OUTPUT);
    randomSeed(analogRead(0));  // Seed for random number generator
}

void SimonGame::displaySimonsSequence() {
    for (int i = 0; i < sequenceLength; i++) {
        showLED(sequence[i], true); // Turn on LED
        playNote(buttonNotes[sequence[i]], 500); // Play note for 500 ms
        delay(600); // Wait for 600 ms
        showLED(sequence[i], false); // Turn off LED
        delay(400); // Wait for 400 ms between signals
    }
}

void SimonGame::checkUserInput() {
    int inputIndex = 0;
    while (inputIndex < sequenceLength) {
        for (int i = 0; i < numControls; i++) {
            if (digitalRead(buttonPins[i]) == LOW) {
                delay(50); // Basic debouncing
                if (i == sequence[inputIndex]) {
                    playNote(buttonNotes[i], 200); // Feedback for correct input
                    inputIndex++;
                    if (inputIndex == sequenceLength) {
                        sequenceLength++; // Increase sequence length for next round
                        delay(1000); // Wait before next sequence display
                        displaySimonsSequence();
                        return;
                    }
                } else {
                    vibrateVibrator(500); // Incorrect input
                    updateDisplay(); // Show game over or error message
                    return;
                }
            }
        }
    }
}

void SimonGame::updateDisplay() {
    lcd.clear();
    if (sequenceLength == sizeof(sequence) / sizeof(sequence[0])) {
        lcd.print("You Won!");
    } else {
        lcd.print("Game Over!");
    }
}

void SimonGame::showLED(int index, bool state) {
    digitalWrite(ledPins[index], state ? HIGH : LOW);
}

void SimonGame::playNote(int frequency, int duration) {
    tone(buzzerPin, frequency, duration);
    delay(duration);
    noTone(buzzerPin);
}

void SimonGame::vibrateVibrator(int duration) {
    digitalWrite(vibrationMotorPin, HIGH);
    delay(duration);
    digitalWrite(vibrationMotorPin, LOW);
}

SimonGame game;  // Declare the SimonGame object globally

void setup() {
    game.startGame(
      
    ); // Start the game
}

void loop() {
    game.checkUserInput(); // Constantly check for user inputs
}

