/* 
 *  Robot7 Sensor Network: Node
 *  To be used with Moteino/RFM12B
 */

#include <RFM12B.h>
#include "r7config.h"
#include "utils.h"

#define RADIO_FREQUENCY   RF12_868MHZ            // Radio and frequency (default = RFM12B/868MHz)
#define RADIO_POWER       0                      // 0 = maximum, 7 = minimum
#define RADIO_KBPS        20                     // 20 = 16.42KBps (default = 8)
#define KEY               "Z4NZ1B4RZ4NZ1B4R"     // 16 bytes - keep the same on all encrypted nodes
#define REQUESTACK        true                   // whether to request ACKs for sent messages
#define RF_ACK_MS         50                     // # of ms to wait for an RF ack
#define SERIAL_BAUD       115200

// Globals - tsk, tsk
String __input = "";                             // a string to hold incoming data
boolean __inputDone = false;                     // whether the input is complete

R7Config __r7cfg;
RFM12B __radio;
char* __sendBuffer = NULL;

// wait up to RF_ACK_MS for proper RF ACK
// TODO: handle wait for ACK asynchronously
static bool waitForAck(byte remoteNodeId) {
    long now = millis();
    while (millis() - now <= RF_ACK_MS) {
        if (__radio.ACKReceived(remoteNodeId)) { return true; }
    }
    // Timeout
    return false;
}

void sendRfMessage(byte remoteNodeId)
{
    sprintf(__sendBuffer, "t");
    __radio.Send(remoteNodeId, __sendBuffer, 1, REQUESTACK);
    Serial.print("Request sent to remote node ");
    Serial.print(remoteNodeId);
    Serial.println(waitForAck(remoteNodeId) ? " : ACK OK" : " : no ACK reply");
    localBlink();
}

/*
 * SerialEvent occurs whenever a new data comes in the
 * hardware serial RX.  This routine is run between each
 * time loop() runs, so using delay inside loop can delay
 * response.  Multiple bytes of data may be available.
 */
void serialEvent() {
    while (Serial.available())
    {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        __input += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\r') {
            __inputDone = true;
        }
    }
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    __input.reserve(64);

    Serial.print("Using RF network ID:");
    Serial.print(__r7cfg.getNetworkId());
    Serial.print(", node ID:");
    Serial.println(__r7cfg.getNodeId());

    __radio.Initialize(__r7cfg.getNodeId(), RADIO_FREQUENCY, __r7cfg.getNetworkId(), RADIO_POWER, RADIO_KBPS);
    __radio.Encrypt((uint8_t*)KEY);
}

void loop()
{
    // TODO: handle serial and RF asynchronously
    if (__inputDone) {

        __input.trim();
        __input.toUpperCase();

        // Serial-only commands
        // Handle input: SET networkId:nodeId
        // Rationale: the network and node Ids should only be settable via 
        //            the serial interface to avoid accidental isolation
        if (__input.startsWith("SET")) {

            String params = __input.substring(__input.indexOf(' '));

            byte networkId = (byte)(getValue(params, ':', 0)).toInt();
            byte nodeId = (byte)(getValue(params, ':', 1)).toInt();

             if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128)) {
                __r7cfg.setId(networkId, nodeId);
                Serial.print("Set RF network ID: ");
                Serial.print(networkId);
                Serial.print(", node ID:");
                Serial.println(nodeId);
            }
            else {
                Serial.println("NACK");
            }
        }

        // Distributed commands
        // Handle input: PRINT networkId:nodeId
        if (__input.startsWith("PRINT")) {

            String params = __input.substring(__input.indexOf(' '));

            byte networkId = (byte)(getValue(params, ':', 0)).toInt();
            byte nodeId = (byte)(getValue(params, ':', 1)).toInt();

            if (((networkId == 0) && (nodeId == 0)) || 
                ((networkId == __r7cfg.getNetworkId()) && (nodeId == __r7cfg.getNodeId()))) {
                    String config;
                    __r7cfg.prettyPrint(config);
                    Serial.println(config);
            }
            else if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128)) {
                // TODO: actual RF work
                Serial.println("Remote node PRINT");
            }
            else {
                Serial.println("NACK");
            }
        }
        // Handle input: RESET networkId:nodeId
        else if (__input.startsWith("RESET")) {

            String params = __input.substring(__input.indexOf(' '));

            byte networkId = (byte)(getValue(params, ':', 0)).toInt();
            byte nodeId = (byte)(getValue(params, ':', 1)).toInt();


            // Validate input values
            if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128)) {
                Serial.print("Soft reset node at network ID: ");
                Serial.print(networkId);
                Serial.print(", node ID:");
                Serial.println(nodeId);

                // Local node
                if (networkId == __r7cfg.getNetworkId() && nodeId == __r7cfg.getNodeId()) {
                    delay(25);
                    softReset();
                }
                else {
                    Serial.println("Remote node RESET");
                    // TODO: actual RF work
                }
            }
            else {
                Serial.println("NACK");
            }
        }

        // Handle input: PULSE networkId:nodeId:pin:seconds
        else if (__input.startsWith("PULSE")) {

            String params = __input.substring(__input.indexOf(' '));

            byte networkId = (byte)(getValue(params, ':', 0)).toInt();
            byte nodeId = (byte)(getValue(params, ':', 1)).toInt();
            byte pin = (byte)(getValue(params, ':', 2)).toInt();
            byte seconds = (byte)(getValue(params, ':', 3)).toInt();

            // Validate input values
            if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128) && 
                (pin > 0) && (pin < 14) && (seconds > 0) && (seconds < 60)) {
                Serial.println("Pulsing pin ");
                Serial.print(pin);
                Serial.print(" for ");
                Serial.print(seconds);
                Serial.print("s on network ID:");
                Serial.print(networkId);
                Serial.print(", node ID:");
                Serial.println(nodeId);
                Serial.println("Remote node PULSE");
                // TODO: actual RF work
            }
            else {
                Serial.println("NACK");
            }

        }

        // Handle input: SAMPLE networkId:nodeId:pin:samples
        else if (__input.startsWith("SAMPLE")) {

            String params = __input.substring(__input.indexOf(' '));

            byte networkId = (byte)(getValue(params, ':', 0)).toInt();
            byte nodeId = (byte)(getValue(params, ':', 1)).toInt();
            byte pin = (byte)(getValue(params, ':', 2)).toInt();
            byte samples = (byte)(getValue(params, ':', 3)).toInt();

            // Validate input values
            if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128) && 
                (pin > 0) && (pin < 14) && (samples > 0) && (samples < 256)) {
                Serial.println("Do sample");
            }
            else {
                Serial.println("NACK");
            }
        }
        else {
            Serial.println("NACK");
        }

        // clear the string:
        __input = "";
        __inputDone = false;
    }
  
    //sendRfMessage(remoteNodeId);

    // display received RF message on serial
    if (__radio.ReceiveComplete()) {
        if (__radio.CRCPass()) {
            Serial.print('['); Serial.print(__radio.GetSender(), DEC); Serial.print("] ");
            for (byte i = 0; i < *__radio.DataLen; i++) {
                Serial.print((char)__radio.Data[i]);
            }
      
            if (__radio.ACKRequested()) {
                __radio.SendACK();
                Serial.print(" - ACK sent");
            }
        }
        else {
            Serial.print("BAD-CRC");
        }
    
        Serial.println();
    }
}
