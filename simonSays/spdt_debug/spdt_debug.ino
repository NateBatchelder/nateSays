#include <Arduino.h>

// Define the pin where the switch is connected
const int switchPin = 11;

void setup() {
  // Initialize the switch pin as an input with a pull-up resistor
  pinMode(switchPin, INPUT_PULLUP);

  // Start the serial communication
  Serial.begin(9600);
  // Print a start message
  Serial.println("Switch Test Started - Reading Switch State on GPIO 11");
}

void loop() {
  // Read the state of the switch
  int switchState = digitalRead(switchPin);

  // Print the state of the switch to the Serial Monitor
  Serial.print("Switch State: ");
  Serial.println(switchState); // Will print '0' if the switch is pressed (LOW), '1' if not pressed (HIGH)

  // Add a small delay to make the output readable
  delay(500);
}
