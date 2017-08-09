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
  if (connectToWiFi(ssid, password) == true) {
    flashGreenLight();
  }

  int newNumberOfMentions = getHashtagMentionsFromServer();

  //**************STEP 3*******************************  
  //
  //The numberOfMentionsHasIncreased(int) function takes the new number of hashtag mentions 
  //and returns true if the number of mentions has increased, and false if not.
  //
  //If the number of mentions has increased, call the flashRainbowLights() function.
  //PLACE YOUR CODE HERE
  //
  //**************STEP 3*******************************  

  //go to sleep to save power
  ESP.deepSleep(20e6);
}

/**
 * Empty loop expected by arduino library, do not remove
**/
void loop() {
}
