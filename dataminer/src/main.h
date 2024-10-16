#pragma once

#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <time.h>
#include <string.h>
#include <RTCLib.h>
#include "temp.h"
#include "lora.h"
#include "gps.h"

#define RELAIS_PIN 5  // Relais signal
#define PPS_PIN 13  // PPS signal from GPS module
#define VOLTAGE_METER_PIN A1  // Voltage meter signal
#define CURRENT_METER_PIN A2  // Current meter signal

#define SLEEPTIME_TEMP 700 //time controller can sleep, to have enaugh time to wake up temp sensor (300 ms is needed) and there is somewhere a delay of 100ms
#define TIME_SYNC_INTERVAL 300000 //time interval to sync time with gps 300.000ms = 5 min

#define POSSIBLE_TIME 1000000000 // time at 2001, bigger the defult failur time from gps,  but still defitly wrong

#define ADC_MAX 4095.0
#define V_REF 3.3
#define V_SAT 2.65
#define V_OFFSET 0.65
#define V_SPAN (V_REF - V_OFFSET)
#define I_MAX 30000 //30A as mA

extern uint16_t voltage;
extern uint16_t current;
extern float temp_env;
extern float temp_panel;
extern uint32_t timestamp;

struct Flags
{
  uint8_t pps_interrupt : 1;    // flag for PPS interrupt
  uint8_t use_gps_pps : 1;      // flag for util timer
  uint8_t use_gps_time : 1;     // flag for GPS connection
  uint8_t temp_sensor_wakeup_triggert : 1; // flag for temp sensor wakeup
};

extern uint16_t voltage;        // voltage in mV
extern uint16_t current;        // current in mA
extern float temp_env;          // temperature in °C with 4 digits behind the comma
extern float temp_panel;        // temperature in °C with 4 digits behind the comma
extern uint32_t timestamp;

// Declare the variable as extern
extern Flags flags;


