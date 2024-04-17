#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

const int modeSwitchPin = 12;
const int buzzerPin = 10;  // Buzzer pin
const int buttonPins[] = {2, 4, 6, 8};  // Buttons pin array
const int ledPins[] = {3, 5, 7, 9};  // LEDs pin array
const int numControls = 4;  // Number of button and LED pairs

class SimonGame {
private:
    hd44780_I2Cexp lcd;
    int lives; // Lives based on game mode
    boolean lastModeState = false; // Store the last mode state
    int currentModeState; // Current reading from the mode switch

public:
    void setup() {
        pinMode(modeSwitchPin, INPUT_PULLUP);
        lcd.begin(16, 2);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Simon Says");
        lastModeState = digitalRead(modeSwitchPin); // Initial mode state
        updateDisplay();

        // Initialize LEDs and buttons
        for (int i = 0; i < numControls; ++i) {
            pinMode(ledPins[i], OUTPUT);
            pinMode(buttonPins[i], INPUT_PULLUP);
        }
        pinMode(buzzerPin, OUTPUT);
    }

    void loop() {
        currentModeState = digitalRead(modeSwitchPin);
        if (currentModeState != lastModeState) {
            lastModeState = currentModeState; // Update the last mode state
            updateDisplay(); // Update display based on new mode state
        }
        delay(100); // Delay to help with button debounce and stability
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
};


SimonGame game; // Create an instance of SimonGame

void setup() {
    game.setup(); // Call the setup method of the game instance
}

void loop() {
    game.loop(); // Call the loop method of the game instance
}
