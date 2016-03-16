void runFrontLegMotorUp();
void runFrontLegMotorDown();
void runBackLegMotorForward();
void runBacklegMotorBack();
void runWeightMotorForward();
void runWeightMotorBack();
void runFrontLegMotor(int direction);
void runBackLegMotor(int direction);
void runWieghtMotor(int direction);

void resetAllMotors(){
  frontLegMotor = {"Motor 1", &afmotor1, 255, 16, true, false};
  backLegMotor = {"Motor 2", &afmotor2, 255, 16, true, false};
  weightMotor = {"Motor 3", &afmotor3, 255, 16, true, false};
  currentlySelectedMotor = &frontLegMotor;
  
  frontLegMotor.motor->run(RELEASE);
  frontLegMotor.motor->setSpeed(frontLegMotor.motorSpeed);
  backLegMotor.motor->run(RELEASE);
  backLegMotor.motor->setSpeed(backLegMotor.motorSpeed);
  weightMotor.motor->run(RELEASE);
  weightMotor.motor->setSpeed(weightMotor.motorSpeed);
}



void stopAllMotors(){
  deactivateAllMotors();
  updateAllMotors();
  Serial.println("Stopped all motors");
}

void deactivateAllMotors(){
  frontLegMotor.isActive = false;
  backLegMotor.isActive = false;
  weightMotor.isActive = false;
}

void updateAllMotors(){
  updateMotor(frontLegMotor);
  updateMotor(backLegMotor);
  updateMotor(weightMotor);
}

void determineSelectedMotor(unsigned long irRemoteKeyPressed){
  if(irRemoteKeyPressed == key_1){
    currentlySelectedMotor = &frontLegMotor;
  }else if(irRemoteKeyPressed == key_2){
    currentlySelectedMotor = &backLegMotor;
  }else if(irRemoteKeyPressed == key_3){
    currentlySelectedMotor = &weightMotor;
  }
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


