#include "Arduino.h"
#include "StateKeyboard.h"
#include <BleKeyboard.h>

StateKeyboard::StateKeyboard(BleKeyboard *bleKeyboard)
{
  _bleKeyboard = bleKeyboard;
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
  _bleKeyboard->print("p"); 
}
