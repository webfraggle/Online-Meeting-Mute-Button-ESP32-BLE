/*
  StateKeyboard
*/
#ifndef Config_h
#define Config_h

#include "Arduino.h"

#define NUM_OPTIONS 16
#define NUM_BUTTONS 4

typedef struct {
  String description;
  char keys[5];
} KEYOPTION;

class Config
{
  public:
    Config();
    void loadCurrentOption();
    KEYOPTION getOptionById(uint8_t btnID);
    void saveConfigForBtn(int btnId, int optionId);
    void next();
    void begin();
    String getConfigJSON();
    KEYOPTION currentOption;
    KEYOPTION _configOptions[NUM_OPTIONS];
    byte currentOptionID;

  private:
    
    
};

#endif
