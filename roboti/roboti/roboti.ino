#include <AFMotor.h>
#include <IRremote.h>
#include "DCMotor.h"
#include "RemoteButtonDefinitions.h"

#define ROBOT_STATE_0 0
#define ROBOT_STATE_1 1
#define ROBOT_STATE_2 2
#define ROBOT_STATE_3 3
#define ROBOT_STATE_4 4
#define ROBOT_STATE_RESET 5
#define ROBOT_STATE_PAUSED 6

AF_DCMotor afmotor1(1);
AF_DCMotor afmotor2(2);
AF_DCMotor afmotor3(3);

const int irRemotePin = 26;
const int frontLegSwitchPin = 18;
const int backLegSwitchPin = 19;
const int weightSwitchPin = 20;
const int frontLegBottomPin = 31;
const int backLegFrontSwitchPin = 33;
const int weightFrontSwitchPin = 35;

const boolean stateDelayOn = true;
const int stateDelayDurationMs = 500;
const int stateReportingOn = true;


int currentRobotState = ROBOT_STATE_0;
int lastRobotState = ROBOT_STATE_0;

// Pæling að upphasstilla þetta einhvernveginn??
boolean frontLegSwitchPressed = false;
boolean backLegSwitchPressed = false;
boolean weightSwitchPressed = false;

IRrecv irRemote(irRemotePin);
decode_results irRemoteResults;
unsigned long irRemoteKeyPressed;

DCMotor motor1, motor2, motor3;
DCMotor* currentlySelectedMotor;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!\n");
  
  irRemote.enableIRIn();
  
  motor1 = {"Motor 1", &afmotor1, 255, 16, true, false};
  motor2 = {"Motor 2", &afmotor2, 255, 16, true, false};
  motor3 = {"Motor 3", &afmotor3, 255, 16, true, false};
  currentlySelectedMotor = &motor1;
  
  // turn on motors
  motor1.motor->run(RELEASE);
  motor1.motor->setSpeed(motor1.motorSpeed);
  motor2.motor->run(RELEASE);
  motor2.motor->setSpeed(motor2.motorSpeed);
  motor3.motor->run(RELEASE);
  motor3.motor->setSpeed(motor3.motorSpeed);
  
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

void handleRemote(){
  if (irRemote.decode(&irRemoteResults)) {
    irRemoteKeyPressed = irRemoteResults.value;
    printReceivedValue(irRemoteKeyPressed);
    
    if(irRemoteKeyPressed == key_poweron){
      stopAllMotors(); 
    } else if (irRemoteKeyPressed == key_arrows){
      printSwitchStates();
    } else if (irRemoteKeyPressed == key_usd){
      resetSwitchStates();
    } else if (irRemoteKeyPressed == key_mode){
      printCurrentRobotState();
    } else if (irRemoteKeyPressed == key_mute){
      toggleRobotPauseState();
    }
        
    determineCurrentlySelectedMotor(irRemoteKeyPressed);
    updateCurrentlySelectedMotorState(irRemoteKeyPressed);
    updateCurrentlySelectedMotor();
    printCurrentlySelectedMotorState();
    
    irRemote.resume(); // Receive the next value
  }
}

void toggleRobotPauseState(){
  if(currentRobotState == ROBOT_STATE_PAUSED){
    currentRobotState = lastRobotState;
  }else{
    currentRobotState = ROBOT_STATE_PAUSED;
  }
}
void setRobotStateToPause(){
  if(currentRobotState != ROBOT_STATE_PAUSED){
    lastRobotState = currentRobotState;
  }
  currentRobotState = ROBOT_STATE_PAUSED;
  Serial.print("Pausing robot last state was:\n  ");
  printRobotState(lastRobotState);
}

void printRobotState(int state){
  Serial.print("Robot state: "); 
  switch(currentRobotState){
    case ROBOT_STATE_PAUSED:
      Serial.println("ROBOT_STATE_PAUSED. ");
      break;
    case ROBOT_STATE_RESET:
      Serial.println("ROBOT_STATE_RESET. ");
      break;
    default:
      Serial.print(state);
      Serial.print(". \n");
      break;
  }
}
void updateRobotState(){
  if(stateDelayOn){
    delay(stateDelayDurationMs);
  }
  if(stateReportingOn){
    printRobotState(currentRobotState); 
  }
  int nextRobotState = ROBOT_STATE_RESET;
  switch(currentRobotState){
    case ROBOT_STATE_PAUSED:
      nextRobotState = stepRobotStatePaused();
      break;
    case ROBOT_STATE_RESET:
      nextRobotState = stepRobotStateReset();
      break;
    case ROBOT_STATE_0:
      nextRobotState = stepRobotState0();
      break;
    case ROBOT_STATE_1:
      nextRobotState = stepRobotState1();
      break;
    case ROBOT_STATE_2:
      nextRobotState = stepRobotState2();
      break;
    case ROBOT_STATE_3:
      nextRobotState = stepRobotState3();
      break;
    case ROBOT_STATE_4:
      nextRobotState = stepRobotState4();
      break;
    default:
      Serial.println(" Error in state machine! ");
      break;
  }
  
  currentRobotState = nextRobotState;
}

int stepRobotStatePaused(){
  return ROBOT_STATE_PAUSED;
}

int stepRobotStateReset(){
  // PRE: 
  //   unknown
  // POST:
  //   front leg: down
  //   back leg: back
  //   weight front
  // next state -> 1
  return ROBOT_STATE_PAUSED;
}

int stepRobotState0(){
  // PRE: 
  //   front leg: up
  //   back leg: back
  //   weight front
  // POST:
  //   front leg: down
  //   back leg: back
  //   weight front
  // next state -> 1
  return ROBOT_STATE_PAUSED;
}

int stepRobotState1(){
  // PRE: 
  //   front leg: down
  //   back leg: back
  //   weight front
  // POST:
  //   front leg: down
  //   back leg: front
  //   weight front
  // next state -> 2
  return ROBOT_STATE_PAUSED;
}

int stepRobotState2(){
  // PRE: 
  //   front leg: down
  //   back leg: front
  //   weight front
  // POST:
  //   front leg: up
  //   back leg: front
  //   weight front
  // next state -> 3
  return ROBOT_STATE_PAUSED;
}

int stepRobotState3(){
  // PRE: 
  //   front leg: up
  //   back leg: front
  //   weight front
  // POST:
  //   front leg: up
  //   back leg: back
  //   weight back
  // next state -> 4
  return ROBOT_STATE_PAUSED;
}

int stepRobotState4(){
  // PRE: 
  //   front leg: up
  //   back leg: back
  //   weight back
  // POST:
  //   front leg: up
  //   back leg: back
  //   weight front
  // next state -> 4
  
  // front leg: up
  // back leg: back
  // weight front
  // next state -> 0
  return ROBOT_STATE_PAUSED;
}

void resetSwitchStates(){
  Serial.println("RESET switch states.");
  frontLegSwitchPressed = false;
  backLegSwitchPressed = false;
  weightSwitchPressed = false;
}

boolean frontLegBettomSwitchOn(){
  return digitalRead(frontLegBottomPin) == true;
}

boolean backLegFrontSwitchOn(){
  return digitalRead(backLegFrontSwitchPin) == true;
}

boolean weightFrontSwitchOn(){
  return digitalRead(weightFrontSwitchPin) == true;
}

boolean frontLegTopSwitchOn(){
  int frontLegSwitchState = digitalRead(frontLegSwitchPin);
  if(frontLegSwitchState == 1 && !frontLegBettomSwitchOn()) {
    return true;
  }
  return false;
}

boolean backLegBackSwitchOn(){
  int backLegSwitchState = digitalRead(backLegSwitchPin);
  if(backLegSwitchState == 1 && !backLegFrontSwitchOn()) {
    return true;
  }
  return false;
}

boolean weightBackSwitchOn(){
  int weightSwitchState = digitalRead(weightSwitchPin);
  if(weightSwitchState == 1 && !weightFrontSwitchOn()) {
    return true;
  }
  return false;
}


void printSwitchStates(){
  int frontLegSwitchPinReading = digitalRead(frontLegSwitchPin);
  int backLegSwitchPinReading = digitalRead(backLegSwitchPin);
  int weightSwitchPinReading = digitalRead(weightSwitchPin);
  Serial.println("Switch states:");
  
  Serial.print("  frontLegBettomSwitchOn: ");
  Serial.println(frontLegBettomSwitchOn());
  Serial.print("  frontLegTopSwitchOn: ");
  Serial.println(frontLegTopSwitchOn());
  
  Serial.print("  backLegFrontSwitchOn: ");  
  Serial.println(backLegFrontSwitchOn());
  Serial.print("  backLegBackSwitchOn: ");  
  Serial.println(backLegBackSwitchOn());
  
  Serial.print("  weightFrontSwitchOn: ");
  Serial.println(weightFrontSwitchOn());
  Serial.print("  weightBackSwitchOn: ");
  Serial.println(weightBackSwitchOn());
  
  Serial.print(" frontLegSwitchPressed: ");
  Serial.println(frontLegSwitchPressed);
  Serial.print(" backLegSwitchPressed: ");
  Serial.println(backLegSwitchPressed);
  Serial.print(" weightSwitchPressed: ");
  Serial.println(weightSwitchPressed);
}

void frontLegSwitchInterrupt(){
  frontLegSwitchPressed = true;
  Serial.println("|_frontleg switch interrupt");
}

void backLegSwitchInterrupt(){
  backLegSwitchPressed = true;
  Serial.println("|_backleg switch interrupt");
}

void weightSwitchInterrupt(){
  weightSwitchPressed = true;
  Serial.println("|_weight switch interrupt");
}

void stopAllMotors(){
  deactivateAllMotors();
  updateAllMotors();
  Serial.println("Stopped all motors");
}

void deactivateAllMotors(){
  motor1.isActive = false;
  motor2.isActive = false;
  motor3.isActive = false;
}

void updateAllMotors(){
  updateMotor(motor1);
  updateMotor(motor2);
  updateMotor(motor3);
}

void determineCurrentlySelectedMotor(unsigned long irRemoteKeyPressed){
  if(irRemoteKeyPressed == key_1){
    currentlySelectedMotor = &motor1;
  }else if(irRemoteKeyPressed == key_2){
    currentlySelectedMotor = &motor2;
  }else if(irRemoteKeyPressed == key_3){
    currentlySelectedMotor = &motor3;
  }
}

void printCurrentRobotState(){
  Serial.print("Current robot state: ");
  String state;
  switch(currentRobotState){
    case ROBOT_STATE_RESET:
      state = " RESET state.";
      break;
    case ROBOT_STATE_PAUSED:
      state = " PAUSED state.";
      break;
    case ROBOT_STATE_0:
      state = " 0 state.";
      break;
    case ROBOT_STATE_1:
      state = " 1 state.";
      break;
    case ROBOT_STATE_2:
      state = " 2 state.";
      break;
    case ROBOT_STATE_3:
      state = " 3 state.";
      break;
    case ROBOT_STATE_4:
      state = " 4 state.";
      break;
    default:
      state = " undefined state.";
      break;
  }
  Serial.println(state);
}

void printCurrentlySelectedMotorState(){
  printMotorState(*currentlySelectedMotor);
}

void printMotorState(DCMotor dcmotor){
  Serial.print("Current Motor: Name: " + dcmotor.name);
  Serial.print(" Speed: ");
  Serial.print(dcmotor.motorSpeed);
  Serial.print(" isGoingForward: ");
  if(dcmotor.isGoingForward){
    Serial.print("true");
  }else{
    Serial.print("false");
  }
  Serial.print(" isActive: ");
  if(dcmotor.isActive){
    Serial.print("true");
  }else{
    Serial.print("false");
  } 
  Serial.println("");
}

void updateCurrentlySelectedMotor(){
  updateMotor(*currentlySelectedMotor);
}
  
void updateMotor(DCMotor &dcmotor){
  if(dcmotor.isActive){
    if(dcmotor.isGoingForward){
      dcmotor.motor->run(FORWARD);
    }else{
      dcmotor.motor->run(BACKWARD);
    }
  }else{
    dcmotor.motor->run(RELEASE);
  }
  dcmotor.motor->setSpeed(dcmotor.motorSpeed);
}

void updateCurrentlySelectedMotorState(unsigned long irRemoteKeyPressed){
    updateMotorState(*currentlySelectedMotor, irRemoteKeyPressed);
}

void updateMotorState(DCMotor &dcmotor, unsigned long irRemoteKeyPressed){
  if(irRemoteKeyPressed == key_play){
    if(dcmotor.isActive){
      Serial.println("Stopping motor: \"" + dcmotor.name + "\".");
      // Stop motor
      dcmotor.isActive = !dcmotor.isActive;
    }else{
      Serial.println("Starting motor: \"" + dcmotor.name + "\".");
      // Start motor
      dcmotor.isActive = !dcmotor.isActive;
    }
  }else if(irRemoteKeyPressed == key_forward){
    // Set direction forwards
    Serial.println("Set motor: \"" + dcmotor.name + "\" direction FORWARD");
    dcmotor.isGoingForward = true;
  }else if(irRemoteKeyPressed == key_back){
    // Set direction backwards
    Serial.println("Set motor: \"" + dcmotor.name + "\" direction BACKWARD");
    dcmotor.isGoingForward = false;
  }else if(irRemoteKeyPressed == key_plus){
    // Increase speed
    dcmotor.motorSpeed = dcmotor.motorSpeed + dcmotor.dspeed;
    if(dcmotor.motorSpeed > 255){
      dcmotor.motorSpeed = 255;
    }
    Serial.print("Increase motor: \"" + dcmotor.name + "\" speed. Motor speed: ");
    Serial.println(dcmotor.motorSpeed);
  }else if(irRemoteKeyPressed == key_minus){
    // Decrease speed
    dcmotor.motorSpeed = dcmotor.motorSpeed - dcmotor.dspeed;
    if(dcmotor.motorSpeed < 0){
      dcmotor.motorSpeed = 0;
    }
    Serial.print("Decrease motor: \"" + dcmotor.name + "\" speed. Motor speed: ");
    Serial.println(dcmotor.motorSpeed);
  }
}


void printReceivedValue(unsigned long received){
  String out = "not reacognized";
  switch(received) {
    case key_poweron:
      out = "POWERON";
      break;
    case key_mode:
      out = "MODE";
      break;
    case key_mute:
      out = "MUTE";
      break;
    case key_play:
      out = "PLAY";
      break;
    case key_back:
      out = "BACK";
      break;
    case key_forward:
      out = "FORWARD";
      break;
    case key_eq:
      out = "EQ";
      break;
    case key_minus:
      out = "MINUS";
      break;
    case key_plus:
      out = "PLUS";
      break;
    case key_0:
      out = "KEY0";
      break;
    case key_arrows:
      out = "ARROWS";
      break;
    case key_usd:
      out = "USD";
      break;
    case key_1:
      out = "KEY1";
      break;
    case key_2:
      out = "KEY2";
      break;
    case key_3:
      out = "KEY3";
      break;
    case key_4:
      out = "KEY4";
      break;
    case key_5:
      out = "KEY5";
      break;
    case key_6:
      out = "KEY6";
      break;
    case key_7:
      out = "KEY7";
      break;
    case key_8:
      out = "KEY8";
      break;
    case key_9:
      out = "KEY9";
      break;
    default:
      break;
  }
  Serial.println("Key pressed: " + out);
}
