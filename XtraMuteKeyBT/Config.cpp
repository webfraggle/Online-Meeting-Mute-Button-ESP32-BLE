#include "Arduino.h"
#include "Config.h"
#include "debug.h"
#include <BleKeyboard.h>
#include <EEPROM.h>

Config::Config()
{
  
  //_configOptions[13];
  //currentOption;
  
  int i = 0;
  _configOptions[i].description = "Just Key A";
  _configOptions[i].keys[0] = 'A';
  //_configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  //_configOptions[i].keys[2] = 'M';
  
  i++;
  _configOptions[i].description = "Win Teams ToggleMute Ctrl Shift M";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Mac Teams ToggleMute Cmd Shift M";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Win Zoom ToggleMute Alt A";
  _configOptions[i].keys[0] = KEY_LEFT_ALT;
  _configOptions[i].keys[1] = 'a';
  i++;
  _configOptions[i].description = "Mac Zoom ToggleMute Cmd Shift A";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'a';

    i++;
  _configOptions[i].description = "Win Skype Webex ToggleMute Ctrl M";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = 'm';
    i++;
  _configOptions[i].description = "Mac Skype Webex ToggleMute Cmd Shift M";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';

  i++;
  _configOptions[i].description = "Win GoogleMeet ToggleMute Ctrl D";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = 'd';
  i++;
  _configOptions[i].description = "Mac GoogleMeet ToggleMute Cmd D";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = 'd';

  i++;
  _configOptions[i].description = "Win Mac Jitsi ToggleMute M";
  _configOptions[i].keys[0] = 'm';

  i++;
  _configOptions[i].description = "Win BigBlueButton ToggleMute Alt Shift M";
  _configOptions[i].keys[0] = KEY_LEFT_ALT;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';

    i++;
  _configOptions[i].description = "Mac BigBlueButton ToggleMute Ctrl Option M";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_ALT;
  _configOptions[i].keys[2] = 'm';

  i++;
  _configOptions[i].description = "Win Lock Screen Win L";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = 'l';

      i++;
  _configOptions[i].description = "Mac Lock Screen Ctrl Cmd Q";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_GUI;
  _configOptions[i].keys[2] = 'q';
  
}

void Config::begin()
{
  EEPROM.begin(1);
  loadCurrentOption();
}
void Config::next()
{
  SERIAL_DEBUG_LN("next");
  SERIAL_DEBUG_LN(currentOptionID);
  currentOptionID = currentOptionID+1;
  SERIAL_DEBUG_LN(currentOptionID);
  EEPROM.write(0, currentOptionID);
  EEPROM.commit();
  delay(10);
  loadCurrentOption();
  SERIAL_DEBUG_LNF("New option %d",currentOptionID);
  SERIAL_DEBUG_LN(currentOption.description); 
}

void Config::loadCurrentOption()
{
//  Serial.println("loadCurrentOption");
   currentOptionID = EEPROM.read(0);
   SERIAL_DEBUG_LNF("after eeprom read %d",currentOptionID);
   delay(10);
   currentOption.keys[0] = 0;
   currentOption.keys[1] = 0;
   currentOption.keys[2] = 0;
   currentOption.keys[3] = 0;
   currentOption.keys[4] = 0;
   currentOption = _configOptions[currentOptionID];
   
   if (currentOption.keys[0] == NULL)
    {
      SERIAL_DEBUG_LN("No correct item loaded");
      currentOptionID = 0;
      currentOption = _configOptions[currentOptionID];
      EEPROM.write(0, currentOptionID);
      EEPROM.commit();
      delay(10);
    }
//  Serial.println(currentOptionID);
//  Serial.println(_configOptions[0].description);
//  Serial.println(_configOptions[0].keys[0]);
//  Serial.println(_configOptions[0].keys[1]);
//  Serial.println(_configOptions[0].keys[2]);
//  Serial.println(sizeof(_configOptions[0].keys));
//  Serial.println(sizeof(_configOptions[0].keys[0]));
//  Serial.println("---");
  
}
