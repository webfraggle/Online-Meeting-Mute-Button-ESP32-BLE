/*
  StateKeyboard
*/
#ifndef StateKeyboard_h
#define StateKeyboard_h

#include "Arduino.h"
#include <BleKeyboard.h>
#include "Config.h"

class StateKeyboard
{
  public:
    StateKeyboard(BleKeyboard *bleKeyboard, Config *myconfig);
    void executeKey(uint8_t id);
  private:
    BleKeyboard *_bleKeyboard;
    Config *_config;
};


#endif
