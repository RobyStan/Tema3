#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h> 
#include <Arduino.h>
#include <Wire.h>

// LCD pins
#define LCD_RS 7
#define LCD_EN 6
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Servo setup
Servo gameTimerServo;  
const int servoPin = 9;  
const int servoMaxAngle = 180; 
unsigned long gameStartTime = 0;  
unsigned long gameDuration = 30000;  
bool gameStarted = false;  

char receiveFromSlave();

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH); // Set SS high to start communication

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("game start...");

  // Attach the servo motor to the pin
  gameTimerServo.attach(servoPin);
  gameTimerServo.write(0);  // Make sure the servo starts at 0 degrees
}

void loop() {
  // Receive data from the slave
  char slaveMessage = receiveFromSlave();

  // Handle received data
  if (slaveMessage == 'S' && !gameStarted) { 
    // Start the game (only once)
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game is starting...");
    delay(2000); // Pause to display the message

    gameStartTime = millis();  // Start the game timer
    gameStarted = true;  // Set the gameStarted flag to true to start the timer
    lcd.clear();
  } 
  else if (slaveMessage >= '1' && slaveMessage <= '6') {
    // Button press information
    lcd.clear();
    lcd.setCursor(0, 0);

    // Determine the player based on button ID
    int player = (slaveMessage <= '3') ? 1 : 2;
    lcd.print("Player ");
    lcd.print(player);
    lcd.print(" pressed:");

    lcd.setCursor(0, 1);
    lcd.print("Button ");
    lcd.print(slaveMessage);
  } 
  else if (slaveMessage == 'E' && gameStarted) {
    // End of the game
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Press any key...");

    gameTimerServo.write(0);  // Stop the servo when the game ends
    gameStarted = false;  // Reset the gameStarted flag
  }

  // Handle game timer (servo motor control) when game is active
  if (gameStarted) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameStartTime < gameDuration) {
      // Calculate the servo angle based on the time elapsed
      int angle = map(currentMillis - gameStartTime, 0, gameDuration, 0, servoMaxAngle);
      gameTimerServo.write(angle);  // Set servo position
    } else {
      // After game duration is over, stop the servo and reset position
      gameTimerServo.write(0);
      gameStarted = false;  // Reset the gameStarted flag to indicate the game has ended
    }
  }
}

// Function to receive data from the slave over SPI
char receiveFromSlave() {
  digitalWrite(SS, LOW);               // Begin SPI transaction
  char data = SPI.transfer(0x00);      
  digitalWrite(SS, HIGH);            
  return data;
}
