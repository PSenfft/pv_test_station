#pragma once

#include <Arduino.h>
#include <string.h>
#include "temp.h"
#include "lora.h"
#include "gps.h"

#define RELAIS_PIN 5  // Relais signal
#define PPS_PIN 13  // PPS signal from GPS module
#define VOLTAGE_METER_PIN A1  // Voltage meter signal
#define CURRENT_METER_PIN A2  // Current meter signal

#define TEMP_WAKE_TIME 70 // 70 * 10ms = 700ms to wakeup temp sensor 300 ms before reading
#define LOOP_INTERVAL 10 // 10 * 100ms = 1s loop interval

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
  uint8_t timer_interrupt : 1;  // flag for util timer
  uint8_t pps_interrupt : 1;          // flag for PPS interrupt
  uint8_t gps_connected : 1;    // flag for GPS connection
};

// Declare the variable as extern
extern Flags flags;


