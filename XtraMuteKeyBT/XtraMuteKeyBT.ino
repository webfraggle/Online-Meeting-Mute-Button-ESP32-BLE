#include <BleKeyboard.h>
#include <AceButton.h>
#include <WiFi.h>
#include "debug.h"
#include "StateKeyboard.h"
#include "StateConfig.h"
#include "Config.h"

using namespace ace_button;



const int BUTTON_PIN = GPIO_NUM_33;
const int LED_PIN = LED_BUILTIN;
long sleepTimerStart = 0;
long timeToSleep = 120*1000;
boolean bleConnected = false;

byte wakeupReason = 0;


AceButton button(BUTTON_PIN);
void handleEvent(AceButton*, uint8_t, uint8_t);

BleKeyboard bleKeyboard;
Config myconfig;
StateKeyboard stateKeyboard(&bleKeyboard, &myconfig);
StateConfig stateConfig(&bleKeyboard, &myconfig);

char currentState = 0;
// 0 = keyboard
// 1 = config



byte get_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : return ESP_SLEEP_WAKEUP_EXT0; break;
    case ESP_SLEEP_WAKEUP_EXT1 : return ESP_SLEEP_WAKEUP_EXT1; break;
    case ESP_SLEEP_WAKEUP_TIMER : return ESP_SLEEP_WAKEUP_TIMER; break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : return ESP_SLEEP_WAKEUP_TOUCHPAD; break;
    case ESP_SLEEP_WAKEUP_ULP : return ESP_SLEEP_WAKEUP_ULP; break;
    default : return 0; break;
  }
}

void setup() {
  // put your setup code here, to run once:
  #if DEBUG != 0
    Serial.begin(115200);
  #endif
  // Turn Wifi off to minimize power consumption
  WiFi.mode(WIFI_OFF);
  
  wakeupReason = get_wakeup_reason();
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0); //1 = High, 0 = Low
  
  button.setEventHandler(handleEvent);
  // Configure the ButtonConfig with the event handler.
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  //buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);

  myconfig.begin();
  
  bleKeyboard.begin();
  sleepTimerStart = millis();
}

void loop() {
  if (millis() > sleepTimerStart+timeToSleep)
  {
    SERIAL_DEBUG_LN("Going to sleep now");
    esp_deep_sleep_start();
  }
  if(!bleKeyboard.isConnected()) {
    digitalWrite(LED_PIN, HIGH); // on wemos lolin32 high is off
    return;
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  // check fresh connection 
  if(bleKeyboard.isConnected() && !bleConnected) {
    SERIAL_DEBUG_EOL
    SERIAL_DEBUG_LN("fresh connection");
    SERIAL_DEBUG_LN(wakeupReason == ESP_SLEEP_WAKEUP_EXT0);
    if (wakeupReason == ESP_SLEEP_WAKEUP_EXT0)
    {
      delay(500);
      //executeKeyPress();
      stateKeyboard.executeKey();
    }
  }
  bleConnected = bleKeyboard.isConnected();
  button.check();
}


void handleEvent(AceButton* /*button*/, uint8_t eventType,
    uint8_t /*buttonState*/) {
  sleepTimerStart = millis();
  SERIAL_DEBUG_LN("AceButton handleEvent");
  SERIAL_DEBUG_LNF("eventType: %d", eventType);

  switch (eventType) {
    case AceButton::kEventLongPressed:
      SERIAL_DEBUG_LN("kEventLongPressed");
      switch (currentState)
      {
        case 0:
          currentState = 1;
          stateConfig.startMode();
        break;
        case 1:
          currentState = 0;
        break;
      }
      break;
    case AceButton::kEventPressed:
      SERIAL_DEBUG_LN("kEventPressed");
      break;
    case AceButton::kEventReleased:
      SERIAL_DEBUG_LN("kEventReleased");
      switch (currentState)
      {
        case 0:
          stateKeyboard.executeKey();
        break;
        case 1:
          stateConfig.executeKey();
        break;
      }
      break;
  }
}
