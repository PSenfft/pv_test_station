#include "temp.h"

// Define the tempsensor object
Adafruit_MCP9808 tempsensor_environment = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor_panel = Adafruit_MCP9808();

struct sensor_status {
  uint8_t temp_environment : 1;
  uint8_t temp_panel : 1;
} sensors_available = {0, 0};


// Mode Resolution SampleTime
//  0    0.5°C       30 ms
//  1    0.25°C      65 ms
//  2    0.125°C     130 ms
//  3    0.0625°C    250 ms
void init_temp(){
  if (!tempsensor_environment.begin(SENSOR_ADDRESS_ENV)) {
      sensors_available.temp_environment = 0;
      Serial.println("Couldn't find Tempsensor env!");
  }
  else{
    sensors_available.temp_environment = 1;
    tempsensor_environment.setResolution(3);  // sets the resolution mode of reading, the modes are defined in the table above:                              
  }
  if (!tempsensor_panel.begin(SENSOR_ADDRESS_PANEL)) {
      sensors_available.temp_panel = 0;
      Serial.println("Couldn't find Tempsensor panel!");
  }
  else{
    sensors_available.temp_panel = 1;
    tempsensor_panel.setResolution(3);
  }
}

//wakeup sensor without delay (ther is a delay of 250ms until the sensor is ready to read)
void wakeup_temp_environment(){
  if (sensors_available.temp_environment == 1) {
    tempsensor_environment.shutdown_wake(0); // wake up MSP9808 - power consumption ~200 mikro Ampere, starts temperature sampling
  }
}

void wakeup_temp_panel(){
  if (sensors_available.temp_panel == 1) {
    tempsensor_panel.shutdown_wake(0); // wake up MSP9808 - power consumption ~200 mikro Ampere, starts temperature sampling
  }
}

//read temperature and set sensor to sleep
float read_temp_environment(){
  if (sensors_available.temp_environment == 1) {
    // Read and print out the temperature
    float temp_environment = tempsensor_environment.readTempC(); 
    tempsensor_environment.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
    return temp_environment;
  }
  return 0;
}

float read_temp_panel(){
  if (sensors_available.temp_panel == 1) {
    // Read and print out the temperature
    float temp_panel = tempsensor_panel.readTempC();
    tempsensor_panel.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
    return temp_panel;
  }
  return 0;
}