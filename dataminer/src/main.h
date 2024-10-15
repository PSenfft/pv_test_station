#pragma once

#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <string.h>
#include <RTCLib.h>
#include "temp.h"
#include "lora.h"
#include "gps.h"

#define RELAIS_PIN 5  // Relais signal
#define PPS_PIN 13  // PPS signal from GPS module
#define VOLTAGE_METER_PIN A1  // Voltage meter signal
#define CURRENT_METER_PIN A2  // Current meter signal

#define SLEEPTIME_TEMP 700 //time controller can sleep, to have enaugh time to wake up temp sensor (300 ms is needed)
#define LOOP_INTERVAL 10 // 10 * 100ms = 1s loop interval

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
};

// Declare the variable as extern
extern Flags flags;


