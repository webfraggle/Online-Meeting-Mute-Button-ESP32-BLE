#include "Arduino.h"
#include "Config.h"
#include <BleKeyboard.h>


Config::Config()
{
  
  //_configOptions[13];
  //currentOption;
  currentOptionID = 1;
  int i = 0;
  _configOptions[i].description = "Win Teams ToggleMute Ctrl Shift M";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'M';
  i++;
  _configOptions[i].description = "Mac Teams ToggleMute Cmd Shift M";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'M';
  
}

void Config::loadCurrentOption()
{
//  Serial.println("loadCurrentOption");
//  Serial.println(currentOptionID);
//  Serial.println(_configOptions[0].description);
//  Serial.println(_configOptions[0].keys[0]);
//  Serial.println(_configOptions[0].keys[1]);
//  Serial.println(_configOptions[0].keys[2]);
//  Serial.println(sizeof(_configOptions[0].keys));
//  Serial.println(sizeof(_configOptions[0].keys[0]));
//  Serial.println("---");
  currentOption = _configOptions[currentOptionID];
  //return currentOption;
}
