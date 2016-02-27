// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
#include <IRremote.h>
#include "DCMotor.h"

#define key_poweron 0x00FFA25D
#define key_mode    0x0000629D
#define key_mute    0xFFFFE21D
#define key_play    0x000022DD
#define key_back    0x000002FD
#define key_forward 0xFFFFC23D
#define key_eq      0xFFFFE01F
#define key_minus   0xFFFFA857
#define key_plus    0xFFFF906F
#define key_0       0x00006897
#define key_arrows  0xFFFF9867
#define key_usd     0xFFFFB04F
#define key_1       0x000030CF
#define key_2       0x000018E7
#define key_3       0x00007A85
#define key_4       0x000010EF
#define key_5       0x000038C7
#define key_6       0x00005AA5
#define key_7       0x000042BD
#define key_8       0x00004AB5
#define key_9       0x000052AD

AF_DCMotor afmotor1(1);

const int RECV_PIN = 31;
IRrecv irrecv(RECV_PIN);
decode_results results;

DCMotor motor1;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hello!");
  irrecv.enableIRIn();          // Start the receiver
  
  motor1 = {"Motor 1", &afmotor1, 150, 16, true, false};
  
  // turn on motor
  motor1.motor->run(RELEASE);
  motor1.motor->setSpeed(200);
}


boolean motor1forward = true;
boolean motor1ison = false;
int motor1speed = 200;
int motor1dspeed = 16;
boolean stateChanged = false;

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long keypressed = results.value;
    receivedValue(keypressed);
    
    // reset stateChange
    stateChanged = true;
    
    //update the motor
    updateMotorState(motor1, keypressed);
    updateMotor(motor1);
    printMotorState(motor1);
    
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}


void printMotorState(DCMotor dcmotor){
  Serial.print("Name: " + dcmotor.name);
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

void updateMotor(DCMotor &dcmotor){
  if(stateChanged){
    stateChanged = false;
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
}


void updateMotorState(DCMotor &dcmotor, int keypressed){
  if(keypressed == key_play){
    if(dcmotor.isActive){
      Serial.println("Stopping motor: \"" + dcmotor.name + "\".");
      // Stop motor
      dcmotor.isActive = !dcmotor.isActive;
    }else{
      Serial.println("Starting motor: \"" + dcmotor.name + "\".");
      // Start motor
      dcmotor.isActive = !dcmotor.isActive;
    }
  }else if(keypressed == key_forward){
    // Set direction forwards
    Serial.println("Set motor: \"" + dcmotor.name + "\" direction FORWARD");
    dcmotor.isGoingForward = true;
  }else if(keypressed == key_back){
    // Set direction backwards
    Serial.println("Set motor: \"" + dcmotor.name + "\" direction BACKWARD");
    dcmotor.isGoingForward = false;
  }else if(keypressed == key_plus){
    // Increase speed
    dcmotor.motorSpeed = dcmotor.motorSpeed + dcmotor.dspeed;
    if(dcmotor.motorSpeed > 255){
      dcmotor.motorSpeed = 255;
    }
    Serial.print("Increase motor: \"" + dcmotor.name + "\" speed. Motor speed: ");
    Serial.println(dcmotor.motorSpeed);
  }else if(keypressed == key_minus){
    // Decrease speed
    dcmotor.motorSpeed = dcmotor.motorSpeed - dcmotor.dspeed;
    if(dcmotor.motorSpeed < 0){
      dcmotor.motorSpeed = 0;
    }
    Serial.print("Decrease motor: \"" + dcmotor.name + "\" speed. Motor speed: ");
    Serial.println(dcmotor.motorSpeed);
  }
}


void receivedValue(int received){
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
  //Serial.print(received, HEX);
  //Serial.print(" ");
  //Serial.print(received);
  //Serial.print(" ");
  Serial.println("Key pressed: " + out);
}
