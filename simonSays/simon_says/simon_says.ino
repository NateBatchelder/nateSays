#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Initialize the LCD
hd44780_I2Cexp lcd;

const int modeSwitchPin = 11; // SPDT switch pin (adjust as needed)
int lives; // Lives based on game mode
boolean gameMode = false; // false for Normal, true for Hard

void setup() {
  pinMode(modeSwitchPin, INPUT_PULLUP);
  lcd.begin(16, 2); // Initialize LCD for 16 columns and 2 rows
  lcd.clear();
  lcd.setCursor(3, 0); // Set cursor to start position for centered text on the first line
  lcd.print("Simon Says"); // Print static text on the first line
  updateDisplay(); // Initial call to display the game mode and lives
}

void loop() {
  // Continuously check and update game mode and lives display
  updateDisplay();
  // Additional game logic here...
  delay(100); // Delay to prevent overly rapid display updates
}

void updateDisplay() {
  // Read game mode from SPDT switch
  gameMode = digitalRead(modeSwitchPin);

  // Clear and set up the second line display
  lcd.setCursor(0, 1); // Set cursor at the beginning of the second line
  if(gameMode == LOW) {
    lcd.print("Normal ");
    lives = 5;
  } else {
    lcd.print("Hard   ");
    lives = 3;
  }

  // Display the number of lives right-justified
  lcd.setCursor(11, 1); // Position for displaying lives count
  lcd.print("L:");
  lcd.print(lives);
}

// Include any additional functions or game logic here as needed
