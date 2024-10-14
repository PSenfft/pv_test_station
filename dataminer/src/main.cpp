#include "main.h"

struct Data{
  uint16_t voltage: 12;
  uint16_t current: 12;
  uint16_t temp_env: 13;
  uint16_t temp_panel: 13;
  uint32_t timestamp: 32;
} datas = {0, 0, 0, 0, 0};

Flags flags = {0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_temp();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World");

  wakeup_temp_environment();

  Serial.print("Temp: "); 
  Serial.print(read_temp_environment(), 4); Serial.println("°C"); 


  delay(1000);
}

