#include <SPI.h>
#include <mcp_can.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string
int PEDAL = A0; //pedal signal analog 0-5VDC
bool bitbuf[8];

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
int S_motor_torque_high;
int S_motor_torque_low;
int i;
int R=1;

#define CAN0_INT 27                              // Set INT to pin 27
MCP_CAN CAN0(25);                               // Set CS to pin 25

unsigned int S_motor_running_mode = 0; //1 traction, 2 brake
unsigned int S_motor_control_mode = 0; //1 torque, 2 speed
unsigned int S_motor_run_stop = 0; //1 run, 2 stop
unsigned int S_motor_direction = 0; //0 N, 1 CCW, 2 CW
int S_motor_torque = 0; //if using speed = 0, else 15000 to -15000
int S_motor_speed = 0; //if using torque = 0, else 15000 to -15000
unsigned long lastThing=0;
unsigned long t;
 
unsigned int N_motorTorque = 0; 
unsigned int N_motorSpeed = 0;
unsigned int N_MotorACCurrent = 0;
unsigned int N_MotorACVoltage = 0;
unsigned int N_MCUDCVoltage = 0; 
unsigned int N_motorTemp = 0;
unsigned int N_MCUTemp = 0; 
int geser;


void setup()
{
  Serial.begin(115200);
  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
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
  digitalWrite(CAN0_INT,LOW); //low the int pin
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
  CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
  if(rxId=0x0C08A6A7){
        geser=0;
        geser = rxBuf[3] << 8;
        N_motorTorque = (geser|rxBuf[2])-15000; 
        geser = 0; //reset
        geser = rxBuf[5] << 8;
        N_motorSpeed = (geser|rxBuf[4])-15000;
      }  
  }
  if(rxId=0x0C09A6A7){
    geser=0;
    geser = rxBuf[1] << 8;
    N_MotorACCurrent = (geser|rxBuf[0])*0.1;
    geser=0;
    geser=rxBuf[3] << 8;
    N_MotorACVoltage = (geser|rxBuf[2])*0.1;
    geser=0;
    geser=rxBuf[5] << 8;
    N_MCUDCVoltage = (geser|rxBuf[4])*0.1;
    N_motorTemp = rxBuf[6]-40;
    N_MCUTemp = rxBuf[7]-40;
  }
  if(rxId=0x0C0AA6A7){
    by2bt(rxBuf[1]);
    if (bitbuf[0]=1){
    Serial.println("Hardware driver failure");
    }
    if (bitbuf[1]=1){
    Serial.println("Hardware overcurrent fault");
    }
    if (bitbuf[2]=1){
    Serial.println("Zero offset fault");
    }
    if (bitbuf[3]=1){
    Serial.println("Fan failure");
    }
    if (bitbuf[4]=1){
    Serial.println("Temperature difference failure");
    }
    if (bitbuf[5]=1){
    Serial.println("AC Hall failure");
    }
    if (bitbuf[6]=1){
    Serial.println("Stall failure");
    }
    if (bitbuf[7]=1){
    Serial.println("Low voltage undervoltage fault");
    }
    by2bt(rxBuf[2]);
    if (bitbuf[0]=1){
    Serial.println("Software overcurrent fault");
    }
    if (bitbuf[1]=1){
    Serial.println("Hardware overvoltage fault");
    }
    if (bitbuf[2]=1){
    Serial.println("Total hardware failure");
    }
    if (bitbuf[3]=1){
    Serial.println("Bus overvoltage fault");
    }
    if (bitbuf[4]=1){
    Serial.println("Busbar undervoltage fault");
    }
    if (bitbuf[5]=1){
    Serial.println("Module over temperature fault");
    }
    if (bitbuf[6]=1){
    Serial.println("Module over temperature warning");
    }
    if (bitbuf[7]=1){
    Serial.println("Overspeed fault");
    }
    by2bt(rxBuf[3]);
    if (bitbuf[0]=1){
    Serial.println("Reserved");
    }
    if (bitbuf[1]=1){
    Serial.println("Motor over temperature warning");
    }
    if (bitbuf[2]=1){
    Serial.println("Motor over temperature fault");
    }
    if (bitbuf[3]=1){
    Serial.println("CAN offline failure");
    }
    if (bitbuf[4]=1){
    Serial.println("Encoder failure");
    }
  }
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

//DNR
  if (digitalRead(ModeRPIN)==HIGH){
  MAX=10;
  R=-1;
  }
  
  //pedal
  val = analogRead(PEDAL);
  accel = 15000 + map(val,0,1023,0,MAX)*R;
  S_motor_torque_high = highByte(accel);
  S_motor_torque_low = lowByte(accel);

  //send
  t=millis();
  //Create data packet for CAN message
  if(t-lastThing>=10)
  {
  digitalWrite(CAN0_INT,HIGH);
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

//recive  
  sprintf(msgString, "ID : 0x%.81X DLC:%1d Data:", (rxId & 0x1FFFFFFF), len);
Serial.print(msgString);
for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
  //print data 
sprintf(msgString, "Torque : %1d | RPM : %1d | ", N_motorTorque, N_motorSpeed);
Serial.print(msgString);
sprintf(msgString, "ACCur : %1d | ACVolt : %1d | DCVolt : %1d | ", N_MotorACCurrent, N_MotorACVoltage, N_MCUDCVoltage);
Serial.print(msgString);
sprintf(msgString, "MotTemp : %1d | McuTemp : %1d ", N_motorTemp, N_MCUTemp);
Serial.print(msgString);
Serial.println();
}


void by2bt(byte x)
{
  for (byte i = 0; i < 8; i++)
  {
    bitbuf[7 - i] = bitRead(x, i);
}
}
