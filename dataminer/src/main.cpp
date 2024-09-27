#include "main.h"

Adafruit_ZeroTimer zerotimer = Adafruit_ZeroTimer(3);

uint16_t voltage = 0;
uint16_t current = 0;
float temp_env = 0;
float temp_panel = 0;
uint32_t timestamp = 0;


struct flag
{
  uint8_t time_util:1;         //flag for util timer
  uint8_t pps_isr:1;           //flag for PPS interrupt
  uint8_t gps_connected:1;     //flag for GPS connection
} flags;

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

void pps_isr(){
  flags.pps_isr = 1;
}

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

//check if gps ios connected, otherwise use counter
void pulse_from_GPS(){
  if (flags.pps_isr){
    flags.gps_connected = 1;
  }
  else{
    flags.gps_connected = 0;
  }
}

void read_voltage(){
  analogRead(VOLTAGE_METER_PIN);
}

void read_current(){
  analogRead(CURRENT_METER_PIN);
}

void createPacket(uint8_t* packet) {
    // Wir kopieren die Daten in das Byte-Array
    memcpy(packet, &temp_env, sizeof(float));
    memcpy(packet + sizeof(float), &temp_panel, sizeof(float));
    memcpy(packet + 2 * sizeof(float), &voltage, sizeof(uint16_t));
    memcpy(packet + 2 * sizeof(float) + sizeof(uint16_t), &current, sizeof(uint16_t));
    memcpy(packet + 2 * sizeof(float) + 2 * sizeof(uint16_t), &timestamp, sizeof(uint32_t));
}

void transmitData() {
    uint8_t packet[16];  // 2 x float (8 Byte), 2 x uint16_t (4 Byte), 1 x uint32_t (4 Byte) -> 16 Byte
    createPacket(packet);
    rf95.send(packet, sizeof(packet));
}

void setup() {
  pinMode(PPS_PIN, INPUT);
  pinMode(VOLTAGE_METER_PIN, INPUT);
  pinMode(CURRENT_METER_PIN, INPUT);
  attachInterrupt(PPS_PIN, pps_isr, RISING);


  // Set up the timer (use a 16-bit timer)
  zerotimer.enable(false); // Disable while configuring
  zerotimer.configure(TC_CLOCK_PRESCALER_DIV64,       // prescaler 64
                      TC_COUNTER_SIZE_16BIT,          // bit width of timer/counter
                      TC_WAVE_GENERATION_MATCH_FREQ,  // match frequency generation mode
                      TC_COUNT_DIRECTION_UP);         // period value
  zerotimer.setCompare(1, 7500); // Set the compare value to 7500 for 10ms intervals

  //-----init Serial-----
  Serial.begin(115200);       
  Serial1.begin(9600);

  Serial.println("Serial connected");

  zerotimer.setCallback(true, TC_CALLBACK_CC_CHANNEL1, timer_isr); // Attach ISR to timer
  //zerotimer.enable(true);  // Enable timer

  while (!Serial){
    Serial.println("Serial is not available");
    delay(10);  
  }

  //-----init LoRa-----
  //init_lora();

  // -----init Temp-----
  //init_temp();  
}

void loop() {
  timer_util(); //manage timer

  if(flags.pps_isr == 1){ 
    Serial.println(gps.satellites.value());
    flags.pps_isr = 0;
  }

  if(timer_flags.loop_interval){
    Serial.println("Hello World");
    timer_flags.loop_interval = 0;
  }
}
