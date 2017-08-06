#include <Adafruit_NeoPixel.h>
#include <string.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <ESP8266WiFi.h>
#define PIN 14

extern "C" {
  #include "user_interface.h"
}

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

static const char* ssid     = "ssid";
static const char* password = "password";
const char* host = "hashtag-api.herokuapp.com";

byte rtcStore[2];

WiFiClient client;
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {

  Serial.begin(115200);
  delay(10);

  connectToWiFi(ssid, password);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  int numberOfMentions = getNumberOfHashTagMentions("/count", 80);
  int storedNumberOfMentions = getStoredNumberOfMentions();

  compareMentions(numberOfMentions, storedNumberOfMentions);

  ESP.deepSleep(20e6);

}

void compareMentions(int numberOfMentions, int storedNumberOfMentions) {
  Serial.println("Stored Number of mentions: ");
  Serial.println(storedNumberOfMentions);
  Serial.println("New: ");
  Serial.println(numberOfMentions);

  //if number of mentions has increased, light up in rainbow colours
  if (numberOfMentions != storedNumberOfMentions) {
     setStoredNumberOfMentions(numberOfMentions);
     rainbowCycle(10);
  } else {
    Serial.println("value is THE SAME!");
  }
}

int getStoredNumberOfMentions() {
  initialisertc();
  system_rtc_mem_read(65, rtcStore, 3);
  return rtcStore[1];
}

void initialisertc() {
  rst_info* rinfo = ESP.getResetInfoPtr();
  int bReason = rinfo->reason;
  Serial.print("Reason for reboot ");
  Serial.println(bReason);

  switch (bReason) {
    case 0:
    case 6:
      Serial.println("initalise stored value");
      rtcStore[1] = 0;
      rtcStore[0] = 0;
      system_rtc_mem_write(65, rtcStore, 3);
      Serial.print("rtc = ");
      Serial.println(rtcStore[1]);
      break;
  }
}

void setStoredNumberOfMentions(int mentions) {
  rtcStore[1] = mentions;
  Serial.print("new value = ");
  Serial.println(rtcStore[1]);
  system_rtc_mem_write(65, rtcStore, 3);
}

void createRequest(const String url, const int httpPort) {
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return ;
  }
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

   //check to see if request timed out
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

}

int getNumberOfHashTagMentions(const String url, const int httpPort) {
  int numberOfHashTagMentions;

  createRequest(url, httpPort);

  while(client.available()) {
    numberOfHashTagMentions = (client.readStringUntil('\r')).toInt();
  }

  return numberOfHashTagMentions;
}

void connectToWiFi(const char* ssid, const char* password) {
  if (WiFi.status() != WL_CONNECTED) {
    // WIFI
    Serial.println();
    Serial.print("===> WIFI ---> Connecting to ");
    Serial.println(ssid);
    delay(10);
    WiFi.begin(ssid, password);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      Serial.print(attempt);
      delay(100);
      attempt++;
      if (attempt == 50)
      {
        Serial.println();
        Serial.println("-----> Could not connect to WIFI");
        delay(500);
        attempt = 0;
      }

    }
    Serial.println();
    Serial.print("===> WiFi connected");
    Serial.print(" ------> IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
}

/*
 * Functions to help our LEDs light up in fancy patterns
 */
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  blackout();
}

void blackout() {
  strip.setPixelColor(0, 0, 0, 0);
  strip.setPixelColor(1, 0, 0, 0);
  strip.setBrightness(255);
  strip.show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
