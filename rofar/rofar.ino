/*
 Input Pullup Serial
 
 This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a 
 digital input on pin 2 and prints the results to the serial monitor.
 
 The circuit: 
 * Momentary switch attached from pin 2 to ground 
 * Built-in LED on pin 13
 
 Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal 
 20K-ohm resistor is pulled to 5V. This configuration causes the input to 
 read HIGH when the switch is open, and LOW when it is closed. 
 
 created 14 March 2012
 by Scott Fitzgerald
 
 http://www.arduino.cc/en/Tutorial/InputPullupSerial
 
 This example code is in the public domain
 
 */

const int switchPin = 18;
const int ledPin = 32;

boolean ledState = false;
boolean switch1 = false;

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 150;    // the debounce time; increase if the output flickers

void setup(){
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); 
  
  //attachInterrupt(5,anInterruptFunction,RISING);
  attachInterrupt(5,anInterruptFunction,RISING);
  
}


void anInterruptFunction() {
  if(millis() - lastDebounceTime > debounceDelay){
    lastDebounceTime = millis();
    switch1 = true;
  }
}

void loop(){
  if(switch1){
    handleSwitch1();
  }
}

void handleSwitch1(){
  ledState = !ledState;
  Serial.println("Press!");
  digitalWrite(ledPin, ledState);
  switch1 = false;
}



