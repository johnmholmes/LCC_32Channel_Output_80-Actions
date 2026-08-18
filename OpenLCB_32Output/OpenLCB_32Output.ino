/*Disclaimer and Limitation of Liability

This sketch (software) has been developed specifically for the **ESP32 Devkit 1** and the **SN65HVD230** CAN transceiver module. It has only been tested on the author’s personal model railway layout.

**The sketch is provided “AS IS” and “AS AVAILABLE”**, without any warranties or guarantees of any kind. The author explicitly disclaims all warranties, whether express, implied, or statutory, including but not limited to any warranties of merchantability, fitness for a particular purpose, accuracy, reliability, or non-infringement.

The author accepts **no responsibility or liability** for:
- Any malfunction, failure, or unexpected behaviour of the sketch
- Damage to hardware, loss of data, or disruption to your layout
- Incompatibility caused by updates to third-party libraries, Arduino core, JMRI, or other software
- Any direct, indirect, incidental, consequential, or punitive damages arising from the use or inability to use this sketch

This code is offered strictly for **educational and hobbyist purposes** to help railway modellers learn how to use the OpenLCB Single Thread Library. It is not intended for commercial use, safety-critical applications, or any situation where failure could cause damage or injury.

By downloading, using, or modifying this sketch, you acknowledge that you assume **all risk** and full responsibility for any outcomes resulting from its use.

The author reserves the right to modify or remove this sketch at any time without notice.

*/

/*
  32 CHannels of IO 
  Uses MCP23017

  Dependences: 
    OpenLCB_SIngle_Thread library
    MCP23017 library by Bertrand
    

Copyright JHolmes & David Harris 2026
*/

#include "Config.h"         // Contains configuration, see "Config.h"
#include "ACAN_ESP32Can.h"  // uses ACan class, comment out if using GCSerial
#include "HouseKeeping.h"   // Used for development purposes only do not touch
#include "mdebugging.h"     // debugging
#include "processor.h"      // auto-selects the processor type, and CAN lib, EEPROM lib etc.
#include "OpenLcbCore.h"
#include "OpenLCBHeader.h"  // System house-keeping.

#include <Wire.h>
#include <MCP23017.h>       // Bertrand Levoy's MCP23017 library

// Global tracking arrays required by OpenLCB handlers & Config.h
uint32_t timer[NUM_CHANNEL];
uint8_t state[NUM_CHANNEL];
uint8_t currentEvent[NUM_CHANNEL];

// Instantiate MCP23017 ICs with I2C Addresses
MCP23017 mcp1(0x20); // MCP #1 (Channels 1-16)
MCP23017 mcp2(0x21); // MCP #2 (Channels 17-32)

// Centralized hardware writing abstraction for all 32 channels using Bertrand's library
void setOutputPin(uint8_t channelIndex, uint8_t val) {
  if (channelIndex < 16) {
    // Channels 0 to 15 -> MCP #1 (0x20)
    mcp1.digitalWrite(channelIndex, val);
  } else if (channelIndex < 32) {
    // Channels 16 to 31 -> MCP #2 (0x21)
    mcp2.digitalWrite(channelIndex - 16, val);
  }
}

extern "C" {        // the following are defined as external
#define N(x) xN(x)  // allows the insertion of value (x)
#define xN(x) #x    // .. into the CDI string.

  const char configDefInfo[] PROGMEM =
    CDIheader R"(
      <name>Application Configuration</name>
      <group replication="8">
        <name>Action Groups</name>
        <hints><visibility hideable='yes' hidden='yes' ></visibility></hints>
        <description>Choose an Action group depending on your requirements. You have 80 Actions that can be spread across 32 expander pins.</description>
        <repname>Actions 1-10</repname>
        <repname>Actions 11-20</repname>
        <repname>Actions 21-30</repname>
        <repname>Actions 31-40</repname>
        <repname>Actions 41-50</repname>
        <repname>Actions 51-60</repname>
        <repname>Actions 61-70</repname>
        <repname>Actions 71-80</repname>

        <group replication="10">
          <name>Actions</name>
          <string size='24'><name>Description</name></string>
          <repname>Action </repname>
          <description>Define what an expander channel can do when a consumed event is received.</description>
          <eventid><name>Event for action. Copy your produced event from another node and paste here to trigger this action.</name></eventid>

          <int size="1">
            <name>Choose Output Channel (1-16: MCP #1 [0x20], 17-32: MCP #2 [0x21])</name>
            <map>
              <relation><property>0</property><value>None</value></relation>
              <relation><property>1</property><value>MCP 1 - GPA0</value></relation>
              <relation><property>2</property><value>MCP 1 - GPA1</value></relation>
              <relation><property>3</property><value>MCP 1 - GPA2</value></relation>
              <relation><property>4</property><value>MCP 1 - GPA3</value></relation>
              <relation><property>5</property><value>MCP 1 - GPA4</value></relation>
              <relation><property>6</property><value>MCP 1 - GPA5</value></relation>
              <relation><property>7</property><value>MCP 1 - GPA6</value></relation>
              <relation><property>8</property><value>MCP 1 - GPA7</value></relation>
              <relation><property>9</property><value>MCP 1 - GPB0</value></relation>
              <relation><property>10</property><value>MCP 1 - GPB1</value></relation>
              <relation><property>11</property><value>MCP 1 - GPB2</value></relation>
              <relation><property>12</property><value>MCP 1 - GPB3</value></relation>
              <relation><property>13</property><value>MCP 1 - GPB4</value></relation>
              <relation><property>14</property><value>MCP 1 - GPB5</value></relation>
              <relation><property>15</property><value>MCP 1 - GPB6</value></relation>
              <relation><property>16</property><value>MCP 1 - GPB7</value></relation>
              <relation><property>17</property><value>MCP 2 - GPA0</value></relation>
              <relation><property>18</property><value>MCP 2 - GPA1</value></relation>
              <relation><property>19</property><value>MCP 2 - GPA2</value></relation>
              <relation><property>20</property><value>MCP 2 - GPA3</value></relation>
              <relation><property>21</property><value>MCP 2 - GPA4</value></relation>
              <relation><property>22</property><value>MCP 2 - GPA5</value></relation>
              <relation><property>23</property><value>MCP 2 - GPA6</value></relation>
              <relation><property>24</property><value>MCP 2 - GPA7</value></relation>
              <relation><property>25</property><value>MCP 2 - GPB0</value></relation>
              <relation><property>26</property><value>MCP 2 - GPB1</value></relation>
              <relation><property>27</property><value>MCP 2 - GPB2</value></relation>
              <relation><property>28</property><value>MCP 2 - GPB3</value></relation>
              <relation><property>29</property><value>MCP 2 - GPB4</value></relation>
              <relation><property>30</property><value>MCP 2 - GPB5</value></relation>
              <relation><property>31</property><value>MCP 2 - GPB6</value></relation>
              <relation><property>32</property><value>MCP 2 - GPB7</value></relation>
            </map>
          </int>

          <int size="1">
            <name>Action Algorithm</name>
            <map>
              <relation><property>0</property><value>None</value></relation>
              <relation><property>1</property><value>Low</value></relation>
              <relation><property>2</property><value>High</value></relation>
              <relation><property>3</property><value>Flash</value></relation>
              <relation><property>4</property><value>Double Strobe</value></relation>
              <relation><property>5</property><value>Random</value></relation>
            </map>
          </int>

          <int size="1">
            <name>Parameter 1 (On Duration)</name>
            <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
          </int>

          <int size="1">
            <name>Parameter 2 (Off Duration)</name>
            <hints><slider tickSpacing='85' immediate='yes' showValue='yes'> </slider></hints>
          </int>
        </group>
      </group>
    )" CDIfooter;
}

// ===== MemStruct =====
typedef struct {
  EVENT_SPACE_HEADER eventSpaceHeader;
  char nodeName[20];
  char nodeDesc[24];

  struct {
    char desc[24];
    EventID eid;
    uint8_t pini;
    char action;
    uint8_t durn;
    uint8_t rate;
  } action[NUM_ACTION];
} MemStruct;

extern "C" {

#define aEID(i) CEID(action[i].eid), \
                CEID(action[i + 1].eid), \
                CEID(action[i + 2].eid), \
                CEID(action[i + 3].eid), \
                CEID(action[i + 4].eid)

  // Expanded for 80 total EventIDs
  const EIDTab eidtab[NUM_EVENT] PROGMEM = {
    aEID(0),  aEID(5),  aEID(10), aEID(15), aEID(20),
    aEID(25), aEID(30), aEID(35), aEID(40), aEID(45),
    aEID(50), aEID(55), aEID(60), aEID(65), aEID(70),
    aEID(75)
  };

  extern const char SNII_const_data[] PROGMEM = 
    "\001" MANU "\000" MODEL " " "\000" HWVERSION "\000" SWVERSION " " ;  

};

uint8_t protocolIdentValue[6] = {
  pSimple | pDatagram | pMemConfig | pPCEvents | !pIdent | pTeach | !pStream | !pReservation,
  pACDI | pSNIP | pCDI | !pRemote | !pDisplay | !pTraction | !pFunction | !pDCC,
  0, 0, 0, 0
};

#ifdef PV
  #undef PV
#endif
#define PV(x) { dP(" " #x "="); dP(x); }

#ifdef PVL
  #undef PVL
#endif
#define PVL(x) { dP("\n" #x "="); dP(x); }

void pceCallback(uint16_t index) {
  dP("\npceCallback(");
  dP(index);
  PV(NODECONFIG.read(EEADDR(action[index].pini)));
  PV(NODECONFIG.read(EEADDR(action[index].action)));
  PV(NODECONFIG.read(EEADDR(action[index].durn)));
  PV(NODECONFIG.read(EEADDR(action[index].rate)));
  
  uint8_t i = NODECONFIG.read(EEADDR(action[index].pini)) - 1;  // convert 1-32 to 0-31 index
  if (i >= NUM_CHANNEL) return;

  currentEvent[i] = index;
  timer[i] = millis() + NODECONFIG.read(EEADDR(action[index].durn));
  state[i] = 0;
}

bool initialized = false;

void userInitAll() {
  NODECONFIG.put(EEADDR(nodeName), ESTRING("ESP32-DualMCP"));
  NODECONFIG.put(EEADDR(nodeDesc), ESTRING("32 Channel Output Node"));
  
  for (int i = 0; i < NUM_ACTION; i++) {
    NODECONFIG.write(EEADDR(action[i].pini), 0);
  }
  
  for (int i = 0; i < NUM_CHANNEL; i++) {
    state[i] = 0;
    timer[i] = 0;
    currentEvent[i] = 255;
  }
  initialized = true;
}

void userSoftReset() { REBOOT; }
void userHardReset() { REBOOT; }
void userConfigWritten(uint32_t address, uint16_t length, uint16_t func) {}

NodeID nodeid(NODE_ADDRESS);
#include "OpenLCBMid.h"

enum Action { aLOW = 1, aHIGH, aFLASH, aDSTROBE, aRANDOM };

void doActions() {
  static uint8_t i = 255;
  uint8_t pi, actn, rate, durn;

  if (!initialized) return;
  if (++i >= NUM_CHANNEL) i = 0;

  uint8_t ei = currentEvent[i];
  if (ei == 255) return;

  long now = millis();
  pi = NODECONFIG.read(EEADDR(action[ei].pini)) - 1;  // output index (0 to 31)
  actn = NODECONFIG.read(EEADDR(action[ei].action));
  durn = NODECONFIG.read(EEADDR(action[ei].durn));
  rate = NODECONFIG.read(EEADDR(action[ei].rate));

  switch (actn) {
    case aLOW:
      if (state[i]) return;
      setOutputPin(pi, LOW);
      state[i] = 1;
      break;

    case aHIGH:
      if (state[i]) return;
      setOutputPin(pi, HIGH);
      state[i] = 1;
      break;

    case aFLASH:
      if (timer[i] && (now - timer[i]) > 0) {
        switch (state[i]) {
          case 0:
            setOutputPin(pi, HIGH);
            timer[i] = now + durn * 100;
            state[i] = 1;
            break;
          case 1:
            setOutputPin(pi, LOW);
            timer[i] = now + rate * 100;
            state[i] = 0;
            break;
        }
      }
      break;

    case aDSTROBE:
      if (timer[i] && (now - timer[i]) > 0) {
        switch (state[i]) {
          case 0:
            setOutputPin(pi, HIGH);
            timer[i] = now + 100;
            state[i] = 1;
            break;
          case 1:
            setOutputPin(pi, LOW);
            timer[i] = now + durn * 100;
            state[i] = 2;
            break;
          case 2:
            setOutputPin(pi, HIGH);
            timer[i] = now + 100;
            state[i] = 3;
            break;
          case 3:
            setOutputPin(pi, LOW);
            timer[i] = now + rate * 100;
            state[i] = 0;
            break;
        }
      }
      break;

    case aRANDOM:
      if (timer[i] && (now - timer[i]) > 0) {
        switch (state[i]) {
          case 0:
            setOutputPin(pi, HIGH);
            timer[i] = now + random(durn * durn * 5, durn * durn * 20);
            state[i] = 1;
            break;
          case 1:
            setOutputPin(pi, LOW);
            timer[i] = now + random(rate * rate * 5, rate * rate * 20);
            state[i] = 0;
            break;
        }
      }
      break;
  }
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial);
  delay(500);
  Serial.print("\n Testing 32 Output Node (Dual MCP23017 - Bertrand Library)");
#endif

  // Initialize I2C bus explicitly on GPIO 21 (SDA) and GPIO 22 (SCL)
  Wire.begin(21, 22);

  // Initialize MCP23017 chips using Bertrand's library syntax
  mcp1.init();
  mcp2.init();

  // Configure all ports as outputs (0 = output) using Bertrand's enum syntax
  mcp1.portMode(MCP23017Port::A, 0);
  mcp1.portMode(MCP23017Port::B, 0);
  mcp2.portMode(MCP23017Port::A, 0);
  mcp2.portMode(MCP23017Port::B, 0);

  NodeID nodeid(NODE_ADDRESS);
  Olcb_init(nodeid, RESET_TO_FACTORY_DEFAULTS);
}

void loop() {
  bool activity = Olcb_process();
  doActions();
}