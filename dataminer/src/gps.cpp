#include "gps.h"
#include <time.h>

TinyGPSPlus gps;

int year;
int month;
int day;
int hour;
int minute;
int second;
int satellites;

void init_gps(){
  Serial1.begin(4800);
  while (!Serial1){
    Serial.println("Serial1 is not available");
    delay(10);  
    }
}

void read_gps(){
    while (Serial1.available()){
        gps.encode(Serial1.read());
    }
    // store retrieved gps data
    year = gps.date.year();
    month = gps.date.month();
    day = gps.date.day();
    hour = gps.time.hour();
    minute = gps.time.minute();
    second = gps.time.second();
}

uint32_t get_unix_time_from_gps(){
   // Create a tm structure
    struct tm t;
    t.tm_year = year - 1900; // tm_year is years since 1900
    t.tm_mon = month - 1;    // tm_mon is months since January (0-11)
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = 0;          // No daylight saving time

    // Convert to Unix timestamp
    time_t unixTime = mktime(&t);

    return (uint32_t)unixTime;
}

uint32_t get_sat_amount(){
    return gps.satellites.value();
}
