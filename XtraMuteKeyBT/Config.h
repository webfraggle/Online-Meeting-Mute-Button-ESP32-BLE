/*
  StateKeyboard
*/
#ifndef Config_h
#define Config_h

#include "Arduino.h"

#define NUM_OPTIONS 14
#define NUM_BUTTONS 1

typedef struct {
  String description;
  char keys[5];
} KEYOPTION;

class Config
{
  public:
    Config();
    void loadCurrentOption();
    void next();
    void begin();
    String getConfigJSON();
    KEYOPTION currentOption;
    KEYOPTION _configOptions[NUM_OPTIONS];
    int currentOptionID;

  private:
    
    
};

#endif
