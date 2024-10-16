#include "main.h"

RTC_DS3231 rtc;

// Create a buffer to hold the binary data
uint8_t buffer[11]; // 12 bits + 12 bits + 13 bits + 13 bits + 32 bits = ~82 bits = ~10.25 bytes, we use 8 bytes for alignment

Data data = {0, 0, 0, 0, 0};

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

uint16_t read_voltage(){
  uint32_t adc_Value = analogRead(VOLTAGE_METER_PIN);
  double voltage = (adc_Value / ADC_MAX) * V_REF;
  return static_cast<uint16_t>(voltage);
}

//0.65V Voltage quicent
//2,65V sensor saturation
//Resulution 66.7mV/A
uint16_t read_current(){     
  uint32_t adc_Value = analogRead(CURRENT_METER_PIN);       //analog read 0-4095
  
  // return 0 if current is below 0.65V
  if (adc_Value < 807){
    return 0;
  }

  double voltage = (adc_Value / ADC_MAX) * V_REF;
  double current = ((voltage - V_OFFSET) / V_SPAN) * I_MAX;
  uint16_t float_current = static_cast<uint16_t>(current);
  return float_current;                         //return current in mA
}

// Function to print the binary package for testing
void printBinaryPackage(uint8_t *buffer, size_t length) {
  for (size_t i = 0; i < length; i++) {
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Function to pack the Data struct into an 11-byte binary array
void packDataToBinary(Data &data, uint8_t *buffer) {
  // Clear the buffer
  memset(buffer, 0, 11);

  // Packing the data manually into the buffer
  uint32_t temp1 = (data.voltage & 0xFFF) | ((data.current & 0xFFF) << 12); // Pack voltage + current
  uint32_t temp2 = (data.temp_env & 0x1FFF) | ((data.temp_panel & 0x1FFF) << 13); // Pack temp_env + temp_panel

  // Copy them into the buffer (packing in chunks of bits)
  memcpy(buffer, &temp1, 3);  // First 24 bits: voltage + current (12 + 12)
  memcpy(buffer + 3, &temp2, 4); // Next 26 bits: temp_env + temp_panel (13 + 13)

  // Copy timestamp (32 bits) into the last 4 bytes
  uint32_t timestamp = data.timestamp;
  Serial.print("timestamp: ");
  Serial.println(timestamp);
  memcpy(buffer + 7, &timestamp, 4); // 32 bits for the timestamp
}

// Function to convert ISO 8601 date to Unix timestamp
time_t timestamp_to_unix(String iso_timestamp_string) {
    // Convert String to char array
  char iso_timestamp[iso_timestamp_string.length() + 1]; // +1 for null terminator
  iso_timestamp_string.toCharArray(iso_timestamp, iso_timestamp_string.length() + 1);

  struct tm t;
  memset(&t, 0, sizeof(struct tm));

  // Parsing the ISO 8601 string manually
  sscanf(iso_timestamp, "%4d-%2d-%2dT%2d:%2d:%2dZ", 
         &t.tm_year, &t.tm_mon, &t.tm_mday, 
         &t.tm_hour, &t.tm_min, &t.tm_sec);

  // Adjust year and month (tm_year is years since 1900, tm_mon is 0-based)
  t.tm_year -= 1900;
  t.tm_mon -= 1;

  // Convert to Unix time (seconds since Jan 1, 1970)
  time_t timestamp = mktime(&t);

  // Adjust for UTC, if necessary
  return timestamp;
}


//measure all sensors
void measure_everything(){
  data.voltage = read_voltage();
  data.current = read_current();
  data.temp_env = read_temp_environment();
  data.temp_panel = read_temp_panel();

  if (flags.use_gps_time){
    Serial.println("Using GPS time");
    data.timestamp = get_unix_time_from_gps();
  }else{
    Serial.println("Using RTC time");
    Serial.println(rtc.now().timestamp());
    data.timestamp = timestamp_to_unix(rtc.now().timestamp());
  }
  Serial.print("time: ");
  Serial.println(data.timestamp);
  packDataToBinary(data, buffer);
  printBinaryPackage(buffer, sizeof(buffer));

  reset_temp_sensor_wakeup_timer();
}

void sync_time(){
  if ((currentMillis_rtc_time_sync - previousMillis_rtc_time_sync >= TIME_SYNC_INTERVAL) && get_sat_amount() > 3) {
    if(flags.use_gps_time){
      rtc.adjust(DateTime(get_unix_time_from_gps()));
      Serial.println("Syncing time.. Sat: ");
      Serial.println(get_sat_amount());
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
  
  DateTime now = DateTime(F(__DATE__), F(__TIME__));
  // Subtract 2 hours for UTC adjustment
  now = now - TimeSpan(0, 2, 0, 0);  // TimeSpan(days, hours, minutes, seconds)

  rtc.adjust(now);
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

