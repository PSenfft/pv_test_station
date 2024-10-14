#include <Wire.h>
#include <Adafruit_MCP9808.h>

#define SENSOR_ADDRESS_ENV 0x18
#define SENSOR_ADDRESS_PANEL 0x19

extern Adafruit_MCP9808 tempsensor;

void init_temp();

//wakeup sensor without delay (ther is a delay of 250ms until the sensor is ready to read)
void wakeup_temp_environment();
void wakeup_temp_panel();

//read temperature and set sensor to sleep
float read_temp_environment();
float read_temp_panel();


void fucking_temp_loop();