/*
  StateConfig
*/
#ifndef StateConfig_h
#define StateConfig_h

#include "Arduino.h"
#include <BleKeyboard.h>
#include "Config.h"

class StateConfig
{
  public:
    StateConfig(BleKeyboard *bleKeyboard, Config *myconfig);
    void executeKey(uint8_t id);
    void startMode();
    void endMode();
  private:
    BleKeyboard *_bleKeyboard;
    Config *_config;
    void printString(String str);
};


#endif
