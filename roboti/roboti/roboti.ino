#include <AFMotor.h>
#include <IRremote.h>
#include <SoftwareSerial.h>
#include "RemoteButtonDefinitions.h"
#include "DCMotor.h"
#include "globalDefinitions.h"
#include "stateTransitions.cpp"

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!\n");
  
  pinMode(state0LedPin, OUTPUT);
  pinMode(state1LedPin, OUTPUT);
  pinMode(state2LedPin, OUTPUT);
  pinMode(state3LedPin, OUTPUT);
  pinMode(state4LedPin, OUTPUT);
  pinMode(statePausedLedPin, OUTPUT);
  pinMode(stateResetLedPin, OUTPUT);
  
  irRemote.enableIRIn();
  
  frontLegMotor = {"Motor 1", &afmotor1, 255, 16, true, false};
  backLegMotor = {"Motor 2", &afmotor2, 255, 16, true, false};
  weightMotor = {"Motor 3", &afmotor3, 255, 16, true, false};
  currentlySelectedMotor = &frontLegMotor;
  
  // turn on motors
  frontLegMotor.motor->run(RELEASE);
  frontLegMotor.motor->setSpeed(frontLegMotor.motorSpeed);
  backLegMotor.motor->run(RELEASE);
  backLegMotor.motor->setSpeed(backLegMotor.motorSpeed);
  weightMotor.motor->run(RELEASE);
  weightMotor.motor->setSpeed(weightMotor.motorSpeed);
  
  pinMode(frontLegSwitchPin, INPUT);
  pinMode(backLegSwitchPin, INPUT);
  pinMode(weightSwitchPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(frontLegSwitchPin), &frontLegSwitchInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(backLegSwitchPin), &backLegSwitchInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(weightSwitchPin), &weightSwitchInterrupt, RISING);

  delay(1000);
  resetSwitchStates();
}

void loop() {
  handleRemote();
  updateRobotState();
}

