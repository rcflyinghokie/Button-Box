//Center Switch Panel Code

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMBUTTONS 10
#define NUMROWS 5
#define NUMCOLS 2


byte buttons[NUMROWS][NUMCOLS] = {  //Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth
  {0, 1},
  {2, 3},
  {4, 5},
  {6, 7},
  {8, 9},

};


byte rowPins[NUMROWS] = {2, 3, 4, 5, 6};
byte colPins[NUMCOLS] = {7, 8};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, 10, 0,
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
