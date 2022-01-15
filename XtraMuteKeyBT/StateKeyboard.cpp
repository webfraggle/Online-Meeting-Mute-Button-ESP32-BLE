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


void StateKeyboard::executeKey(uint8_t id)
{
    KEYOPTION currentOption = _config->getOptionById(id);
    
    SERIAL_DEBUG_LN("Execute Key Press");
    SERIAL_DEBUG_LN(id);
    SERIAL_DEBUG_LN(currentOption.description);
  for ( unsigned int i = 0; i < sizeof(currentOption.keys) / sizeof(currentOption.keys[0]); i = i + 1 )
  {
    
    if (currentOption.keys[i] != 0)
    {
      SERIAL_DEBUG_LN(currentOption.keys[i]);
      SERIAL_DEBUG_LN((int) currentOption.keys[i]);
      _bleKeyboard->press(currentOption.keys[i]);
    }
  }
  _bleKeyboard->releaseAll();
  //_bleKeyboard->print("p");
}
