#include <LiquidCrystal_I2C.h>

// Pin definitions
const int trigPin = 2;
const int echoPin = 3;
const int forwardMotor1 = 4;
const int forwardMotor2 = 5;
const int downwardMotor = 6;
const int buzzerPin = 7;
const int buttonPin = 8;

// LCD display initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);


// Button variables
int buttonState = LOW;
int previousButtonState = LOW;
bool motorEnabled = false;

void setup() {
  Serial.begin(9600);

  // LCD setup
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.print("Underwater Robot");

  // Pin setup
  pinMode(forwardMotor1, OUTPUT);
  pinMode(forwardMotor2, OUTPUT);
  pinMode(downwardMotor, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  
}

void loop() {
  // Measure distance using the SR04T/SR04M module
  unsigned long duration = measureDistance();
  unsigned int distance = ((duration * 0.033) / 2);


  // Read the state of the button
  buttonState = digitalRead(buttonPin);

  // Check if the button state has changed
  if (buttonState != previousButtonState) {
    // If the button is pressed, toggle the motor state
    if (buttonState == HIGH) {
      motorEnabled = !motorEnabled;
    }
    // Update the previous button state
    previousButtonState = buttonState;
  }

  // Control the forward motor based on the motor state
  if (motorEnabled) {
    digitalWrite(forwardMotor1, HIGH);
    digitalWrite(forwardMotor2, LOW);
  } else {
    digitalWrite(forwardMotor1, LOW);
    digitalWrite(forwardMotor2, LOW);
  }

  // Display the average distance and object detection on the LCD
  lcd.setCursor(0, 1);
  lcd.print("DISTANCE : ");
  lcd.print(distance);
  lcd.print(" cm  ");

  lcd.setCursor(0, 0);
  if (averageDistance < 30) {
    lcd.print("Object detected!");
    // Perform actions when an object is detected
  } else {
    lcd.print("                ");  // Clear the first line of the LCD
  }

  delay(500);
}

unsigned long measureDistance() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(350);
  digitalWrite(trigPin, LOW);

  // Measure the echo pulse duration
  unsigned long duration = pulseIn(echoPin, HIGH);

  return duration;
}
