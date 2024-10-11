#include "main.h"

struct Data{
  uint16_t voltage: 12;
  uint16_t current: 12;
  uint16_t temp_env: 13;
  uint16_t temp_panel: 13;
  uint32_t timestamp: 32;
} datas = {0, 0, 0, 0, 0};

Flags flags = {0, 0, 0};

void timerISR(){
  flags.timer_interrupt = 1;

  #if (TIMER_INTERRUPT_DEBUG > 0)
      static uint32_t curMillis = 0;
        
      curMillis = millis();
      
      if (curMillis > TIMER_INTERVAL_MS)
      {
        Serial.print("ITimer: millis() = "); Serial.print(curMillis);
        Serial.print(", delta = "); Serial.println(curMillis - preMillisTimer);
      }
      
      preMillisTimer = curMillis;
  #endif
  }

void pps_isr(){
  flags.pps_interrupt = 1;
}

//check if gps is connected, otherwise use counter
void pulse_from_GPS(){
  if (flags.pps_interrupt){
    flags.gps_connected = 1;
  }
  else{
    flags.gps_connected = 0;
  }
}

uint32_t read_voltage(){
  return analogRead(VOLTAGE_METER_PIN);
}

//0.65V Voltage quicent
//2,65V sensor saturation
//Resulution 66.7mV/A
uint32_t compute_current(uint32_t adcValue){     
  return current;
  if (current > 806 && ) {
    double voltage = (adcValue / ADC_MAX) * V_REF;
    double current = ((voltage - V_OFFSET) / V_SPAN) * I_MAX;
    uint16_t float_current = static_cast<uint16_t>(current);
    return float_current;

  }else{
    return 0;
  }
}

uint32_t read_current(){
  return analogRead(CURRENT_METER_PIN);       //analog read 0-4095
}

void createPacket(uint8_t* packet) {
    // copy Data to  Byte-Array
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

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

volatile uint32_t preMillisTimer = 0;

void init_timer(){
  Serial.print(F("\nStarting TimerInterruptTest on ")); Serial.println(BOARD_NAME);
  Serial.println(SAMD_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in millisecs
  if (ITimer.attachInterruptInterval_MS(TIMER_INTERVAL_MS, timerISR))
  {
    preMillisTimer = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(preMillisTimer);
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));
}

void setup() {
  pinMode(PPS_PIN, INPUT);
  pinMode(VOLTAGE_METER_PIN, INPUT);
  pinMode(CURRENT_METER_PIN, INPUT);
  attachInterrupt(PPS_PIN, pps_isr, RISING);

  while (!Serial){
    Serial.println("Serial is not available");
    delay(10);  
  }
  init_gps();

  delay(100);
  
  //init_timer();
  //-----init LoRa-----
  init_lora();

  // -----init Temp-----
  init_temp(); 
}

void loop() {
/*   if(flags.timer_interrupt){
    Serial.println("wake up");
    Serial.println(millis());
    wakeup_temp_environment();
    wakeup_temp_panel();
    flags.timer_interrupt = 0;
  } */

  fucking_temp_loop();

  if(flags.pps_interrupt){
    
  }
}

