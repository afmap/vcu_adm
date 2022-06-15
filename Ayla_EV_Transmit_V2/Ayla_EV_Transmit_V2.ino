#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515(25);


void setup() {
  canMsg1.can_id  = 0x0800A7A6;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0x18;
  canMsg1.data[1] = 0x00;
  canMsg1.data[2] = 0x98;
  canMsg1.data[3] = 0x3A;
  canMsg1.data[4] = 0xA2;
  canMsg1.data[5] = 0x3A;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;
  
  while (!Serial);
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
  Serial.println("Write to CAN");
}

void loop() {
  mcp2515.sendMessage(&canMsg1);
  Serial.println("Messages sent");
  delay(10);
}
