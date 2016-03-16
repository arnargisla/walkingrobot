
void frontLegSwitchInterrupt(){
  frontLegSwitchPressed = true;
  //Serial.println("|_frontleg switch interrupt");
}

void backLegSwitchInterrupt(){
  backLegSwitchPressed = true;
  //Serial.println("|_backleg switch interrupt");
}

void weightSwitchInterrupt(){
  weightSwitchPressed = true;
  //Serial.println("|_weight switch interrupt");
}

void resetSwitchStates(){
  Serial.println("RESET switch states.");
  frontLegSwitchPressed = false;
  backLegSwitchPressed = false;
  weightSwitchPressed = false;
}

boolean frontLegBottomSwitchOn(){
  return digitalRead(frontLegBottomPin) == true;
}

boolean frontLegTopSwitchOn(){
  int frontLegSwitchState = digitalRead(frontLegSwitchPin);
  if(frontLegSwitchState == 1 && !frontLegBottomSwitchOn()) {
    return true;
  }
  return false;
}

boolean backLegFrontSwitchOn(){
  return digitalRead(backLegFrontSwitchPin) == true;
}

boolean backLegBackSwitchOn(){
  int backLegSwitchState = digitalRead(backLegSwitchPin);
  if(backLegSwitchState == 1 && !backLegFrontSwitchOn()) {
    return true;
  }
  return false;
}

boolean weightFrontSwitchOn(){
  return digitalRead(weightFrontSwitchPin) == true;
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
  
  Serial.print("  frontLegBottomSwitchOn: ");
  Serial.println(frontLegBottomSwitchOn());
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
