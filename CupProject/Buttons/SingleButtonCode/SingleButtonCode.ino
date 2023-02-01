// constants won't change. They're used here to set pin numbers:
const int BUTTON_PIN = 2;       // the number of the pushbutton pin

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // read the state of the switch/button:
  int buttonState = digitalRead(BUTTON_PIN);

  // print out the button's state 
  Serial.println(buttonState);
}
