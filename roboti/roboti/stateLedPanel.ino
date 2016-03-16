void updateStateLedPanel(){
  resetStateLedPanel();
  setCurrenStateLed();
}
void resetStateLedPanel(){
  digitalWrite(state0LedPin, LOW);
  digitalWrite(state1LedPin, LOW);
  digitalWrite(state2LedPin, LOW);
  digitalWrite(state3LedPin, LOW);
  digitalWrite(state4LedPin, LOW);
  digitalWrite(statePausedLedPin, LOW);
  digitalWrite(stateResetLedPin, LOW);
}

void setCurrenStateLed(){
  setStateLed(currentRobotState);
}

void setStateLed(int robotState){
    switch(robotState){
    case ROBOT_STATE_PAUSED:
      digitalWrite(statePausedLedPin, HIGH);
      break;
    case ROBOT_STATE_RESET:
      digitalWrite(stateResetLedPin, HIGH);
      break;
    case ROBOT_STATE_0:
      digitalWrite(state0LedPin, HIGH);
      break;
    case ROBOT_STATE_1:
      digitalWrite(state1LedPin, HIGH);
      break;
    case ROBOT_STATE_2:
      digitalWrite(state2LedPin, HIGH);
      break;
    case ROBOT_STATE_3:
      digitalWrite(state3LedPin, HIGH);
      break;
    case ROBOT_STATE_4:
      digitalWrite(state4LedPin, HIGH);
      break;
    default:
      break;
  }
}



