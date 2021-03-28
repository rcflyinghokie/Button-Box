//On On Test Code

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMBUTTONS 8
#define NUMROWS 1
#define NUMCOLS 6


//Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth, "99" is a "placeholder" position
byte buttons[NUMROWS][NUMCOLS] = {
  {0, 2, 4, 6, 8, 10},
};

// connect to the row pinouts of the keypad
byte rowPins[NUMROWS] = {A3};

// connect to the column pinouts of the keypad
byte colPins[NUMCOLS] = {A2, A1, A0, 15, 14, 16};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   NUMBUTTONS, 0,                  // Button Count, Hat Switch Count
                   false, false, false,        // No X Y Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

int TwoPosSwitch[] = {0, 2, 4, 6}; // List of all 2 position switch buttons wired
int ThreePosSwitch[] = {0, 2, 3, 5, 10, 12}; // List of all 3 position switch buttons wired
int ButtonLookup = 0;

void setup() {
  Joystick.begin();
}

void loop() {
  CheckAllButtons();
}

void CheckAllButtons(void) {
  if (buttbx.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      ButtonLookup = ((buttbx.key[i].kchar) + 1);
      if ( buttbx.key[i].stateChanged && (buttbx.key[i].kchar == 0 || buttbx.key[i].kchar == 2 || buttbx.key[i].kchar == 4))
      {
        switch (buttbx.key[i].kstate) {
          case PRESSED:

          case HOLD:
            Joystick.setButton(buttbx.key[i].kchar, 1);
            Joystick.setButton((ButtonLookup), 0);
            break;
          case RELEASED:

          case IDLE:
            Joystick.setButton(buttbx.key[i].kchar, 0);
            Joystick.setButton((ButtonLookup), 1);
            break;
        }
      }
      else if ( buttbx.key[i].stateChanged && (buttbx.key[i].kchar == 6))
      {
        switch (buttbx.key[i].kstate) {
          case PRESSED:

          case HOLD:
            Joystick.setButton(buttbx.key[i].kchar, 1);
            //Joystick.releaseButton((ButtonLookup));
            break;
          case RELEASED:

          case IDLE:
            Joystick.setButton(buttbx.key[i].kchar, 0);
            //Joystick.setButton((ButtonLookup), 1);
            break;
        }
      }
    }
  }
}
