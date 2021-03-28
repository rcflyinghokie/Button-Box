#include <Keypad.h>
#include <Joystick.h>

#define NUM_BUTTONS 17
#define NUM_ROWS 3
#define NUM_COLS 6

// define the symbols on the buttons of the keypads
byte buttons[NUM_ROWS][NUM_COLS] = {
  {0,1,2,3,4,5},
  {6,7,8,9,10,11},
  {12,13,14,15,16,17},
};

// connect to the row pinouts of the keypad
byte rowPins[NUM_ROWS] = {21,20};

// connect to the column pinouts of the keypad
byte colPins[NUM_COLS] = {16,15,14,10,9,8};

// set up map for kcode to kchar
const int KCODES_SIZE = NUM_ROWS * NUM_COLS;
Key kcodes[KCODES_SIZE] = {};

Keypad buttbx = Keypad(makeKeymap(buttons), rowPins, colPins, NUM_ROWS, NUM_COLS);

// Create the Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  NUM_BUTTONS, 0,                  // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

String msg;

void setup() {
  msg = "";

  // Initialize Joystick Library
  Joystick.begin();

  if (buttbx.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) { // scan whole list
      // Key key = buttbx.key[i];

      kcodes[buttbx.key[i].kcode] = buttbx.key[i];
    }
  }
}

void loop() {
  checkAllButtons();
  delay(50);
}

void checkAllButtons(void) {
  if (buttbx.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) { // scan whole list
      Key key = buttbx.key[i];

      if (key.stateChanged) { // only find keys that have changed state
        switch (key.kstate) { // report active key state
          case PRESSED:
            msg = " PRESSED.";
            Joystick.setButton(key.kchar, 1);
            break;

          case HOLD:
            msg = " HOLD.";
            Joystick.setButton(key.kchar, 1);
            break;

          case RELEASED:
            msg = " RELEASED.";
            Joystick.setButton(key.kchar, 0);

            if (key.kcode == 9 || key.kcode == 6)
            {
              Serial.print("Pressing kcode: ");
              Serial.println(kcodes[10].kcode);
              Joystick.setButton(kcodes[10].kchar, 1);
            }

            break;

          case IDLE:
            msg = " IDLE.";
            Joystick.setButton(key.kchar, 0);
        }

        char str[21]; // enough to hold all numbers up to 64-bits
        Serial.print("Key[");
        Serial.print(sprintf(str, "%d", i));
        Serial.print("] ");
        Serial.print(key.kcode);
        Serial.println(msg);
      }
    }
  }
}
