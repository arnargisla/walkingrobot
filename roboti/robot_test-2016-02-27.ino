#include <AFMotor.h>

// Skilgreini mótora
AF_DCMotor frontLeg(1);
AF_DCMotor backLeg(2);
AF_DCMotor Weight(3);

// Skilgreini pinnanúmer á rofum
const int frontMIN = 0;
const int frontMAX = 1;
const int backMIN = 2;
const int backMAX = 3;
const int weightMIN = 4;
const int weightMAX = 5;

// Ljósdíóða til að prófa
const int ledPin = 13;
bool buttonState = false;

void setup() {
  // Byrja Serial samskipti
  Serial.begin(9600);
  Serial.println("Hello");
  
  // Upphafsstilli mótora
  frontLeg.setSpeed(255);
  frontLeg.run(RELEASE);
  
  backLeg.setSpeed(255);
  backLeg.run(RELEASE);
  
  Weight.setSpeed(255);
  Weight.run(RELEASE);

  // Skilgreini rofapinnana sem inntak
  // Sleppa þessu ef þeir eru interrupt?
  pinMode(frontMIN, INPUT);
  pinMode(frontMAX, INPUT);
  pinMode(backMIN, INPUT);
  pinMode(backMAX, INPUT);
  pinMode(weightMIN, INPUT);
  pinMode(weightMAX, INPUT);

  //Ljósdíóðan
  pinMode(ledPin, OUTPUT);
}

void anInterruptFunction() {
  // Do something
  buttonState = !buttonState;
}

void loop() {
  // put your main code here, to run repeatedly:
  attachInterrupt(digitalPinToInterrupt(2),anInterruptFunction,RISING);
  if(buttonState == HIGH){
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
