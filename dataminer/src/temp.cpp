#include "temp.h"

// Define the tempsensor object
Adafruit_MCP9808 tempsensor_environment = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor_panel = Adafruit_MCP9808();

struct sensor_status {
  uint8_t temp_environment : 1;
  uint8_t temp_panel : 1;
} sensors_available = {0, 0};


void init_temp(){
  if (!tempsensor_environment.begin(0x18)) {  
      sensors_available.temp_environment = 0;
      Serial.println("Couldn't find Tempsensor1!");
  }
  else{
    sensors_available.temp_environment = 1;
    Serial.println("set resolution");
    tempsensor_environment.setResolution(3);  // sets the resolution mode of reading, the modes are defined in the table bellow:
                                              //tempsensor_panel.setResolution(3);
                                              // Mode Resolution SampleTime
                                              //  0    0.5°C       30 ms
                                              //  1    0.25°C      65 ms
                                              //  2    0.125°C     130 ms
                                              //  3    0.0625°C    250 ms
    Serial.println("resoluttion: ");
    Serial.println(tempsensor_environment.getResolution());
    tempsensor_environment.wake();
    Serial.println(tempsensor_environment.readTempC());
  }
  if (!tempsensor_environment.begin(0x19)) {
      sensors_available.temp_panel = 0;
      Serial.println("Couldn't find Tempsensor2!");
  }
  else{
    sensors_available.temp_panel = 1;
    tempsensor_panel.setResolution(3);
  }
}

//wakeup sensor without delay (ther is a delay of 250ms until the sensor is ready to read)
void wakeup_temp_environment(){
  if (sensors_available.temp_environment == 1) {
    Serial.println("wakeup temp");
    Serial.println(millis());
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
    Serial.print("Temp: "); 
    Serial.print(temp_environment, 4); Serial.println("°C"); 
    
    //tempsensor_environment.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
    return temp_environment;
  }
  return 0;
}


float read_temp_panel(){
  if (sensors_available.temp_panel == 1) {
    // Read and print out the temperature
    float temp_panel = tempsensor_panel.readTempC();
    Serial.print("Temp: "); 
    Serial.print(temp_panel, 4); Serial.println("°C"); 
    
    tempsensor_panel.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
    return temp_panel;
  }
  return 0;
}

void fucking_temp_loop(){
  Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
  tempsensor_environment.wake();   // wake up, ready to read!

  // Read and print out the temperature, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempsensor_environment.getResolution());
  float c = tempsensor_environment.readTempC();
  float f = tempsensor_environment.readTempF();
  Serial.print("Temp: "); 
  Serial.print(c, 4); Serial.print("*C\t and "); 
  Serial.print(f, 4); Serial.println("*F.");
  
  delay(2000);
  Serial.println("Shutdown MCP9808.... ");
  tempsensor_environment.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  Serial.println("");
  delay(200);
}