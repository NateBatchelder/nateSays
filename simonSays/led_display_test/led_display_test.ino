#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Initialize the LCD
hd44780_I2Cexp lcd;

const int modeSwitchPin = 11; // SPDT switch connected to digital pin 11
const int buzzerPin = 10;     // Buzzer connected to digital pin 10
int gameMode = 0;             // 0 for Normal, 1 for Hard
int lives = 3;                // Start with 3 lives
int lastModeState = -1;       // Last state of the game mode switch

void setup() {
  pinMode(modeSwitchPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(3, 0); // Set cursor at position 3 (0-based index)
  lcd.print("Simon Says");
  updateDisplay();
}

void loop() {
  int switchState = debounceReadPin(modeSwitchPin);
  if (switchState != lastModeState) { // Check if mode has changed
    if (switchState == LOW && gameMode != 0) {
      gameMode = 0;
      Serial.println("Game Mode: Normal");
      tone(buzzerPin, 1000, 200); // Play a chime
    } else if (switchState == HIGH && gameMode != 1) {
      gameMode = 1;
      Serial.println("Game Mode: Hard");
      tone(buzzerPin, 1000, 200); // Play a chime
    }
    lastModeState = switchState; // Update last known state
    updateDisplay(); // Update the LCD display
  }
}

void updateDisplay() {
  lcd.setCursor(0, 1);  // Set cursor at the beginning of the second line
  for (int i = 0; i < lives; i++) {
    lcd.print('*'); // Print asterisk for each life
  }
  // Clear any remaining characters from previous updates
  for (int i = lives; i < 5; i++) { // Assuming a maximum of 5 lives for clearing
    lcd.print(' ');
  }

  // Print the game mode aligned to the right
  String modeText = (gameMode == 0) ? "Normal" : "Hard";
  lcd.setCursor(16 - modeText.length(), 1);
  lcd.print(modeText);
}

int debounceReadPin(int pin) {
  int state = digitalRead(pin);
  delay(50);
  return digitalRead(pin) == state ? state : -1;
}
