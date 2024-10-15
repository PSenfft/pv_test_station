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

unsigned long currentMillis_temp_timer = 0;
unsigned long previousMillis_temp_timper = 0;

unsigned long currentMillis_rtc_time_sync = 0;
unsigned long previousMillis_rtc_time_sync = 0;

// --- iunterrupts ---
void pps_isr(){
  flags.pps_interrupt = 1;
}


//wakeup temp sensor when timer is over // has to be resetted after meassuring
void temp_sensor_wakeup_timer(){
  if (currentMillis_temp_timer - previousMillis_temp_timper >= SLEEPTIME_TEMP && flags.temo_sensor_wakeup_triggert == 0) {

    wakeup_temp_environment();
    wakeup_temp_panel();
    flags.temo_sensor_wakeup_triggert = 1;
  }
}

void reset_temp_sensor_wakeup_timer(){
  flags.temo_sensor_wakeup_triggert = 0;
  previousMillis_temp_timper = currentMillis_temp_timer; // Reset the timer
}

//measure all sensors
void measure_everything(){

  Serial.println("Measuring everything");

  Serial.print("Temp: "); 
  Serial.print(read_temp_environment(), 4); 
  Serial.println("°C"); 

  Serial.println("time:"); 
  Serial.println(rtc.now().timestamp());

  Serial.println("temp:"); 
  Serial.println(rtc.getTemperature());

  Serial.print("GPS time: ");
  Serial.println(get_unix_time_from_gps());

  Serial.print("Sats: ");
  Serial.println(get_sat_amount());
  //TODO read voltage
  //TODO read current
  //TODO read temp_env
  //TODO read temp_panel
  //TODO read timestamp

  reset_temp_sensor_wakeup_timer();
}

void sync_time(){
  Serial.println("Syncing time");
  if ((currentMillis_rtc_time_sync - previousMillis_rtc_time_sync >= TIME_SYNC_INTERVAL) && get_sat_amount() > 3) {
    if(flags.use_gps_time){
      rtc.adjust(DateTime(get_unix_time_from_gps()));
    }
    previousMillis_rtc_time_sync = currentMillis_rtc_time_sync; // Reset the timer
  }
}

//check date
void check_timings(){
  // if no interrupt from PPS signal leave function
  if (!flags.pps_interrupt){ return;}

  //read gps and decide if gps or rtc time is to be used
  read_gps();
  //proof if gps time is valid otherwise set flag to use rtc time
  if(get_unix_time_from_gps() < POSSIBLE_TIME){
    flags.use_gps_time = 0; // GPS time is not valid use RTC time
  }else{
    flags.use_gps_time = 1; // GPS time is valid use GPS time
  }

  sync_time();
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
  currentMillis_temp_timer = millis();
  temp_sensor_wakeup_timer();
  check_timings();
  
}

