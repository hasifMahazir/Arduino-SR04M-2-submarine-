#include <LiquidCrystal_I2C.h>
// Pin definitions
const int trigPin = 2;
const int echoPin = 3;
const int forwardMotor1 = 4;
const int forwardMotor2 = 5;
const int downwardMotor = 6;
const int buzzerPin = 7;

// LCD display initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Distance measurement variables
const int numReadings = 2; // Number of readings to average
unsigned int readings[numReadings];
int readingsIndex = 0;
unsigned long total = 0;

void setup() {
  Serial.begin(57600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.print("Underwater Robot");

  pinMode(forwardMotor1, OUTPUT);
  pinMode(forwardMotor2, OUTPUT);
  pinMode(downwardMotor, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize distance readings array
  for (int i = 0; i < numReadings; ++i) {
    readings[i] = 0;
  }
}

void loop() {
  // Measure distance using the SR04T/SR04M module
  unsigned long duration = measureDistance();

  // Calculate distance in centimeters
  unsigned int distance = ((duration * 0.035) / 2);

  // Store the distance reading in the readings array for averaging
  total = total - readings[readingsIndex];
  readings[readingsIndex] = distance;
  total = total + readings[readingsIndex];
  readingsIndex = (readingsIndex + 1) % numReadings;

  // Calculate the average distance
  unsigned int averageDistance = total / numReadings;
if(averageDistance == 0){

}else{
  // Display the average distance on the LCD
  lcd.setCursor(0, 1);
  //lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm  ");
  
  if (averageDistance < 30) {
    lcd.setCursor(0, 0);
    lcd.print("Object detected!");
    //digitalWrite(buzzerPin, HIGH);
  
    //digitalWrite(buzzerPin, LOW);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("                ");  // Clear the first line of LCD
  }
}
  digitalWrite(forwardMotor1, HIGH);
  digitalWrite(forwardMotor2, LOW);
  digitalWrite(downwardMotor, HIGH);

  delay(400);
}

unsigned long measureDistance() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(350);
  digitalWrite(trigPin, LOW);

  // Measure the echo pulse duration
  unsigned long duration = pulseIn(echoPin, HIGH);

  return duration;
}
