#include "Arduino.h"
#include "StateKeyboard.h"
#include <BleKeyboard.h>
#include "Config.h"

StateKeyboard::StateKeyboard(BleKeyboard *bleKeyboard, Config *myconfig)
{
  _bleKeyboard = bleKeyboard;
  _config = myconfig;
}

void StateKeyboard::keyPressed()
{
  this->executeKeyPress();
}

void StateKeyboard::keyReleased()
{

}

void StateKeyboard::executeKeyPress()
{
  //SERIAL_DEBUG_LN("Execute Key Press");
  _config->loadCurrentOption();
  Serial.println(_config->currentOptionID);
  Serial.println(_config->currentOption.description);
  Serial.println(sizeof(_config->currentOption.keys));
  for ( unsigned int i = 0; i < sizeof(_config->currentOption.keys) / sizeof(_config->currentOption.keys[0]); i = i + 1 )
  {
    
    if (_config->currentOption.keys[i] != 0)
    {
      Serial.println(_config->currentOption.keys[i]);
      Serial.println((int) _config->currentOption.keys[i]);
      _bleKeyboard->press(_config->currentOption.keys[i]);
    }
  }
  _bleKeyboard->releaseAll();
  //_bleKeyboard->print("p");
}
