
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
    } else if (irRemoteKeyPressed == key_eq){
      toggleRobotStateReporting();
    } else if (irRemoteKeyPressed == key_0){
      Serial.println("Resetting all motors");
      resetAllMotors();
    } else if (irRemoteKeyPressed == key_4){
      Serial.println("Setting state to RESET");
      currentRobotState = ROBOT_STATE_RESET;
    }
        
    determineSelectedMotor(irRemoteKeyPressed);
    updateCurrentlySelectedMotorState(irRemoteKeyPressed);
    updateCurrentlySelectedMotor();
    printCurrentlySelectedMotorState();
    
    irRemote.resume(); // Receive the next value
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

