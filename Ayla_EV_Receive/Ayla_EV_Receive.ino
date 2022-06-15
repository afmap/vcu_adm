#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string
bool bitbuf[8];

#define CAN0_INT 27                              // Set INT to pin 2
MCP_CAN CAN0(25);                               // Set CS to pin 10
 
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
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
  pinMode(CAN0_INT, INPUT);                     // Configuring pin for /INT input
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
  // digitalWrite(CAN0_INT,LOW); //low the int pin
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
  //print message
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
}


void by2bt(byte x)
{
  for (byte i = 0; i < 8; i++)
  {
    bitbuf[7 - i] = bitRead(x, i);
  }
}
/***********************************
  END FILE
***********************************/
