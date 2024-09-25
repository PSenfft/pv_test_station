#include "main.h"

unsigned long previousMillis = 0;



struct flag
{
  u_int8_t pps_isr_flag:1;      //flag for PPS interrupt
  u_int8_t gps_connected:1;     //flag for GPS connection
} flags;

void PPS_ISR(){
  flags.pps_isr_flag = 1;
}

void wait(unsigned long ms){
  unsigned long currentMillis = millis();
  while(millis() - currentMillis < ms){
    //wait
  }
}

//check if gps ios connected, otherwise use counter
void pulse_from_GPS(){
  if (flags.pps_isr_flag){
    flags.gps_connected = 1;
  }
  else{
    flags.gps_connected = 0;
  }
}

void setup() {
  pinMode(PPS_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PPS_PIN), PPS_ISR, RISING);

  //-----init Serial-----
  Serial.begin(115200);       
  Serial1.begin(9600);
  while (!Serial){
    Serial.println("Serial is not available");
    delay(10);  
  }

  //-----init LoRa-----
  init_lora();

  // -----init Temp-----
  init_temp();  
}

void loop() {
  if (flags.pps_isr_flag){
    flags.pps_isr_flag = 0;
    Serial.println("PPS interrupt");
  }
  Serial.println(String(read_gps()));
  read_temp();
}


   

struct timer_ms
{
  u_int8_t ms;
};


struct timer_10ms
{
  /* data */
};

struct timer_100ms
{
  u_int8_t ms;
};

