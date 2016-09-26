#ifndef _R7CONFIG_H_
#define _R7CONFIG_H_

#include <Arduino.h>

#define RADIO_FREQUENCY     RF12_868MHZ            // Radio and frequency (default = RFM12B/868MHz)
#define RADIO_POWER         0                      // 0 = maximum, 7 = minimum
#define RADIO_KBPS          20                     // 20 = 16.42KBps (default = 8)
#define KEY                 "Z4NZ1B4RZ4NZ1B4R"     // 16 bytes - keep the same on all encrypted nodes
#define REQUESTACK          true                   // whether to request ACKs for sent messages
#define RF_ACK_MS           50                     // # of ms to wait for an RF ack
#define SERIAL_BAUD         115200                 // serial interface baud rate 
#define USE_DEFAULTS_ADDR   0                      // EEPROM address for 'use defaults' flag 
#define DEFAULT_NETWORKID   77                     // RF network ID (default = 77)
#define NETWORKID_ADDRESS   1                      // EEPROM address for network ID
#define DEFAULT_NODEID      10                     // Unique node ID for this unit (range: 11 - 127)
#define NODEID_ADDRESS      2                      // EEPROM address for node ID

class R7Config {

    private:
        byte _networkId;
        byte _nodeId;

        void load();
        void store();

    public:
        R7Config();
        // -- Getters
        byte getNetworkId() { return _networkId; };
        byte getNodeId() { return _nodeId; };
        void prettyPrint( String& ouput );
        // -- Setters
        void setId(const byte& networkId, const byte& nodeId);
};

#endif // _R7CONFIG_H_
