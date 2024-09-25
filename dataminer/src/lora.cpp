#include "lora.h"

#define RFM95_CS    8
#define RFM95_INT   3
#define RFM95_RST   4

  // Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void init_lora(){
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
}