# vcu_adm
ADM VCU DEVELOPMENT

This Repository is for converting ICE Car to EV. 
Mechanical Parts is beyon this repository scope. 

The Program below developed using Arduino IDE. There are 3 Main Parts. 

VCU - Vehicle Controller Unit
MCU - Motor Controller Unit
PDU - Power Distribution Unit
*OBC - Onboard Charger
*DCDC - HV DC to LV DC Converter

*OBC and DCDC are one parts with PDU

**CAN PROTOCOL**
The protocol is format for data sent by the VCU to controlling the MCU and PDU. The data follows Intel Format with Extended ID for CAN 2.0B. 
No need for Handshake, as long as it follows the protocol the device should running normally.

**Datasheet**
The detail of the device may found at this folder for the specification. 

**Hardware**
The schematic is harware connectivity.

Objective
- Motor speed/torque should be controlled by VCU via CAN Bus
- Motor direction is based on value of the speed/torque (ie, negative for CCW) and controlled by DNR Switch
- Output Power (PDU) enable/disable should be controlled by VCU via CAN BUS
- Charging enable/disable should be controlled by VCU via CAN BUS
- DCDC enable/disable should be controlled by VCU via CAN BUS
- VCU should know the Charging status, Voltage & Current
- VCU should know the DCDC Discharging Status, Voltage & Current
- VCU should know Output Power status, Voltage & Current

**Update**
- Using DIY VCU (Arduino Pro Mega 2560 and MCP2515).Unsucessful. No data received, fixing wiring (fixed
- Using DIY VCU (Arduino Pro Mega 2560 and MCP2515).Unsucessful. Motor Temperature Error -> Due to wrong wiring/getting loose (fixed)
- Using DIY VCU . Unsucessful. Data sent was wronggly formatted. Receive data from maker to send. **Error CAN Offline.** (unfixed)
Feedback -> Can communication should be sent continuously according to the protocol and cannot be stopped

**CAN MESSAGE**
VCU Transmit 
- Wait (200 ms for BOOT)
- ID: 0x0800A6A7  DLC: 8  Data: 0x14 0x00 0xA2 0x3A 0x00 0x00 0x00 0x00 -> Torque 10 Nm, Speed 0 rpm (10ms cycle)

byte 0 : 00 01 01 00 
byte 1 : 00 00 00 00
byte 2 : 10 10 00 10
byte 3 : 00 11 10 10 

running mode 0 (correct)
control mode 1 (correct)
run mode 1 (correct)
motor dir 0 (use 0 because not used, determine by value) 

torque 10Nm (correct)
10 + 15000 (offset) -> 3A A2 (intel syntax) 
speed 0 (correct)

- ID: 0x0800A6A7  DLC: 8  Data: 0x18 0x00 0x98 0x3A 0xFC 0x3A 0x00 0x00 -> Torque 0 Nm, Speed 100 rpm (10ms cycle)

byte 0 -> 00 01 10 00
running mode 0 (correct)
control mode 2 (correct)
run mode 1 (correct)
motor dir 0 (correct)

3A 98 -> 98 3A (0 Nm)
3A FC -> FC 3A (100 rpm)

MCU Transmit
_See MCU CAN Protocol for Details_
- Extended ID: 0x0C08A6A7  DLC: 8  Data: 0x60 0x3B 0x98 0x3A 0x98 0x3A 0x34 0x03 (10ms cycle)
- Extended ID: 0x0C09A6A7  DLC: 8  Data: 0x00 0x00 0x00 0x00 0x98 0x0D 0x43 0x44 (100ms cycle)
- Extended ID: 0x0C0AA6A7  DLC: 8  Data: 0x00 0x00 0x08 0x00 0x00 0x80 0x11 0x01 (100ms cycle)

**Thing To DO**
- Make sure the message sent correctly. (Loop Test) 
- Make sure the message sent consistent (10ms). (Timing Test)
- Make sure the received data correctly unparsed. (Read Data Test)
