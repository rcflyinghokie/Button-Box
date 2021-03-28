//Used to test the button functions

#include <Joystick.h>


#define ENABLE_PULLUPS
#define NUMROTARIES 4


const int outputPinStart = 2; // Constant that maps output pin start on the micro.
const int inputPinStart = 6; // Constant that maps input pin start on the micro.
int currentButtonState; // Declares the currentButtonState variable
int lastButtonState[4][4] = { // Last state of the button
{0,0,0,0},
{0,0,0,0},
{0,0,0,0},
{0,0,0,0}};
int buttonValue[4][4] = { // Controller button number
{0,1,2,3},
{4,5,6,7},
{8,9,10,11},
{12,13,14,15}};


struct rotariesdef {
  byte pin1;
  byte pin2;
  int ccwchar;
  int cwchar;
  volatile unsigned char state;
};

rotariesdef rotaries[NUMROTARIES] {
  {0,1},
  {4,5},
  {6,9},
  {10,11},
};

#define DIR_CCW 0x10
#define DIR_CW 0x20
#define R_START 0x0

#ifdef HALF_STEP
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};
#else
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   31, 0, //Button count, HAT Switch Count
                   false, false, true, //X,Y,Z Axis
                   true, true, true, //X,Y,Z Rotation
                   false, false, //Rudder, Throttle
                   false, false, false); //Accelerator, Brake, Steering

//int CurrentXAxisStep;
//int CurrentYAxisStep;
unsigned int CurrentZAxisStep;
unsigned int CurrentRxAxisStep;
unsigned int CurrentRyAxisStep;
unsigned int CurrentRzAxisStep;
//unsigned int CurrentThrottleAxisStep;
//unsigned int CurrentRudderAxisStep;


void setup() {

//int CurrentXAxisStep = 0;
//int CurrentYAxisStep = 0;
unsigned int CurrentZAxisStep = 0;
unsigned int CurrentRxAxisStep = 0;
unsigned int CurrentRyAxisStep = 0;
unsigned int CurrentRzAxisStep = 0;
//unsigned int CurrentThrottleAxisStep = 0;
//unsigned int CurrentRudderAxisStep = 0;
  
  Joystick.begin();

  // Set Range Values
  //Joystick.setXAxisRange(-127, 127);
  //Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(0, 360);
  Joystick.setRxAxisRange(0, 360);
  Joystick.setRyAxisRange(0, 360);
  Joystick.setRzAxisRange(0, 360);
  //Joystick.setThrottleRange(0, 255);
  //Joystick.setRudderRange(255, 0);

{
for (int x = 2; x < 7; x++) // Initialize output pins
{
pinMode(x, OUTPUT);
}
for (int x = 6; x < 10; x++) // Initialize input pins
{
pinMode(x, INPUT_PULLUP);
}

Joystick.begin(); // Initialize Joystick Library
}

  rotary_init();

}

void loop() {

  CheckAllEncoders();

  //CheckAllButtons();

}

void CheckAllButtons(void) {
{
for (int incRow = 0; incRow < 4; incRow++) // Loops through the output rows
{
for (int incCol = 0; incCol < 4; incCol++) // Loops through the output rows
{
digitalWrite(incRow + outputPinStart, LOW); // Writes the output rows with +5v
currentButtonState = !digitalRead(incCol + inputPinStart); // Checks the state of the input pin
if (currentButtonState != lastButtonState[incRow][incCol]) // Checks if the currentButtonState is the same as the lastButtonState
{
Joystick.setButton(buttonValue[incRow][incCol],currentButtonState); // Activates the contrller button
lastButtonState[incRow][incCol] = currentButtonState; // Saves the currentButtonState as the lastButtonState
}
digitalWrite((incRow + outputPinStart), HIGH); // Writes the output rows with 0v
}
delay(50);
}
}
}


void rotary_init() {
  for (int i = 0; i < NUMROTARIES; i++) {
    pinMode(rotaries[i].pin1, INPUT);
    pinMode(rotaries[i].pin2, INPUT);
#ifdef ENABLE_PULLUPS
    digitalWrite(rotaries[i].pin1, HIGH);
    digitalWrite(rotaries[i].pin2, HIGH);
#endif
  }
}


unsigned char rotary_process(int _i) {
  unsigned char pinstate = (digitalRead(rotaries[_i].pin2) << 1) | digitalRead(rotaries[_i].pin1);
  rotaries[_i].state = ttable[rotaries[_i].state & 0xf][pinstate];
  return (rotaries[_i].state & 0x30);
}

void CheckAllEncoders(void) {
  for (int i = 0; i < NUMROTARIES; i++) {
    unsigned char result = rotary_process(i);
    
       //Z Axis
    
    if (result == DIR_CCW && i == 0) {
      if (CurrentZAxisStep >= 360){   //Stop incrementing
        CurrentZAxisStep = 360;
      }
      else {
      CurrentZAxisStep = (CurrentZAxisStep + 10); //Increments axis
      }
      
      Joystick.setZAxis(CurrentZAxisStep);  //Sets new axis state
    };
    
    if (result == DIR_CW && i == 0) {
      if (CurrentZAxisStep <= 0){  //Stop incrementing
        CurrentZAxisStep = 0;
      }
      else {
      CurrentZAxisStep = (CurrentZAxisStep - 10); //Increments axis
      }
      
      Joystick.setZAxis(CurrentZAxisStep);  //Sets new axis state
    };

      //Rx Axis
   
    if (result == DIR_CCW && i == 1) {
      if (CurrentRxAxisStep >= 360){   //Stop incrementing
        CurrentRxAxisStep = 360;
      }
      else {
      CurrentRxAxisStep = (CurrentRxAxisStep + 10); //Increments axis
      }
      
      Joystick.setRxAxis(CurrentRxAxisStep);  //Sets new axis state
    };
    
    if (result == DIR_CW && i == 1) {
      if (CurrentRxAxisStep <= 0){  //Stop incrementing
        CurrentRxAxisStep = 0;
      }
      else {
      CurrentRxAxisStep = (CurrentRxAxisStep - 10); //Increments axis
      }
      
      Joystick.setRxAxis(CurrentRxAxisStep);  //Sets new axis state
    };

       //Ry Axis
   
    if (result == DIR_CCW && i == 2) {
      if (CurrentRyAxisStep >= 360){   //Stop incrementing
        CurrentRyAxisStep = 360;
      }
      else {
      CurrentRyAxisStep = (CurrentRyAxisStep + 10); //Increments axis
      }
      
      Joystick.setRyAxis(CurrentRyAxisStep);  //Sets new axis state
    };
    
    if (result == DIR_CW && i == 2) {
      if (CurrentRyAxisStep <= 0){  //Stop incrementing
        CurrentRyAxisStep = 0;
      }
      else {
      CurrentRyAxisStep = (CurrentRyAxisStep - 10); //Increments axis
      }
      
      Joystick.setRyAxis(CurrentRyAxisStep);  //Sets new axis state
    };

        //Rz Axis
   
    if (result == DIR_CCW && i == 3) {
      if (CurrentRzAxisStep >= 360){   //Stop incrementing
        CurrentRzAxisStep = 360;
      }
      else {
      CurrentRzAxisStep = (CurrentRzAxisStep + 10); //Increments axis
      }
      
      Joystick.setRzAxis(CurrentRzAxisStep);  //Sets new axis state
    };
    
    if (result == DIR_CW && i == 3) {
      if (CurrentRzAxisStep <= 0){  //Stop incrementing
        CurrentRzAxisStep = 0;
      }
      else {
      CurrentRzAxisStep = (CurrentRzAxisStep - 10); //Increments axis
      }
      
      Joystick.setRzAxis(CurrentRzAxisStep);  //Sets new axis state
    };
  }
}
