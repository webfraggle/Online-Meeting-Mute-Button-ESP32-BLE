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
  _configOptions[i].description = "Win Teams: Toggle Mute 'Ctrl Shift M'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Win Teams: Toggle Video 'Ctrl Shift O'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'o';
  i++;
  _configOptions[i].description = "Win Teams: End Call 'Ctrl Shift H'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'h';
  i++;
  _configOptions[i].description = "Win Zoom: Toggle 'Mute Alt A'";
  _configOptions[i].keys[0] = KEY_LEFT_ALT;
  _configOptions[i].keys[1] = 'a';
  i++;
  _configOptions[i].description = "Win Skype/Webex: Toggle Mute 'Ctrl M'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = 'm';
  i++;
  _configOptions[i].description = "Win GoogleMeet: Toggle Mute 'Ctrl D'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = 'd';
  i++;
  _configOptions[i].description = "Win Mac Jitsi: Toggle 'Mute M'";
  _configOptions[i].keys[0] = 'm';
  i++;
  _configOptions[i].description = "Win BigBlueButton: Toggle Mute Alt 'Shift M'";
  _configOptions[i].keys[0] = KEY_LEFT_ALT;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Win: Lock Screen 'Win L'";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = 'l';
  i++;
  _configOptions[i].description = "Mac Teams: Toggle Mute 'Cmd Shift M'";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Mac Zoom: Toggle Mute 'Cmd Shift A'";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'a';
  i++;
  _configOptions[i].description = "Mac Skype/Webex: Toggle Mute 'Cmd Shift M'";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = KEY_LEFT_SHIFT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Mac GoogleMeet: Toggle Mute 'Cmd D'";
  _configOptions[i].keys[0] = KEY_LEFT_GUI;
  _configOptions[i].keys[1] = 'd';
  i++;
  _configOptions[i].description = "Mac BigBlueButton: Toggle Mute 'Ctrl Option M'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_ALT;
  _configOptions[i].keys[2] = 'm';
  i++;
  _configOptions[i].description = "Mac: Lock Screen 'Ctrl Cmd Q'";
  _configOptions[i].keys[0] = KEY_LEFT_CTRL;
  _configOptions[i].keys[1] = KEY_LEFT_GUI;
  _configOptions[i].keys[2] = 'q';
  
}

void Config::begin()
{
  EEPROM.begin(NUM_BUTTONS);
  loadCurrentOption();
}
void Config::next()
{
  SERIAL_DEBUG_LN("next");
  SERIAL_DEBUG_LN(currentOptionID);
  currentOptionID = currentOptionID+1;
  SERIAL_DEBUG_LN(currentOptionID);
  if (currentOptionID >= NUM_OPTIONS)
  {
    currentOptionID = 0;
  }
  EEPROM.write(0, currentOptionID);
  EEPROM.commit();
  delay(10);
  loadCurrentOption();
  SERIAL_DEBUG_LNF("New option %d",currentOptionID);
  SERIAL_DEBUG_LN(currentOption.description); 
}

KEYOPTION Config::getOptionById(uint8_t btnID)
{
  byte optionID = EEPROM.read(btnID);
  return _configOptions[optionID];
}
void Config::saveConfigForBtn(int btnId, int optionId)
{
  EEPROM.write(btnId, optionId);
  EEPROM.commit();
  delay(10);
}

        
void Config::loadCurrentOption()
{
//  Serial.println("loadCurrentOption");
   currentOptionID = EEPROM.read(0);
   if (currentOptionID >= NUM_OPTIONS)
   {
      SERIAL_DEBUG_LN("No correct item loaded");
      currentOptionID = 0;
      EEPROM.write(0, currentOptionID);
      EEPROM.commit();
      delay(10);
   }
   SERIAL_DEBUG_LNF("after eeprom read %d",currentOptionID);
   delay(10);
   currentOption.keys[0] = 0;
   currentOption.keys[1] = 0;
   currentOption.keys[2] = 0;
   currentOption.keys[3] = 0;
   currentOption.keys[4] = 0;
   currentOption = _configOptions[currentOptionID];
   
//  Serial.println(currentOptionID);
//  Serial.println(_configOptions[0].description);
//  Serial.println(_configOptions[0].keys[0]);
//  Serial.println(_configOptions[0].keys[1]);
//  Serial.println(_configOptions[0].keys[2]);
//  Serial.println(sizeof(_configOptions[0].keys));
//  Serial.println(sizeof(_configOptions[0].keys[0]));
//  Serial.println("---");
  
}

String Config::getConfigJSON()
{
  String json = "{\"options\":[";
  for (uint8_t i = 0; i < NUM_OPTIONS; i++) {
     json += "{\"description\":\"" + _configOptions[i].description + "\",\"index\":" + i + "}";
     if (i < NUM_OPTIONS - 1)
      json += ",";
  }
  json += "]";
  json += ",\"settings\":{\"numButtons\":"+String(NUM_BUTTONS)+"}";
  json += ",\"currentOptions\":[";
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    byte optionID = EEPROM.read(i);
    json += String(optionID);
    if (i < NUM_BUTTONS - 1)
      json += ",";
  }
  json += "]";
  json += "}";
  return json;
}
