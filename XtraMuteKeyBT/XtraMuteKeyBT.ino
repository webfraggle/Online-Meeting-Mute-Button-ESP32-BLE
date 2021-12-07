#include <BleKeyboard.h>
#include <AceButton.h>
#include <WiFi.h>
#include "StateKeyboard.h"
#include "Config.h"

using namespace ace_button;

#define DEBUG 1

const int BUTTON_PIN = GPIO_NUM_33;
const int LED_PIN = LED_BUILTIN;
long sleepTimerStart = 0;
long timeToSleep = 120*1000;
boolean bleConnected = false;

byte wakeupReason = 0;



// define debugging MACROS
#if DEBUG != 0
#define SERIAL_DEBUG_ADD(s) Serial.print(s);
#define SERIAL_DEBUG_ADDF(format, ...) Serial.printf(format, __VA_ARGS__);
#define SERIAL_DEBUG_EOL Serial.print("\n");
#define SERIAL_DEBUG_BOL Serial.printf("DEBUG [%lu]: ", millis());
#define SERIAL_DEBUG_LN(s) SERIAL_DEBUG_BOL SERIAL_DEBUG_ADD(s) SERIAL_DEBUG_EOL
#define SERIAL_DEBUG_LNF(format, ...) SERIAL_DEBUG_BOL SERIAL_DEBUG_ADDF(format, __VA_ARGS__) SERIAL_DEBUG_EOL
#else
#define SERIAL_DEBUG_ADD(s) do{}while(0);
#define SERIAL_DEBUG_ADDF(format, ...) do{}while(0);
#define SERIAL_DEBUG_EOL do{}while(0);
#define SERIAL_DEBUG_BOL do{}while(0);
#define SERIAL_DEBUG_LN(s) do{}while(0);
#define SERIAL_DEBUG_LNF(format, ...) do{}while(0);
#endif


AceButton button(BUTTON_PIN);
void handleEvent(AceButton*, uint8_t, uint8_t);

BleKeyboard bleKeyboard;
Config myconfig;
StateKeyboard stateKeyboard(&bleKeyboard, &myconfig);




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
      stateKeyboard.executeKeyPress();
    }
  }
  bleConnected = bleKeyboard.isConnected();
  button.check();
}


void handleEvent(AceButton* /*button*/, uint8_t eventType,
    uint8_t /*buttonState*/) {
  switch (eventType) {
    case AceButton::kEventPressed:
      SERIAL_DEBUG_LN("pressed");
      stateKeyboard.keyPressed();
      sleepTimerStart = millis();
      break;
    case AceButton::kEventReleased:
      SERIAL_DEBUG_LN("released");
      sleepTimerStart = millis();
      break;
  }
}
