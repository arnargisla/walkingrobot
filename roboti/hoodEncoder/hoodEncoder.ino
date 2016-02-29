// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 28;
const int directionLedPin = 26;
const int extraLedPin = 32;
const int opticalSensorPin = 30;

// Variables will change:
int currentDirection = HIGH;   
int encoderValue = 0;

int buttonState;
int lastButtonState = LOW;

int opticalSensorState;
int lastOpticalSensorState = LOW;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastButtonDebounceTime = 0;  // the last time the output pin was toggled
long buttonDebounceDelay = 50;    // the debounce time; increase if the output flickers

long lastEncoderDebounceTime = 0;
long encoderDebounceDelay = 50; 

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(opticalSensorPin, INPUT);
  pinMode(directionLedPin, OUTPUT);
  pinMode(extraLedPin, OUTPUT);

  // set initial LED state
  digitalWrite(directionLedPin, currentDirection);
  digitalWrite(extraLedPin, HIGH);
}

void loop() {
  int buttonPinReading = digitalRead(buttonPin);
  handleButton(buttonPinReading);
  
  int opticalSensorReading = digitalRead(opticalSensorPin);
  handleOpticalSensor(opticalSensorReading);
}

void handleOpticalSensor(int opticalSensorReading){
  if (opticalSensorReading != lastOpticalSensorState) {
    lastEncoderDebounceTime = millis();
  } 
  
  if ((millis() - lastEncoderDebounceTime) > encoderDebounceDelay) {
    if (opticalSensorReading != opticalSensorState) {
      opticalSensorState = opticalSensorReading;
      Serial.println(opticalSensorState);
      if (opticalSensorState == HIGH) {
        stepEncoder();
        printEncoderValue();
      }
    }
  }
  lastOpticalSensorState = opticalSensorReading;
}

void stepEncoder(){
  if(currentDirection == HIGH){
    encoderValue++;
  }else{
    encoderValue--;
  }
}

void printEncoderValue(){
  Serial.print("Encoder value: ");
  Serial.print(encoderValue);
  Serial.print(". Direction: ");
  if(currentDirection == HIGH){
    Serial.println(" forward.");
  }else{ 
    Serial.println(" backward.");
  }
}
  

void handleButton(int buttonPinReading){
  if (buttonPinReading != lastButtonState) {
    // reset the debouncing timer
    lastButtonDebounceTime = millis();
  } 
  
  if ((millis() - lastButtonDebounceTime) > buttonDebounceDelay) {
    // whatever the buttonPinReading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (buttonPinReading != buttonState) {
      buttonState = buttonPinReading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        currentDirection = !currentDirection;
      }
    }
  }
  digitalWrite(directionLedPin, currentDirection);
  lastButtonState = buttonPinReading;
}

