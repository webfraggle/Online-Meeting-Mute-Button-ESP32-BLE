/*
  StateKeyboard
*/
#ifndef Config_h
#define Config_h

#include "Arduino.h"

struct KEYOPTION{
  String description;
  char keys[5];
};

class Config
{
  public:
    Config();
    void loadCurrentOption();
    void next();
    void begin();
    KEYOPTION currentOption;
    KEYOPTION _configOptions[13];
    int currentOptionID;

  private:
    
    
};

#endif
