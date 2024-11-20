#include <SPI.h>
#include <Arduino.h>
#include <Wire.h>

#define LED0_RED 9
#define LED0_GREEN 8
#define LED0_BLUE 7
#define LED1_RED 6
#define LED1_GREEN 5
#define LED1_BLUE 4

#define BUTTON1 A0  // Blue - Player 1
#define BUTTON2 A1  // Green - Player 1
#define BUTTON3 A2  // Red - Player 1
#define BUTTON4 A3  // Blue - Player 2
#define BUTTON5 A4  // Green - Player 2
#define BUTTON6 A5  // Red - Player 2

const int buttonPins[] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6};
char buttonMapping[] = {'1', '2', '3', '4', '5', '6'}; // Corresponding button IDs

int roundCount = 0;  // Counter for the number of rounds

void lightUpLED(int player, int color);
int getButtonPin(int color);
int getButtonPinForPlayer2(int color);
void sendCommand(char command);

void setup() {
  Serial.begin(9600); // Initialize serial monitor
  SPI.begin();
  pinMode(MISO, OUTPUT); // Set MISO pin as output for SPI communication
  pinMode(SS, INPUT);    // Slave Select pin is input

  // Set button pins as inputs with pull-up resistors
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Set LED pins as outputs
  pinMode(LED0_RED, OUTPUT);
  pinMode(LED0_GREEN, OUTPUT);
  pinMode(LED0_BLUE, OUTPUT);
  pinMode(LED1_RED, OUTPUT);
  pinMode(LED1_GREEN, OUTPUT);
  pinMode(LED1_BLUE, OUTPUT);

  Serial.println("Waiting for button press to start the game...");
  while (true) {
    if (digitalRead(BUTTON1) == LOW || digitalRead(BUTTON2) == LOW || digitalRead(BUTTON3) == LOW || 
        digitalRead(BUTTON4) == LOW || digitalRead(BUTTON5) == LOW || digitalRead(BUTTON6) == LOW) {
      sendCommand('S'); // Send game start signal to master
      Serial.println("Game started!");
      delay(1000); // 1-second delay for master display to update
      break; // Start the game when any button is pressed
    }
  }
}

void loop() {
  roundCount++; // Increment round counter

  // Select a random color and player
  int color = random(0, 3);  // 0 = Blue, 1 = Green, 2 = Red
  int player = random(1, 3); // 1 = Player 1, 2 = Player 2

  Serial.print("Player ");
  Serial.print(player);
  Serial.print(" assigned color: ");
  Serial.println(color == 0 ? "Blue" : (color == 1 ? "Green" : "Red"));

  lightUpLED(player, color);

  int buttonPin = (player == 1) ? getButtonPin(color) : getButtonPinForPlayer2(color);
  unsigned long startMillis = millis();

  // Wait for correct button press
  while (millis() - startMillis < 10000) { // 10 seconds timeout
    if (digitalRead(buttonPin) == LOW) {
      sendCommand(buttonMapping[buttonPin - A0]); // Send button ID to master
      Serial.println("Correct button pressed!");
      delay(500); // Debounce delay
      break;
    }
  }

  Serial.println("Round finished.");
  lightUpLED(player, -1); // Turn off LEDs
  delay(1000); // Pause before the next round

  // End game after 5 rounds
  if (roundCount >= 5) {
    sendCommand('E'); // Send game over signal to master
    Serial.println("Game session ended!");
    Serial.println("Press any button to start a new session...");

    while (true) {
      if (digitalRead(BUTTON1) == LOW || digitalRead(BUTTON2) == LOW || digitalRead(BUTTON3) == LOW || 
          digitalRead(BUTTON4) == LOW || digitalRead(BUTTON5) == LOW || digitalRead(BUTTON6) == LOW) {
        sendCommand('S'); // Send game start signal to master
        roundCount = 0;   // Reset round counter
        Serial.println("New game session started!");
        delay(1000); // 1-second delay for master display to update
        break;
      }
    }
  }
}

void lightUpLED(int player, int color) {
  if (player == 1) {
    digitalWrite(LED0_RED, color == 2);
    digitalWrite(LED0_GREEN, color == 1);
    digitalWrite(LED0_BLUE, color == 0);
  } else if (player == 2) {
    digitalWrite(LED1_RED, color == 2);
    digitalWrite(LED1_GREEN, color == 1);
    digitalWrite(LED1_BLUE, color == 0);
  } else {
    digitalWrite(LED0_RED, LOW);
    digitalWrite(LED0_GREEN, LOW);
    digitalWrite(LED0_BLUE, LOW);
    digitalWrite(LED1_RED, LOW);
    digitalWrite(LED1_GREEN, LOW);
    digitalWrite(LED1_BLUE, LOW);
  }
}

int getButtonPin(int color) {
  return buttonPins[color]; // Return button pin for Player 1
}

int getButtonPinForPlayer2(int color) {
  return buttonPins[color + 3]; // Return button pin for Player 2
}

void sendCommand(char command) {
  SPDR = command;               // Write the command to SPI data register
  while (!(SPSR & (1 << SPIF))); // Wait for transmission to complete
}