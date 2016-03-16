#include <AFMotor.h>
#include <IRremote.h>
#include <SoftwareSerial.h>
#include "RemoteButtonDefinitions.h"
#include "DCMotor.h"
#include "globalDefinitions.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!\n");
  
  pinMode(stateMachineMotorControlEnabledLedPin, OUTPUT);
  if(stateMachineAllowedControlOfMotors){
    digitalWrite(stateMachineMotorControlEnabledLedPin, HIGH);
  }else{
    digitalWrite(stateMachineMotorControlEnabledLedPin, LOW);
  }

  pinMode(state0LedPin, OUTPUT);
  pinMode(state1LedPin, OUTPUT);
  pinMode(state2LedPin, OUTPUT);
  pinMode(state3LedPin, OUTPUT);
  pinMode(state4LedPin, OUTPUT);
  pinMode(statePausedLedPin, OUTPUT);
  pinMode(stateResetLedPin, OUTPUT);
  
  irRemote.enableIRIn();
  
  resetAllMotors();
  
  pinMode(frontLegSwitchPin, INPUT);
  pinMode(backLegSwitchPin, INPUT);
  pinMode(weightSwitchPin, INPUT);
  pinMode(weightMiddleSwitchPin, INPUT);
  
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

