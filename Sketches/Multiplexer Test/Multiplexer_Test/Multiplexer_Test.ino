

//Multiplexer Test
#include <CD74HC4067.h>
#include <Joystick.h>

#define NUMBUTTONS 3

CD74HC4067 my_mux(10, 16, 14, 15);  // create a new CD74HC4067 object with its four control pins

const int common_pin = A0; // select a pin to share with the 16 channels of the CD74HC4067

//Set up data buffer.
uint8_t dataBuffer[3];

//Set up length constant and array lengths.
const uint8_t inputCount = sizeof(dataBuffer);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, NUMBUTTONS, 0,
                   false, false, false, false, false, false,
                   false, false, false, false, false);

void setup() {
  // Initialize Button Pins
  pinMode(common_pin, INPUT); // set the initial mode of the common pin.

  // Initialize Joystick Library
  Joystick.begin();
}

void loop()
{
  // loop through the multiplexer channels, reading each one, and putting int into dataBuffer.
  for (uint8_t i = 0; i < inputCount; i++) {
    my_mux.channel(i);
    //delay(50);
    dataBuffer[i] = digitalRead(i);
  }

  //loop through dataBuffer
  for (uint8_t u = 0; u < inputCount; u++) {
    if (dataBuffer[u] == 0)
    {

      Joystick.pressButton(u);

    }
    else
    {

      Joystick.releaseButton(u);

    }
  }
}
