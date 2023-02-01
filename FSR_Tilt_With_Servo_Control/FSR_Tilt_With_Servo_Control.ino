/* Simple example code for Force Sensitive Resistor (FSR) with Arduino. More info: https://www.makerguides.com */

// Define FSR pin:

#include <Servo.h>
#define fsrxpin A7

Servo myservo;         // create servo object to control a servo

int pos = 0;           // variable to store the servo position      
int fsrxreading;       // Define variable to store sensor readings: Variable to store FSR value

void setup() {
  myservo.attach(10); // Attach servo to pin 10
  Serial.begin(9600); // Begin serial communication at a baud rate of 9600:
}

void loop() {
  // Read the FSR pin and store the output as fsrxreading:
  fsrxreading = analogRead(fsrxpin);

  // Print the fsrxreading in the serial monitor:
  // Print the string "Analog reading = ".
  Serial.print("Analog reading = ");
  // Print the fsrxreading:
  Serial.print(fsrxreading);

  // We can set some threshholds to display how much pressure is roughly applied:
  if (fsrxreading <= 99) {
    Serial.println(" - Level");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 100){
    Serial.println(" - 10% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 200) {
    Serial.println(" - 20% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 300) {
    Serial.println(" - 30% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 400) {
    Serial.println(" - 40% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 500) {
    Serial.println(" - 50% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 600) {
    Serial.println(" - 60% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 700) {
    Serial.println(" - 70% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 800) {
    Serial.println(" - 80% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else if (fsrxreading <= 900) {
    Serial.println(" - 90% Tilt");
    myservo.write(fsrxreading/5);
    delay(15);
  } else {
    Serial.println(" - Spilled");
    myservo.write(fsrxreading/5);
    delay(15);
  }

  delay(500); //Delay 500 ms.
}
