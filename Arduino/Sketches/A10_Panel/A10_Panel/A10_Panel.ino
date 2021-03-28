//A-10C Panel Code

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMBUTTONS 19
#define NUMROWS 7
#define NUMCOLS 3


byte buttons[NUMROWS][NUMCOLS] = {  //Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth
  {0, 6, 13},
  {1, 7, 14},
  {2, 8, 15},
  {3, 9, 16},
  {4, 10, 17},
  {32, 11, 18},
  {5, 12, 32},

};


byte rowPins[NUMROWS] = {9, 8, 7, 6, 5, 4, 3};
byte colPins[NUMCOLS] = {21, 20, 19};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, 19, 0,
                   false, false, false, false, false, false,
                   false, false, false, false, false);

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
            break;
        }
      }
    }
  }
}
