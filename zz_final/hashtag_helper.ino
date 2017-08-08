/**
 * Helper functions to fetch and compare hashtag mentions
**/
#include <string.h>
#include <ESP8266WiFi.h>
#include "storage_helper.h"

const char* host = "hashtag-api.herokuapp.com";
WiFiClient client;

/**
 * Make a request to the remote server to get the number of mentions for a pre-defined hashtag
**/
int getHashtagMentionsFromServer() {
  int numberOfHashTagMentions;
  String url = "/count";
  int httpPort = 80;
  createRequest(url, httpPort);

  while(client.available()) {
    numberOfHashTagMentions = (client.readStringUntil('\r')).toInt();
  }

  return numberOfHashTagMentions;
}

/**
 * Compare number of twitter mentions with previous stored value to check if there
 * has been an increase in the number of mentions
 **/
boolean numberOfMentionsHasIncreased(int newNumberOfMentions) {
  int oldNumberOfMentions = getStoredNumberOfMentions();
  Serial.println("Stored Number of mentions: ");
  Serial.println(oldNumberOfMentions);
  Serial.println("New: ");
  Serial.println(newNumberOfMentions);
  setStoredNumberOfMentions(newNumberOfMentions);

  return newNumberOfMentions > oldNumberOfMentions;
}


/**
 * connect to a remote server to check for twitter mentions
**/
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

/**
 *  Connect to wifi and print error if unsuccessful
**/
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
