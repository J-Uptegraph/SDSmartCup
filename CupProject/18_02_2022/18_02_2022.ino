#include <Servo.h>
/* Simple example code for Force Sensitive Resistor (FSR) with Arduino. More info: https://www.makerguides.com */
#define fsrXaxis A0
#define fsrYaxis A7
#define ledpinG1 2
#define ledpinG2 3
#define ledpinG3 4
#define ledpinG4 5
#define ledpinR1 6
#define ledpinR2 7
#define ledpinR3 8
#define ledpinR4 9
#define servoPin 10

int pos = 0;

Servo myservo;

//Define variable to store sensor readings:
int fsrxreading; //Variable to store FSR value at the (x-axis) base of the cup
int fsryreading; //Variable to store FSR value along the (y-axis) side of the cup
int state = HIGH; // The current state of the output pin
int previous = 0; // The previous reading from the FSR
// The follow variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int:
long lastToggleTime = 0;  // The last time the output pin was toggled ** Changed variable from "time" to "lastToggleTime" as time was a typedVariable
long debounce = 40; // The debounce time, increase if the output flickers
int angle = 0; // Servo Angle

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin);
  // Begin serial communication at a baud rate of 9600:
 
  // Set ledpinG1 as output:
  pinMode(ledpinG1, OUTPUT);
  pinMode(ledpinG2, OUTPUT);
  pinMode(ledpinG3, OUTPUT);
  pinMode(ledpinG4, OUTPUT);
  pinMode(ledpinR1, OUTPUT);
  pinMode(ledpinR2, OUTPUT);
  pinMode(ledpinR3, OUTPUT);
  pinMode(ledpinR4, OUTPUT);
  
}

void loop() {
  // Read the FSR pin and store the output as fsrreading:
  fsrxreading = analogRead(fsrXaxis);
  fsryreading = analogRead(fsrYaxis);


  //==============================================X AXIS===========================
  // Print the fsrreading in the serial monitor:
  // Print the string "Analog reading = ".
  // Print the fsrreading:

 if (fsrxreading > 10) {
  Serial.println(fsrxreading);
    Serial.println(" - Empty");
  } 
  else digitalWrite(ledpinG1, LOW);
  if (fsrxreading > 300) {
    Serial.println(" - About 30% Full");
    digitalWrite(ledpinG1, HIGH);
  } 
  else digitalWrite(ledpinG2, LOW);
  if (fsrxreading > 500) {
    Serial.println(" - About 50% Full");
    digitalWrite(ledpinG2, HIGH);
  } 
  else digitalWrite(ledpinG3, LOW);
  if (fsrxreading > 800) {
    Serial.println(" - About 80% Full");
    digitalWrite(ledpinG3, HIGH);
  } 
  else digitalWrite(ledpinG4, LOW);
  if (fsrxreading > 900) {
    Serial.println(" - Full");
    digitalWrite(ledpinG4, HIGH);
  }
  else {
    digitalWrite(ledpinG4, LOW);
  }

  //==============================================Y AXIS===========================

  // Print the fsrreading in the serial monitor:
  // Print the string "Analog reading = ".
  // Print the fsrreading:

 if (fsryreading > 20) {
    Serial.println(" - No Tilt");
    Serial.println(fsryreading);
  } 
  else digitalWrite(ledpinR1, LOW);
  if (fsryreading > 30) {
    Serial.println(" - About 30% Tilt");
    digitalWrite(ledpinR1, HIGH);
  } 
  else digitalWrite(ledpinR2, LOW);
  if (fsryreading > 40) {
    Serial.println(" - About 50% Tilt");
    digitalWrite(ledpinR2, HIGH);
  } 
  else digitalWrite(ledpinR3, LOW);
  if (fsryreading > 70) {
    Serial.println(" - About 80% of Tilt");
    digitalWrite(ledpinR3, HIGH);
  } 
  else digitalWrite(ledpinR4, LOW);
  if (fsryreading > 100) {
    Serial.println(" - Spilled");
    digitalWrite(ledpinR4, HIGH);
    myservo.write(90);
    delay(100);
    myservo.write(180);
    delay(100);
    myservo.write(0);
    delay(100);
  }
  else {
    digitalWrite(ledpinR4, LOW);
  }
  delay(200);
}
