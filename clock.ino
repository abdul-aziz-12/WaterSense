#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
 
void setup () {
    Serial.begin(9600);
    Wire.begin();
    rtc.begin();
  if (! rtc.isrunning()) {
    Serial.println("rtc is NOT running!");
    // following line sets the rtc to the date & time this sketch was compiled
   rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));;
  }
}
void loop () {
    DateTime now = rtc.now(); 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 
    delay(1000);
}
