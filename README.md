# LCC_32Channel_Output_80 Actions
A Beyond the Basic Node Using 2 MCP23017 to control 32 output channels

This sketch implaments 2 MCP23017 to offer 32 outputs with 80 actions.

Designed for driving low current LEDs. 

Tested on the ESP32 Devkit 1 30 pin version
ESP32 board manager version 3.3.11 by Espressif
MCP23017.h by Bertrand Lemasle version 2.0.0
OpenLCB Single Thread.h Version 1.1.19

MCP23017 addresses used
device 1 0x20
device 2 0x21
Ensure current limiting resistors are used to avoid damaging the I/O pins.
See datasheet in this web page https://www.microchip.com/en-us/product/mcp23017


It offers the following Pin setting so you can choose a couple of actions per pin,  
Depending on your needs for animation.

None
Low
High
Flash
Double Strobe
Random

Notes:
  * if RESET_TO_FACTORY_DEFAULTS is 1, then the EEPROM is intialized, 
    it must be changed to 0 and the sketch reloaded to allow JMRI to change items permanently.
  * The ACAN_ESP32Can.h library has been changed by David Harris.


The SN65HVD230 Can Transceiver is attached via pins

RX to pin D15
TX to pin D2
VCC Pin 3.3volts
GND Pin GND

The i2c is connected via pins

SDA pin D21
SCL pin D22
VCC Pin 3.3volts
GND Pin GND

