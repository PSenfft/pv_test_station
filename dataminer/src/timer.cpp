#include "timer.h"

struct timer
{
  uint8_t temp_wake:1;         //timer for wakeup temp sensor
  uint8_t loop_interval:1;     //timer for 1s interval
} timer_flags;

//util timer
uint8_t util_timer_10ms = 0;
uint8_t util_timer_100ms = 0;


// 10ms timer
uint8_t counter_10ms_wake = 0;         //timer to wakeup temp sensor 300 ms before reading

// 100ms timer
uint8_t counter_100ms_interval = 0;    //timer for loop interval

/// get triggert every 10ms
void timer_isr(){
  util_timer_10ms++;                //increment 10ms timer
  util_timer_10ms++;                //increment counter for 100ms ticks
  flags.time_util = 1;
}

//util function to manage timer
void timer_util(){
  //Serial.println(flags.time_util);
  if(flags.time_util == 1){            //check if timer isr was triggert
    //tick 100ms
    if (util_timer_10ms >= 10){   //for time up to 100ms
      util_timer_100ms++;         //increment 100ms timer
      util_timer_10ms = 0;        //reset counter
    }

    //check 10ms timer
    if (util_timer_10ms >= TEMP_WAKE_TIME){
      timer_flags.temp_wake = 1;  //set flag to wakeup temp sensor
      util_timer_10ms = 0;        //reset timer
    }

    //check 100ms timer
    if (util_timer_100ms >= 10){  //for time up to 1000ms | 1s

      if (counter_100ms_interval >= LOOP_INTERVAL){
        timer_flags.loop_interval = 1;  //set flag for 1s interval
        counter_100ms_interval = 0;           //reset timer
    }

      util_timer_100ms = 0;       //reset timer
    }
    flags.time_util = 0;          //reset flag
  }
}