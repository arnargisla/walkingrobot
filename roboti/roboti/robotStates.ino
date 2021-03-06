void enableStateMachineMotorControl(){
  Serial.println("State machine motor control enabled!");
  digitalWrite(stateMachineMotorControlEnabledLedPin, HIGH);
  stateMachineAllowedControlOfMotors = true;
}

void disableStateMachineMotorControl(){
  Serial.println("State machine motor control disabled!");
  digitalWrite(stateMachineMotorControlEnabledLedPin, LOW);
  stateMachineAllowedControlOfMotors = false;
}


void toggleRobotPauseState(){
  if(currentRobotState == ROBOT_STATE_PAUSED){
    currentRobotState = lastRobotState;
  }else{
    lastRobotState = currentRobotState;
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

void updateRobotState(){
  if(stateDelayOn){
    delay(stateDelayDurationMs);
  }
  if(stateReportingOn){
    stateReportingTimer = (stateReportingTimer + 1) % stateReportingTimerLimit;
    if(stateReportingTimer == 0){
      printRobotState(currentRobotState); 
      localStateReportingOn = true;
    }else{
      localStateReportingOn = false;
    }
  }
  updateStateLedPanel();
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
  stopFrontLegMotor();
  stopBackLegMotor();
  stopWeightMotor();
  return ROBOT_STATE_PAUSED;
}

int stepRobotStateReset(){
  // PRE: 
  //   unknown
  // POST:
  //   front leg: down
  //   back leg: back
  //   weight forward
  // next state -> 1
  boolean stateComplete = true;
  stateComplete = moveFrontLegDown() && stateComplete;
  stateComplete = moveBackLegBack() && stateComplete;
  stateComplete = moveWeightForward() && stateComplete;
  
  if(stateComplete){
    Serial.println("State RESET complete switching to state 1");
    return ROBOT_STATE_1;
  }else{
    return ROBOT_STATE_RESET;
  }
}

int stepRobotState0(){
  // PRE: 
  //   front leg: up
  //   back leg: back
  //   weight forward
  // POST:
  //   front leg: down
  //   back leg: back
  //   weight forward
  // next state -> 1 
  boolean stateComplete = true;
  stateComplete = moveFrontLegDown() && stateComplete;
  stateComplete = moveBackLegBack() && stateComplete;
  stateComplete = moveWeightForward() && stateComplete;
  
  if(stateComplete){
    Serial.println("State 0 complete switching to state 1");
    return ROBOT_STATE_1;
  }else{
    return ROBOT_STATE_0;
  }
}


int stepRobotState1(){
  // PRE: 
  //   front leg: down
  //   back leg: back
  //   weight forward
  // POST:
  //   front leg: down
  //   back leg: forward
  //   weight forward
  // next state -> 2  
  boolean stateComplete = true;
  stateComplete = moveFrontLegDown() && stateComplete;
  stateComplete = moveBackLegForward() && stateComplete;
  stateComplete = moveWeightForward() && stateComplete;
  
  if(stateComplete){
    Serial.println("State 1 complete switching to state 2");
    return ROBOT_STATE_2;
  }else{
    return ROBOT_STATE_1;
  }
}

int stepRobotState2(){
  // PRE: 
  //   front leg: down
  //   back leg: forward
  //   weight forward
  // POST:
  //   front leg: up
  //   back leg: forward
  //   weight forward
  // next state -> 3
  boolean stateComplete = true;
  stateComplete = moveFrontLegUp() && stateComplete;
  stateComplete = moveBackLegForward() && stateComplete;
  stateComplete = moveWeightForward() && stateComplete;
  
  if(stateComplete){
    Serial.println("State 2 complete switching to state 3");
    return ROBOT_STATE_3;
  }else{
    return ROBOT_STATE_2;
  }
}

int stepRobotState3(){
  // PRE: 
  //   front leg: up
  //   back leg: forward
  //   weight forward
  // POST:
  //   front leg: up
  //   back leg: back
  //   weight back
  // next state -> 4
  boolean stateComplete = true;
  stateComplete = moveFrontLegUp() && stateComplete;
  stateComplete = moveBackLegBack() && stateComplete;
  stateComplete = moveWeightBack() && stateComplete;
  
  if(stateComplete){
    Serial.println("State 3 complete switching to state 4");
    return ROBOT_STATE_4;
  }else{
    return ROBOT_STATE_3;
  }
}

int stepRobotState4(){
  // PRE: 
  //   front leg: up
  //   back leg: back
  //   weight back
  // POST:
  //   front leg: up
  //   back leg: back
  //   weight forward
  // next state -> 4
  boolean stateComplete = true;
  stateComplete = moveFrontLegUp() && stateComplete;
  stateComplete = moveBackLegBack() && stateComplete;
  stateComplete = moveWeightForward() && stateComplete;
  
  if(stateComplete){
    Serial.println("State 4 complete switching to state 0");
    return ROBOT_STATE_0;
  }else{
    return ROBOT_STATE_4;
  }
}

void printCurrentRobotState(){
  printRobotState(currentRobotState);
}

void printRobotState(int stateToPrint){
  Serial.print("Current robot state: ");
  String state;
  switch(stateToPrint){
    case ROBOT_STATE_RESET:
      state = " RESET state.";
      break;
    case ROBOT_STATE_PAUSED:
      state = " PAUSED state.";
      break;
    case ROBOT_STATE_0:
      state = " state 0.";
      break;
    case ROBOT_STATE_1:
      state = " state 1.";
      break;
    case ROBOT_STATE_2:
      state = " state 2.";
      break;
    case ROBOT_STATE_3:
      state = " state 3.";
      break;
    case ROBOT_STATE_4:
      state = " state 4.";
      break;
    default:
      state = " undefined state.";
      break;
  }
  Serial.println(state);
}

void toggleRobotStateReporting(){
  stateReportingOn = !stateReportingOn;
}


int moveFrontLegDown(){  
  if(frontLegBottomSwitchOn()){
    if(localStateReportingOn) Serial.println("+ Front leg is down");
    if(stateMachineAllowedControlOfMotors) stopFrontLegMotor();
    return true;
  }else{
    if(localStateReportingOn) Serial.println("- Front leg not yet down");
    if(stateMachineAllowedControlOfMotors) runFrontLegMotorDown();
    return false;
  }
}
int moveFrontLegUp(){  
  if(frontLegTopSwitchOn()){
    if(localStateReportingOn) Serial.println("+ Front leg is up");
    if(stateMachineAllowedControlOfMotors) stopFrontLegMotor();
    return true;
  }else{
    if(localStateReportingOn) Serial.println("- Front leg not yet up");
    if(stateMachineAllowedControlOfMotors) runFrontLegMotorUp();
    return false;
  }
}


int moveBackLegForward(){
   if(backLegFrontSwitchOn()){
    if(localStateReportingOn) Serial.println("+ Back leg is forward");
    if(stateMachineAllowedControlOfMotors) stopBackLegMotor();
    return true;
  }else{
    if(localStateReportingOn) Serial.println("- Back leg not yet forward");
    if(stateMachineAllowedControlOfMotors) runBackLegMotorForward();
    return false;
  }
}

int moveBackLegBack(){
   if(backLegBackSwitchOn()){
    if(localStateReportingOn) Serial.println("+ Back leg is back");
    if(stateMachineAllowedControlOfMotors) stopBackLegMotor();
    return true;
  }else{
    if(localStateReportingOn) Serial.println("- Back leg not yet back");
    if(stateMachineAllowedControlOfMotors) runBackLegMotorBackward();
    return false;
  }
}

int moveWeightForward(){
  if(weightFrontSwitchOn()){
    if(localStateReportingOn) Serial.println("+ Weight is forward");
    if(stateMachineAllowedControlOfMotors) stopWeightMotor();
    return true;
  }else{
    if(localStateReportingOn) Serial.println("- Weight not yet forward");
    if(stateMachineAllowedControlOfMotors) runWeightMotorForward();
    return false;
  }
}

int moveWeightBack(){
  if(weightBackSwitchOn()){
    if(localStateReportingOn) Serial.println("+ Weight is back");
    if(stateMachineAllowedControlOfMotors) stopWeightMotor();
    return true;
  }else{
    if(localStateReportingOn) Serial.println("- Weight not yet back");
    if(stateMachineAllowedControlOfMotors) runWeightMotorBackward();
    return false;
  }
}

