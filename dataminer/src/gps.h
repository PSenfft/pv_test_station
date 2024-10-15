#include <TinyGPS++.h>

extern TinyGPSPlus gps;

void init_gps();
void read_gps();

uint32_t get_unix_time_from_gps();
uint32_t get_sat_amount();