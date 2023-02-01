#include <ezButton.h>

/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-button-library
 *
 * This example:
 *   + uses debounce for multiple buttons.
 *   + reads state of multiple buttons
 *   + detects the pressed and released events of multiple buttons
 */

#include <ezButton.h>

ezButton button1(2);  
ezButton button2(4);  
ezButton button3(6); 
ezButton button4(8);

void setup() {
  Serial.begin(9600);
  button1.setDebounceTime(50); // set debounce time to 50 milliseconds
  button2.setDebounceTime(50); // set debounce time to 50 milliseconds
  button3.setDebounceTime(50); // set debounce time to 50 milliseconds
  button4.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button1.loop(); // MUST call the loop() function first
  button2.loop(); // MUST call the loop() function first
  button3.loop(); // MUST call the loop() function first
  button4.loop();

  int btn1State = button1.getState();
  int btn2State = button2.getState();
  int btn3State = button3.getState();
  int btn4State = button4.getState(); 
  //    Serial.print("button 1 state: ");
  //    Serial.println(btn1State);
  //    Serial.print("button 2 state: ");
  //    Serial.println(btn2State);
  //    Serial.print("button 3 state: ");
  //    Serial.println(btn3State);
  //    Serial.print("button 4 state: ");
  //    Serial.println(btn4State);

  if(button1.isPressed()){
    Serial.println("Button 1: Off Button Pressed");
    delay(100);
  }

  if(button1.isReleased()){
    Serial.println("Button 1: Off Button Released");
    delay(100);
  }

  if(button2.isPressed()){
    Serial.println("Button 2: Low Button Pressed");
    delay(100);
  }

  if(button2.isReleased()){
     Serial.println("Button 2: Low Button Released");
     delay(100);
  }
   
  if(button3.isPressed()){
    Serial.println("Button 3: Medium Button Pressed");
    delay(100);
  }

  if(button3.isReleased()){
    Serial.println("Button 3: Medium Button Released");
    delay(100);
  }

  if(button4.isPressed()){
    Serial.println("Button 4: High Button Pressed");
    delay(100);
  }

  if(button4.isReleased()){
    Serial.println("Button 4: High Button Released");
    delay(100);
  }
   delay(50);
}
