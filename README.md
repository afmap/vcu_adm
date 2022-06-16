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
Cycle TX
200ms (booting)
10ms (send data)

VCU Transmit 
ID: 0x0800A6A7  DLC: 8  Data: 0x14 0x00 0xA2 0x3A 0x00 0x00 0x00 0x00 (10ms) -> Torque 10 Nm 

Cycle RX
10ms motor speed/torque status
100ms motor voltage, current, temperature
100ms motor error status
