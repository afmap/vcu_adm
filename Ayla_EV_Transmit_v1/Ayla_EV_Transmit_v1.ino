#include <SPI.h>
#include <mcp_can.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string
int PEDAL = A0; //pedal signal analog 0-5VDC

int ModeDPIN = 3; //pin D input add pull down
int ModeNPIN = 4; //pin R input add pull down
int ModeRPIN = 5; //pin R input add pull down
int ModeEcoPIN = 6; //pin Eco Mode input add pull down
int sensorValue = 0;
int acceleratorValue = 0;
int ModeDPINState = 0;
int ModeNPINState = 0;
int ModeRPINState = 0;
int ModeEcoPINState = 0;
int MAX;

int eco;
int val;
int accel;
byte S_motor_torque_high;
byte S_motor_torque_low;
int i;
int R=1;
unsigned long t;

#define CAN0_INT 27                              // Set INT to pin 27
MCP_CAN CAN0(25);                               // Set CS to pin 25

unsigned int S_motor_running_mode = 0; //1 traction, 2 brake
unsigned int S_motor_control_mode = 0; //1 torque, 2 speed
unsigned int S_motor_run_stop = 0; //1 run, 2 stop
unsigned int S_motor_direction = 0; //0 N, 1 CCW, 2 CW
int S_motor_torque = 0; //if using speed = 0, else 15000 to -15000
int S_motor_speed = 0; //if using torque = 0, else 15000 to -15000
unsigned long lastThing=0;

void setup()
{
  Serial.begin(115200);
  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515..."); //if any fault in can wire
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
  pinMode(CAN0_INT, INPUT);                     // Configuring pin for /INT input
  pinMode(PEDAL, INPUT);
  pinMode(ModeDPIN, INPUT);
  pinMode(ModeNPIN, INPUT);
  pinMode(ModeRPIN, INPUT);
  pinMode(ModeEcoPIN, INPUT);
}

void loop()
{
  t=millis();
  while(t>200){
// ecomode();
// DNR();
// pedal();
// send();
 sendstatictorque();
//sendstaticrpm();
  }
}

void sendstatictorque(){
  t=millis();
  //Create data packet for CAN message
  if(t-lastThing>9)
  {
    //0x18 for rpm, 0x14 for torque
  byte canMsg[8] = {0x14, 0x00, 0x99, 0x3A, 0x00, 0x00, 0x00, 0x00};
     // format:    (id, id_type, dlc, data buf)
  for(i=0; i<8;i++){
    Serial.print(canMsg[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
  byte sndStat = CAN0.sendMsgBuf(0x0800A7A6, 1, 8, canMsg);
    if(sndStat == CAN_OK){
    // Serial.println("Message Sent Successfully!");
  } else {
    // Serial.println("Error Sending Message...");
  }
 lastThing += 10;
  }
}

void sendstaticrpm(){
  t=millis();
//Create data packet for CAN message
  if(t-lastThing>9)
  {
    //0x18 for rpm, 0x14 for torque
  byte canMsg[8] = {0x18, 0x00, 0x98, 0x3A, 0xFC, 0x3A, 0x00, 0x00};
     // format:    (id, id_type, dlc, data buf)
  for(i=0; i<8;i++){
    Serial.print(canMsg[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
  byte sndStat = CAN0.sendMsgBuf(0x0800A7A6, 1, 8, canMsg);
    if(sndStat == CAN_OK){
    // Serial.println("Message Sent Successfully!");
  } else {
    // Serial.println("Error Sending Message...");
  }
 lastThing += 10;
  }
}

void ecomode(){
  //eco mode
   eco=digitalRead(ModeEcoPIN);
  if (eco==HIGH) {
  MAX=20;   //if pressed 
  R=1;
  }else{
  MAX=100; //if not pressed
  R=1;
  }
  // Serial.println(ModeEcoPINState);
}

void DNR(){
 if (digitalRead(ModeRPIN)==HIGH){
  MAX=10;
  R=-1;
  }
}

void pedal(){
 val = analogRead(PEDAL);
  accel = 15000 + map(val,0,1023,0,MAX)*R;
  S_motor_torque_high = highByte(accel);
  S_motor_torque_low = lowByte(accel);
}

void send(){  
  t=millis();
  //Create data packet for CAN message
  if(t-lastThing>9)
  {
    //0x18 for rpm, 0x14 for torque
  byte canMsg[8] = {0x18, 0x00, S_motor_torque_low, S_motor_torque_high, 0x00, 0x00, 0x00, 0x00};
     // format:    (id, id_type, dlc, data buf)
  for(i=0; i<8;i++){
    Serial.print(canMsg[i],HEX);
    Serial.print(" ");
  }
  Serial.println();
  byte sndStat = CAN0.sendMsgBuf(0x0800A7A6, 1, 8, canMsg);
    if(sndStat == CAN_OK){
    // Serial.println("Message Sent Successfully!");
  } else {
    // Serial.println("Error Sending Message...");
  }
 lastThing += 10;
  }
}
