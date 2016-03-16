#define ROBOT_STATE_0 0
#define ROBOT_STATE_1 1
#define ROBOT_STATE_2 2
#define ROBOT_STATE_3 3
#define ROBOT_STATE_4 4
#define ROBOT_STATE_RESET 5
#define ROBOT_STATE_PAUSED 6

#define DOWNWARD FORWARD
#define UPWARD BACKWARD

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

const int stateMachineMotorControlEnabledLedPin = 39;

const int state0LedPin = 41;
const int state1LedPin = 43;
const int state2LedPin = 45;
const int state3LedPin = 47;
const int state4LedPin = 49;
const int stateResetLedPin = 51;
const int statePausedLedPin = 53;

const boolean stateDelayOn = false;
const int stateDelayDurationMs = 500;
boolean stateReportingOn = false;
boolean localStateReportingOn = false;
long stateReportingTimer = 0;
long stateReportingTimerLimit = 10000;

boolean stateMachineAllowedControlOfMotors = false;

int currentRobotState = ROBOT_STATE_0;
int lastRobotState = ROBOT_STATE_0;

// Pæling að upphasstilla þetta einhvernveginn??
boolean frontLegSwitchPressed = false;
boolean backLegSwitchPressed = false;
boolean weightSwitchPressed = false;

IRrecv irRemote(irRemotePin);
decode_results irRemoteResults;
unsigned long irRemoteKeyPressed;

DCMotor frontLegMotor, backLegMotor, weightMotor;
DCMotor* currentlySelectedMotor;
