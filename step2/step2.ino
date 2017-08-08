#include "neopixel_helper.h"
#include "hashtag_helper.h"

static const char* ssid     = "ssid"; //enter WiFi SSID here
static const char* password = "password"; //enter WiFi password here

/**
 * The setup method keeps looping over and over and executing the code within
**/
void setup() {
  //start serial communications so we can print debug statements
  Serial.begin(115200);
  //delay for 10ms
  delay(10);

  initializeLightsToOff();
  connectToWiFi(ssid, password);

  int newNumberOfMentions = getHashtagMentionsFromServer();

  //if number of twitter mentions for the hashtag has increased, start displaying rainbow lights
  if (numberOfMentionsHasIncreased(newNumberOfMentions) == true) {
    //start rainbow LED pattern
     rainbowCycle();
  } else {
    //black out all leds
    blackout();
    Serial.println("value is THE SAME!");
  }

  //go to sleep to save power
  ESP.deepSleep(20e6);
}

/**
 * Empty loop expected by arduino library, do not remove
**/
void loop() {
}
