#include "Arduino.h"
#include "StateConfig.h"
#include "debug.h"
#define USE_NIMBLE
#include <BleKeyboard.h>
#include "Config.h"


StateConfig::StateConfig(BleKeyboard *bleKeyboard, Config *myconfig)
{
  _bleKeyboard = bleKeyboard;
  _config = myconfig;
}

void StateConfig::startMode()
{
  SERIAL_DEBUG_LN("StateConfig start");
  SERIAL_DEBUG_LNF("CurrentConfig %s", _config->currentOption.description);
  this->printString("Entering ConfigMode");
  this->printString("Current Config:");
  this->printString(_config->currentOption.description);
  //_bleKeyboard->print("StateConfig start");
  //_bleKeyboard->print("CurrentConfig:");
  //_bleKeyboard->print(_config->currentOption.description);
}

void StateConfig::endMode()
{
  SERIAL_DEBUG_LN("StateConfig end");
  
  this->printString("Leaving ConfigMode");
  //_bleKeyboard->print("StateConfig start");
  //_bleKeyboard->print("CurrentConfig:");
  //_bleKeyboard->print(_config->currentOption.description);
}

void StateConfig::executeKey(uint8_t id)
{
  SERIAL_DEBUG_LN("StateConfig execute");
  _config->next();
  SERIAL_DEBUG_LN(_config->currentOption.description.length());
  this->printString(_config->currentOption.description);
  delay(50); 
}

void StateConfig::printString(String str)
{
  for (int x = 0;x<str.length();x++){
    _bleKeyboard->write(str[x]);
    delay(10);
  }
  _bleKeyboard->write(KEY_RETURN);
  delay(10);
}
