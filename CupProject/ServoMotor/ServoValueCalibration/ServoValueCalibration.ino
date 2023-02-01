
#include <Servo.h>
#include <ezButton.h>

Servo valveServo;     // Creates a new servo object
int pos = 0;          // Variable pos set to 0
int servoOpen = 0;    // Variable servoOpen set to 0 **** Revisit for calibration ****
int servoClose = 30;  // Variable servoClose set to 30 ****Revisit for calibration ****
ezButton button1(2);  


void setup() {
  Serial.begin(9600);
  valveServo.attach(10);        // Attaches servo object to digital pin 10 on the Arduino Board
  button1.setDebounceTime(50);  // set debounce time to 50 milliseconds
  valveServo.write(servoOpen);  
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    valveServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
    if(button1.isPressed()){
    Serial.println(pos);
    delay(1000);
    }
  }
  
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    valveServo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println("Current Position" + pos);
    delay(15);                       // waits 15 ms for the servo to reach the position
    if(button1.isPressed()){
    Serial.println(pos);
    delay(1000);
    }
  }
}
