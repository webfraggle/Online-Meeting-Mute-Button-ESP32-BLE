#include "Arduino.h"
#include "StateKeyboard.h"
#include "debug.h"
#include <BleKeyboard.h>
#include "Config.h"


StateKeyboard::StateKeyboard(BleKeyboard *bleKeyboard, Config *myconfig)
{
  _bleKeyboard = bleKeyboard;
  _config = myconfig;
}


void StateKeyboard::executeKey()
{
  //SERIAL_DEBUG_LN("Execute Key Press");
  //_config->loadCurrentOption();
//  SERIAL_DEBUG_LN(_config->currentOptionID);
  SERIAL_DEBUG_LN(_config->currentOption.description);
//  SERIAL_DEBUG_LN(sizeof(_config->currentOption.keys));
  for ( unsigned int i = 0; i < sizeof(_config->currentOption.keys) / sizeof(_config->currentOption.keys[0]); i = i + 1 )
  {
    
    if (_config->currentOption.keys[i] != 0)
    {
      SERIAL_DEBUG_LN(_config->currentOption.keys[i]);
      SERIAL_DEBUG_LN((int) _config->currentOption.keys[i]);
      _bleKeyboard->press(_config->currentOption.keys[i]);
    }
  }
  _bleKeyboard->releaseAll();
  //_bleKeyboard->print("p");
}
