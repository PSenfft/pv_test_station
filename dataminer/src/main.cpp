#include "main.h"

RTC_DS3231 rtc;

struct Data{
  uint16_t voltage: 12;
  uint16_t current: 12;
  uint16_t temp_env: 13;
  uint16_t temp_panel: 13;
  uint32_t timestamp: 32;
} datas = {0, 0, 0, 0, 0};

Flags flags = {0, 0, 0};

int last_millis = 0;

// --- iunterrupts ---
void pps_isr(){
  flags.pps_interrupt = 1;
}

void measure_everything(){
  //TODO read voltage
  //TODO read current
  //TODO read temp_env
  //TODO read temp_panel
  //TODO read timestamp
}

void check_timings(){
  // if no interrupt from PPS signal leave function
  if (!flags.pps_interrupt){ return;}

  read_gps();

  //proof if gps time is valid otherwise set flag to use rtc time
  if(get_unix_time_from_gps() < POSSIBLE_TIME){
    flags.use_gps_time = 0; // GPS time is not valid use RTC time
  }else{
    flags.use_gps_time = 1; // GPS time is valid use GPS time
  }

  measure_everything();
  //TODO set wachdog timer

  flags.pps_interrupt = 0;
}

void init_rtc(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void setup() {
  //Pin setup
  pinMode(PPS_PIN, INPUT);
  pinMode(VOLTAGE_METER_PIN, INPUT);
  pinMode(CURRENT_METER_PIN, INPUT);
  attachInterrupt(PPS_PIN, pps_isr, RISING);

  //USB Serial
  Serial.begin(9600);

  init_rtc();
  init_temp();
  init_gps();
  init_lora();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World");

  wakeup_temp_environment();
  delay(300);

  Serial.print("Temp: "); 
  Serial.print(read_temp_environment(), 4); 
  Serial.println("°C"); 

  Serial.println("time:"); 
  Serial.println(rtc.now().timestamp());

  Serial.println("temp:"); 
  Serial.println(rtc.getTemperature());

  delay(1000);
  
}

