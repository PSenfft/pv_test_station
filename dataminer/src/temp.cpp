#include "temp.h"

// Define the tempsensor object
Adafruit_MCP9808 tempsensor_environment;
Adafruit_MCP9808 tempsensor_panel;

void init_temp(){
  if (!tempsensor_environment.begin(0x18)) {
      Serial.println("Couldn't find Tempsensor1!");
  }
  if (!tempsensor_environment.begin(0x19)) {
      Serial.println("Couldn't find Tempsensor2!");
  }

  tempsensor_environment.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  //tempsensor_panel.setResolution(3);
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}

//wakeup sensor without delay (ther is a delay of 250ms until the sensor is ready to read)
void wakeup_temp_environment(){
  tempsensor_environment.shutdown_wake(0); // wake up MSP9808 - power consumption ~200 mikro Ampere, starts temperature sampling
}

void wakeup_temp_panel(){
  tempsensor_panel.shutdown_wake(0); // wake up MSP9808 - power consumption ~200 mikro Ampere, starts temperature sampling
}

//read temperature and set sensor to sleep
float read_temp_environment(){
  // Read and print out the temperature
  float temp_environment = tempsensor_environment.readTempC();
  Serial.print("Temp: "); 
  Serial.print(temp_environment, 4); Serial.println("°C"); 
  
  tempsensor_environment.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  return temp_environment;
}


float read_temp_panel(){
  // Read and print out the temperature
  float temp_panel = tempsensor_panel.readTempC();
  Serial.print("Temp: "); 
  Serial.print(temp_panel, 4); Serial.println("°C"); 
  
  tempsensor_panel.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  return temp_panel;
}