#include "Arduino.h"
#include "StateConfig.h"
#include "debug.h"
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
}


void StateConfig::executeKey()
{
  SERIAL_DEBUG_LN("StateConfig execute");
  _config->next();
}
