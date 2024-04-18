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

SimonGame::SimonGame() { 
    for (int i = 0; i < numControls; ++i) {
        pinMode(ledPins[i], OUTPUT);
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
    pinMode(buzzerPin, OUTPUT);
    pinMode(vibrationMotorPin, OUTPUT);
    randomSeed(analogRead(0));  // Seed for random number generator
}

void SimonGame::startGame() {
    lcd.begin(16, 2); // Ensure LCD is initialized here instead of in the constructor
    delay(50);        // Optional: delay to ensure LCD setup completes
    lcd.clear();
    lcd.print("Simon Says");

    sequenceLength = 1;  // Start with one step in the sequence
    for (int i = 0; i < sequenceLength; i++) {
        sequence[i] = random(0, numControls);  // Generate random steps
    }
    displaySimonsSequence();  // Start the game by displaying the first sequence
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
                Serial.print("Button Pressed: ");
                Serial.println(i);
                delay(50); // Basic debouncing
                showLED(i, true);  // Ensure LED is turned on when button is pressed
                delay (200); // short delay to ensure button is visable
                if (i == sequence[inputIndex]) {
                    playNote(buttonNotes[i], 200); // Feedback for button press
                    inputIndex++;
                    if (inputIndex == sequenceLength) {
                        sequenceLength++; // Increase sequence length for next round
                        delay(1000); // Wait before next sequence display
                        displaySimonsSequence();
                        return;
                    }
                    showLED(i, false); // Turn off LED after validation
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

void SimonGame::playChime() {
    int chimeNotes[] = {262, 294, 330, 349}; // Example notes
    int count = sizeof(chimeNotes) / sizeof(chimeNotes[0]);
    int duration = 200; // Duration for each note
    for (int i = 0; i < count; i++) {
        playNote(chimeNotes[i], duration);
        delay(100); // Delay between notes
    }
}

void SimonGame::vibrateVibrator(int duration) {
    digitalWrite(vibrationMotorPin, HIGH);
    delay(duration);
    digitalWrite(vibrationMotorPin, LOW);
}

SimonGame game;  // Declare the SimonGame object globally

void setup() {
    pinMode(buzzerPin, OUTPUT);
    pinMode(vibrationMotorPin, OUTPUT);
    pinMode(modeSwitchPin, INPUT);
    for (int i = 0; i < numControls; ++i) {
        pinMode(ledPins[i], OUTPUT);
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
    randomSeed(analogRead(0));  // Seed for random number generator

    game.startGame(); // Start the game
}

void loop() {
    game.checkUserInput(); // Constantly check for user inputs
}

