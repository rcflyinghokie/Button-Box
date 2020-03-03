//Main Panel Code

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMROTARIES 4
#define NUMBUTTONS 36
#define NUMROWS 6
#define NUMCOLS 6


byte buttons[NUMROWS][NUMCOLS] = {  //Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth
  {28, 24, 13, 7, 20, 0},
  {27, 25, 14, 8, 21, 1},
  {30, 26, 15, 9, 22, 2},
  {0, 29, 16, 10, 23, 3},  //0 is dummy placeholder
  {0, 0, 17, 11, 5, 4},   //0 is dummy placeholder
  {0, 0, 18, 12, 6, 19},  //0 is dummy placeholder
};

struct rotariesdef {
  byte pin1;
  byte pin2;
  int ccwchar;
  int cwchar;
  volatile unsigned char state;
};

rotariesdef rotaries[NUMROTARIES] {  //Physical pin 1, pin 3 of rotary encoders (pin 2 is ground)
  {0, 1},
  {4, 5},
  {6, 9},
  {10, 11},
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

byte rowPins[NUMROWS] = {13, 12, 8, 7, 3, 2};  //Physical pins heading each row
byte colPins[NUMCOLS] = {A5, A4, A3, A2, A1, A0};  //Pysical pins heading each column

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

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


  // Set Range Values
  //Joystick.setXAxisRange(-127, 127);
  //Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(0, 360);
  Joystick.setRxAxisRange(0, 360);
  Joystick.setRyAxisRange(0, 360);
  Joystick.setRzAxisRange(0, 360);
  //Joystick.setThrottleRange(0, 255);
  //Joystick.setRudderRange(255, 0);

  Joystick.begin();
  
  rotary_init();

}

void loop() {

  CheckAllEncoders();

  CheckAllButtons();

}

void CheckAllButtons(void) {
  if (buttbx.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if ( buttbx.key[i].stateChanged )
      {
        switch (buttbx.key[i].kstate) {
          case PRESSED:
          case HOLD:
            Joystick.setButton(buttbx.key[i].kchar, 1);
            break;
          case RELEASED:
          case IDLE:
            Joystick.setButton(buttbx.key[i].kchar, 0);
            break;
        }
      }
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
      if (CurrentZAxisStep >= 360) {  //Stop incrementing
        CurrentZAxisStep = 360;
      }
      else {
        CurrentZAxisStep = (CurrentZAxisStep + 10); //Increments axis
      }

      Joystick.setZAxis(CurrentZAxisStep);  //Sets new axis state
    };

    if (result == DIR_CW && i == 0) {
      if (CurrentZAxisStep <= 0) { //Stop incrementing
        CurrentZAxisStep = 0;
      }
      else {
        CurrentZAxisStep = (CurrentZAxisStep - 10); //Increments axis
      }

      Joystick.setZAxis(CurrentZAxisStep);  //Sets new axis state
    };

    //Rx Axis

    if (result == DIR_CCW && i == 1) {
      if (CurrentRxAxisStep >= 360) {  //Stop incrementing
        CurrentRxAxisStep = 360;
      }
      else {
        CurrentRxAxisStep = (CurrentRxAxisStep + 10); //Increments axis
      }

      Joystick.setRxAxis(CurrentRxAxisStep);  //Sets new axis state
    };

    if (result == DIR_CW && i == 1) {
      if (CurrentRxAxisStep <= 0) { //Stop incrementing
        CurrentRxAxisStep = 0;
      }
      else {
        CurrentRxAxisStep = (CurrentRxAxisStep - 10); //Increments axis
      }

      Joystick.setRxAxis(CurrentRxAxisStep);  //Sets new axis state
    };

    //Ry Axis

    if (result == DIR_CCW && i == 2) {
      if (CurrentRyAxisStep >= 360) {  //Stop incrementing
        CurrentRyAxisStep = 360;
      }
      else {
        CurrentRyAxisStep = (CurrentRyAxisStep + 10); //Increments axis
      }

      Joystick.setRyAxis(CurrentRyAxisStep);  //Sets new axis state
    };

    if (result == DIR_CW && i == 2) {
      if (CurrentRyAxisStep <= 0) { //Stop incrementing
        CurrentRyAxisStep = 0;
      }
      else {
        CurrentRyAxisStep = (CurrentRyAxisStep - 10); //Increments axis
      }

      Joystick.setRyAxis(CurrentRyAxisStep);  //Sets new axis state
    };

    //Rz Axis

    if (result == DIR_CCW && i == 3) {
      if (CurrentRzAxisStep >= 360) {  //Stop incrementing
        CurrentRzAxisStep = 360;
      }
      else {
        CurrentRzAxisStep = (CurrentRzAxisStep + 10); //Increments axis
      }

      Joystick.setRzAxis(CurrentRzAxisStep);  //Sets new axis state
    };

    if (result == DIR_CW && i == 3) {
      if (CurrentRzAxisStep <= 0) { //Stop incrementing
        CurrentRzAxisStep = 0;
      }
      else {
        CurrentRzAxisStep = (CurrentRzAxisStep - 10); //Increments axis
      }

      Joystick.setRzAxis(CurrentRzAxisStep);  //Sets new axis state
    };
  }
}
