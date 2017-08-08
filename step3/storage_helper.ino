extern "C" {
  #include "user_interface.h"
}

byte rtcStore[2];

/**
 * Get previous number of hashtag mentions from memory
**/
int getStoredNumberOfMentions() {
  initialiseRtc();
  system_rtc_mem_read(65, rtcStore, 3);
  return rtcStore[1];
}

/**
 * Store new number of hashtag mentions in memory
**/
void setStoredNumberOfMentions(int mentions) {
  rtcStore[1] = mentions;
  Serial.print("new value = ");
  Serial.println(rtcStore[1]);
  system_rtc_mem_write(65, rtcStore, 3);
}

/**
 * Initialize EEPROM memory
**/
void initialiseRtc() {
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
