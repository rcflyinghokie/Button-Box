//Breaker Panel Code

// MHeironimus Joystick - Version: 2.0.7
#include <Joystick.h>

//#define ENABLE_PULLUPS
#define NUMBUTTONS 4


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, NUMBUTTONS, 0,
                   false, false, false, false, false, false,
                   false, false, false, false, false);

void setup() {
  // Initialize Button Pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
}

// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap = 2;

// Last state of the button
int lastButtonState[4] = {0,0,0,0};

void loop() {

  // Read pin values
  for (int index = 0; index < NUMBUTTONS; index++)
  {
    int currentButtonState = !digitalRead(index + pinToButtonMap);
    if (currentButtonState != lastButtonState[index])
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
    }
  }

  delay(50);
}
