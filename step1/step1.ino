#include "neopixel_helper.h"
#include "hashtag_helper.h"

//**************STEP 1*******************************  
static const char* ssid     = "ssid"; //Step 1: enter WiFi SSID here
static const char* password = "password"; //Step 1: enter WiFi password here
//**************STEP 1*******************************  


/**
 * The setup method keeps looping over and over and executing the code within
**/
void setup() {
  //start serial communications so we can print debug statements
  Serial.begin(115200);
  //delay for 10ms
  delay(10);

  initializeLightsToOff();
  if (connectToWiFi(ssid, password) == true) {
    flashGreenLight();
  }

  //go to sleep to save power
  ESP.deepSleep(20e6);
}

/**
 * Empty loop expected by arduino library, do not remove
**/
void loop() {
}
