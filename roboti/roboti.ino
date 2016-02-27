// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>
#include <IRremote.h>

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

AF_DCMotor motor(1);

const int RECV_PIN = 31;
IRrecv irrecv(RECV_PIN);
decode_results results;


int inByte = 0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hello!");


  irrecv.enableIRIn(); // Start the receiver
  // turn on motor
  motor.setSpeed(255);
 
  motor.run(RELEASE);
}

boolean forward = true;
boolean ison = false;
void loop() {
  //uint8_t i;
  
  if (irrecv.decode(&results)) {
    String out = "";
    int keypressed = results.value;
    receivedValue(results.value);
    if(keypressed == key_play){
      if(ison){
        out = "motor stop";
        ison = !ison;
        motor.run(RELEASE);
      }else{
        ison = !ison;
        if(forward){
          out = "motor forward";
          motor.run(FORWARD);
        }else{
          out = "motor backward";
          motor.run(BACKWARD);
        }
      }
    }else if(keypressed == key_forward){
      if(ison){
        out = "motor forward";
        forward = true;
        motor.run(FORWARD);
      }
    }else if(keypressed == key_back){
      if(ison){
        out = "motor backward";
        forward = false;
        motor.run(BACKWARD);
      }
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}


String receivedValue(int received){
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
  Serial.println(out);
}
