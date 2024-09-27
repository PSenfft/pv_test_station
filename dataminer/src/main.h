#include <Arduino.h>
#include <string.h>
#include <Adafruit_ZeroTimer.h>
#include "temp.h"
#include "lora.h"
#include "gps.h"

#define PPS_PIN 13  // PPS signal from GPS module
#define VOLTAGE_METER_PIN A0  // Voltage meter signal
#define CURRENT_METER_PIN A1  // Current meter signal

#define TEMP_WAKE_TIME 70 // 70 * 10ms = 700ms to wakeup temp sensor 300 ms before reading
#define LOOP_INTERVAL 10 // 10 * 100ms = 1s loop interval

extern uint16_t voltage;
extern uint16_t current;
extern float temp_env;
extern float temp_panel;
extern uint32_t timestamp;
