#include <Adafruit_MCP9808.h>

extern Adafruit_MCP9808 tempsensor;

void init_temp();
void wakeup_temp_environment();
void wakeup_temp_panel();
float read_temp_environment();
float read_temp_panel();
