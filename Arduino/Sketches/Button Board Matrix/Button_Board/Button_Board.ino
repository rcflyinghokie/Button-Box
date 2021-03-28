//Matrix Code (Button Board)

// Keypad - Version: Latest
#include <Key.h>
#include <Keypad.h>

// MHeironimus Joystick - Version: 2.0
#include <Joystick.h>

#define ENABLE_PULLUPS
#define LIST_MAX 30             // Max number of keys on the active list.
#define MAPSIZE 10              // MAPSIZE is the number of rows (times 16 columns)
#define NUMBUTTONS 30
#define NUMROWS 6
#define NUMCOLS 5


byte buttons[NUMROWS][NUMCOLS] = {  //Matrix defining button numbers as seen by Windows, 0 is button 1 and so forth
  {0, 1, 2, 3, 4},
  {5, 6, 7, 8, 9},
  {10, 11, 12, 13, 14},
  {15, 16, 17, 18, 19},
  {20, 21, 22, 23, 24},
  {25, 26, 27, 28, 29},

};

byte rowPins[NUMROWS] = {2, 3, 4, 5, 6, 7};
byte colPins[NUMCOLS] = {21, 20, 19, 18, 15};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, 30, 0,
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
