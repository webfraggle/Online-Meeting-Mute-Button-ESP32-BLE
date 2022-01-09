#include <BleKeyboard.h>
#include <AceButton.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <FastLED.h>

#include "debug.h"
#include "StateKeyboard.h"
#include "StateConfig.h"
#include "Config.h"

using namespace ace_button;


// wifi definition
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager/tree/development
WiFiManager wifiManager;
bool wifiMangerPortalRunning = false;
bool wifiConnected = false;

WebServer webServer(80);
bool webServerActive = true;
long timeToDeactivateWebServer = 1*60*1000;

const int BUTTON_PIN = GPIO_NUM_33;
const int LED_PIN = LED_BUILTIN;
long sleepTimerStart = 0;
long timeToSleep = 15*60*1000;
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


// ######################## web server functions #########################

String getRebootString() {
    return "<html><head><meta http-equiv=\"refresh\" content=\"4; url=/\"/></head><body><font face='arial'><b><h2>Rebooting... returning in 4 seconds</h2></b></font></body></html>";
}

void handleReboot() {
    webServer.send(200, "text/html", getRebootString());
    delay(500);
    ESP.restart();
}


void setup() {
  // put your setup code here, to run once:
  //#if DEBUG != 0
    Serial.begin(115200);
  //#endif

  if (!SPIFFS.begin ()) {
        Serial.println(F("An Error has occurred while mounting SPIFFS"));
        return;
    }
  


  // setting up Wifi
    String macID = WiFi.macAddress().substring(12, 14) +
        WiFi.macAddress().substring(15, 17);
    macID.toUpperCase();

    String nameString = "BLE Keyboard";

    char nameChar[nameString.length() + 1];
    nameString.toCharArray(nameChar, sizeof(nameChar));

    // setup wifiManager
    wifiManager.setHostname("BLE_Keyboard"); // set hostname
    wifiManager.setConfigPortalBlocking(false); // config portal is not blocking (LEDs light up in AP mode)
    wifiManager.setSaveConfigCallback(handleReboot); // after the wireless settings have been saved a reboot will be performed
    #if LED_DEBUG != 0
        wifiManager.setDebugOutput(true);
    #else
        wifiManager.setDebugOutput(false);
    #endif
    //wifiManager.setConnectTimeout(70);

    //wifiManager.setTimeout(80);

    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if (wifiManager.autoConnect(nameChar)) {
        Serial.println("INFO: Wi-Fi connected");
    } else {
        Serial.printf("INFO: Wi-Fi manager portal running. Connect to the Wi-Fi AP '%s' to configure your wireless connection\n", nameChar);
        wifiMangerPortalRunning = true;
    }
  
  
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
  bleKeyboard.setName("SingleButton Keyboard");
  bleKeyboard.setDelay(50);
  bleKeyboard.begin();
  
  sleepTimerStart = millis();

  webServer.on("/config.json", HTTP_GET, []() {
    String json = myconfig.getConfigJSON();
    webServer.send(200, "application/json", json);
   });

  webServer.serveStatic("/", SPIFFS, "/", "max-age=86400");

  webServer.begin();
}

void loop() {


  if (webServerActive){
    webServer.handleClient();
  }

  if (wifiMangerPortalRunning) {
        wifiManager.process();
   }
   EVERY_N_SECONDS(1) {
        Serial.println("N Sec");
        int currentWifiStatus = wifiManager.getLastConxResult();
        Serial.print(currentWifiStatus);
        Serial.print(" ");
        Serial.print(WiFi.localIP());
        Serial.println("");
        Serial.print("webServerActive");
        Serial.println(webServerActive);
        if (currentWifiStatus != WL_CONNECTED && !wifiMangerPortalRunning) {
            Serial.println("Trying to connect to Wifi");
            wifiConnected = false;
        }
        if (currentWifiStatus == WL_CONNECTED && !wifiConnected) {
            wifiConnected = true;
            Serial.print("INFO: WiFi Connected! Open http://");
            Serial.print(WiFi.localIP());
            Serial.println(" in your browser");
            //wifiManager.resetSettings();
        }
   }
  
  if (millis() > sleepTimerStart+timeToSleep)
  {
    SERIAL_DEBUG_LN("Going to sleep now");
    esp_deep_sleep_start();
  }
  if (millis() > timeToDeactivateWebServer)
  {
    // Turn Wifi off to minimize power consumption
    WiFi.mode(WIFI_OFF);
    webServerActive = false;
    webServer.stop();
    webServer.close();
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
      wakeupReason = 0;
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
          stateConfig.endMode();
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
