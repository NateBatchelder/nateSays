#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

const int modeSwitchPin = 12;
const int buzzerPin = 10;  // Buzzer pin
const int vibrationMotorPin = 11; // Vibration motor pin
const int buttonPins[] = {2, 4, 6, 8};  // Buttons pin array
const int ledPins[] = {3, 5, 7, 9};  // LEDs pin array
const String buttonColors[] = {"GREEN", "WHITE", "BLUE", "YELLOW"};
const int buttonTones[] = {262, 294, 330, 349}; // Frequencies for C4, D4, E4, F4 notes
const int numControls = 4;  // Number of button and LED pairs

class SimonGame {
private:
    hd44780_I2Cexp lcd;
    int lives; // Lives based on game mode
    boolean lastModeState = false; // Store the last mode state
    int currentModeState; // Current reading from the mode switch
    unsigned long previousMillis = 0; // will store last time LED was updated
    const long interval = 200; // interval at which to keep LED on (milliseconds)

    void checkButtons() {
        for (int i = 0; i < numControls; i++) {
            if (digitalRead(buttonPins[i]) == LOW) { // Assuming active low buttons
                activateFeedback(i);
            }
        }
    }

    void activateFeedback(int buttonIndex) {
        digitalWrite(ledPins[buttonIndex], HIGH); // LED ON
        tone(buzzerPin, buttonTones[buttonIndex], interval); // Play tone
        activateVibration(interval); // Vibrate
        updateLCDWithColor(buttonIndex); // Show color on LCD
        delay(interval); // Maintain feedback for the interval
        digitalWrite(ledPins[buttonIndex], LOW); // LED OFF
        resetLCD(); // Clear the color display and show "Simon Says"
    }

    void resetLCD() {
    lcd.setCursor(0, 0);  // Set cursor to the beginning of the first line
    lcd.print("                ");  // Clear the first line by overwriting with spaces
    lcd.setCursor(3, 0);
    lcd.print("Simon Says");
}

    void updateLCDWithColor(int buttonIndex) {
    lcd.setCursor(0, 0);  // Set cursor to the beginning of the first line
    lcd.print("                ");  // Clear the first line by overwriting with spaces
    lcd.setCursor((16 - buttonColors[buttonIndex].length()) / 2, 0); // Center the text on the first line
    lcd.print(buttonColors[buttonIndex]);
}

    void updateDisplay() {
        lcd.setCursor(0, 1);
        if (lastModeState == LOW) {
            lcd.print("Normal ");
            lives = 5;
        } else {
            lcd.print("Hard   ");
            lives = 3;
        }
        lcd.setCursor(11, 1);
        lcd.print("L:");
        lcd.print(lives);
    }

    void activateVibration(int duration) {
        digitalWrite(vibrationMotorPin, HIGH); // Turn on vibration
        delay(duration); // Keep it on for the duration specified
        digitalWrite(vibrationMotorPin, LOW); // Turn off vibration
    }

public:
    void setup() {
        pinMode(modeSwitchPin, INPUT_PULLUP);
        pinMode(buzzerPin, OUTPUT);
        pinMode(vibrationMotorPin, OUTPUT);
        lcd.begin(16, 2);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Simon Says");
        lastModeState = digitalRead(modeSwitchPin); // Initial mode state
        updateDisplay();
        for (int i = 0; i < numControls; ++i) {
            pinMode(ledPins[i], OUTPUT);
            pinMode(buttonPins[i], INPUT_PULLUP);
        }
    }

    void loop() {
        currentModeState = digitalRead(modeSwitchPin);
        if (currentModeState != lastModeState) {
            lastModeState = currentModeState;
            updateDisplay();
        }
        checkButtons(); // Check for button presses
        delay(100); // Delay to help with button debounce and stability
    }
};

SimonGame game; // Create an instance of SimonGame

void setup() {
    game.setup(); // Call the setup method of the game instance
}

void loop() {
    game.loop(); // Call the loop method of the game instance
}
