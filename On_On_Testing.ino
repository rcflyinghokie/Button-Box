//On On Test Code

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMBUTTONS 17
#define NUMROWS 2
#define NUMCOLS 6


//Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth, "99" is a "placeholder" position
byte buttons[NUMROWS][NUMCOLS] = {
  {0, 2, 3, 5, 6, 8},
  {10, 12, 13, 15, 99, 99},
};

// connect to the row pinouts of the keypad
byte rowPins[NUMROWS] = {A3, A2};

// connect to the column pinouts of the keypad
byte colPins[NUMCOLS] = {16, 10, 15, 14, 8, 9};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   NUMBUTTONS, 0,                  // Button Count, Hat Switch Count
                   false, false, false,        // No X Y Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

int TwoPosSwitch[] = {6, 8, 13, 14}; // List of all 2 position switch buttons wired
int ThreePosSwitch[] = {0, 2, 3, 5, 10, 12}; // List of all 3 position switch buttons wired

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
  
            if (TwoPosSwitch = i)
            {
              Joystick.setButton(buttbx.key[i + 1].kchar, 1);
            }
            break;
        }
      }
    }
  }
}
