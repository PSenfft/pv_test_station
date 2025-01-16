#include <RH_RF95.h>
#include <Adafruit_BusIO_Register.h>

#define RFM95_CS    8
#define RFM95_INT   3
#define RFM95_RST   4

  // Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

extern RH_RF95 rf95;

void init_lora();