#include "temp.h"

// Define the tempsensor object
Adafruit_MCP9808 tempsensor;

void init_temp(){
    if (!tempsensor.begin(0x18)) {
        Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
  }

  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}

void read_temp(){
  tempsensor.wake();   // wake up, ready to read!

  // Read and print out the temperature
  float c = tempsensor.readTempC();
  Serial.print("Temp: "); 
  Serial.print(c, 4); Serial.println("°C"); 
  
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
}