/*
  StateKeyboard
*/
#ifndef StateKeyboard_h
#define StateKeyboard_h

#include "Arduino.h"
#include <BleKeyboard.h>

class StateKeyboard
{
  public:
    StateKeyboard(BleKeyboard *bleKeyboard);
    void keyPressed();
    void keyReleased();
    void executeKeyPress();
  private:
    BleKeyboard *_bleKeyboard;
};

#endif
