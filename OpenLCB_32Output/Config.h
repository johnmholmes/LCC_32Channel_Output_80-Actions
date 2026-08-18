#ifndef Config_h
#define Config_h

#include <Arduino.h>

// OpenLCB Identification Values
#define MANU "OpenLCB"
#define MODEL "Dual-MCP23017 "
#define HWVERSION "BEYOND Basic"
#define SWVERSION "1.0.0"

// To set a new nodeid based on your MERG membership number, edit the next line
#define MERG_NUMBER 8528 // A MERG membership number (in decimal)
#define NODE_INDEX 1     // The digit needs to be unique to your nodes
#define NODE_ADDRESS 0x03, 0x04, (MERG_NUMBER >> 16), (MERG_NUMBER >> 8), (MERG_NUMBER & 0xFF), NODE_INDEX 

// To set a new nodeid edit the next line
//#define NODE_ADDRESS 0x05,0x01,0x01,0x01,0x8E,0x03  // must be unique from an address space owned by you or DIY

// To Force Reset EEPROM to Factory Defaults set this value to 1, else 0.
// Need to do this at least once.
#define RESET_TO_FACTORY_DEFAULTS 1

#define ESP32_BOARD // Needed for sketch to work.

/*
 Allow direct to JMRI via USB, without CAN controller, comment out for CAN
*/

//#define USEGCSERIAL
//#include "GCSerial.h"
//#define NOCAN

//ESP32 Devkit 1
#define CAN_RX_PIN (gpio_num_t) 15
#define CAN_TX_PIN (gpio_num_t) 2

// M5Stack Atom Lite
//#define CAN_RX_PIN (gpio_num_t) 32
//#define CAN_TX_PIN (gpio_num_t) 26


// Application Limits
#define NUM_ACTION 80   // Total triggerable actions (80 configured in CDI)
#define NUM_EVENT  80   // Total OpenLCB events to consume
#define NUM_CHANNEL 32  // Total physical output channels (16 on MCP1 + 16 on MCP2)

// Global State Tracking Arrays
extern uint32_t timer[NUM_CHANNEL];
extern uint8_t state[NUM_CHANNEL];
extern uint8_t currentEvent[NUM_CHANNEL];

#define EEPROMSIZE 4096
#define EEPROMbegin { EEPROM.begin(EEPROMSIZE); dP("\nEEPROM begin "); dP(EEPROMSIZE); }
#define EEPROMcommit { EEPROM.commit(); dP("EEPROM COMMIT"); }

#endif