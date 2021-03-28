//On On Test Code

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMBUTTONS 8
#define NUMROWS 1
#define NUMCOLS 4


//Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth, "99" is a "placeholder" position
byte buttons[NUMROWS][NUMCOLS] = {
  {0, 2, 4, 6},
};

// connect to the row pinouts of the keypad
byte rowPins[NUMROWS] = {A3};

// connect to the column pinouts of the keypad
byte colPins[NUMCOLS] = {A2, A1, A0, 15};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   NUMBUTTONS, 0,                  // Button Count, Hat Switch Count
                   false, false, false,        // No X Y Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

int TwoPosSwitch[] = {0, 2, 4, 6}; // List of all 2 position switch buttons wired
//int ThreePosSwitch[] = {0, 2, 3, 5, 10, 12}; // List of all 3 position switch buttons wired

void setup() {
  Joystick.begin();
}

void loop() {
  CheckAllButtons();
  //CheckTwoPos();
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
            Joystick.setButton(i + 1, 0);
          case HOLD:
            Joystick.setButton(buttbx.key[i].kchar, 1);
            break;
          case RELEASED:
            Joystick.setButton(i + 1, 1);
          case IDLE:
            Joystick.setButton(buttbx.key[i].kchar, 0);
            break;
        }
      }
    }
  }
}
