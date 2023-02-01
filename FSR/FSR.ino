/* Simple example code for Force Sensitive Resistor (FSR) with Arduino. More info: https://www.makerguides.com */

// Define FSR pin:
#define fsrpin A0

//Define variable to store sensor readings:
int fsrreading; //Variable to store FSR value

void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
}

void loop() {
  // Read the FSR pin and store the output as fsrreading:
  fsrreading = analogRead(fsrpin);

  // Print the fsrreading in the serial monitor:
  // Print the string "Analog reading = ".
  Serial.print("Analog reading = ");
  // Print the fsrreading:
  Serial.print(fsrreading);

  // We can set some threshholds to display how much pressure is roughly applied:
  if (fsrreading <= 99) {
    Serial.println(" - Empty");
  } else if (fsrreading <= 100) {
    Serial.println(" - 10%");
  } else if (fsrreading <= 200) {
    Serial.println(" - 20%");
  } else if (fsrreading <= 300) {
    Serial.println(" - 30%");
  } else if (fsrreading <= 400) {
    Serial.println(" - 40%");
  } else if (fsrreading <= 500) {
    Serial.println(" - 50%");
  } else if (fsrreading <= 600) {
    Serial.println(" - 60%");
  } else if (fsrreading <= 700) {
    Serial.println(" - 70%");
  } else if (fsrreading <= 800) {
    Serial.println(" - 80%");
  } else if (fsrreading <= 900) {
    Serial.println(" - 90%");
  } else {
    Serial.println(" - Full");
  }

  delay(500); //Delay 500 ms.
}
