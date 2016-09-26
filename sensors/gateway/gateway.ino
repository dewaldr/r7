/* 
 *  Robot7 Sensor Config Sketch
 *  To be used with Moteino/RFM12B
 */

#include <RFM12B.h>
#include "r7config.h"
#include "utils.h"


/* Global variables
 */
String __serial = "";                             // a string to hold incoming data
boolean __serialDone = false;                     // whether the input is complete
R7Config __r7cfg;
RFM12B __radio;


/* Function prototpes
 */
void setup();
void loop();
static bool waitForRfAck(byte remoteNodeId);
void sendRfMessage(const String& msg, const byte remoteNodeId);
void serialEvent();
int handleCommand(String& input, String& output);


/* Implementation
 */
void setup()
{
    Serial.begin(SERIAL_BAUD);
    __serial.reserve(64);

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
    if (__serialDone) {

        String output; 
        int res = handleCommand(__serial, output);
    
        Serial.println(output);

        // clear and reset the serial input
        __serial = "";
        __serialDone = false;
    }
  
    //sendRfMessage(remoteNodeId);

    // display received RF message on serial
    if (__radio.ReceiveComplete()) {
        if (__radio.CRCPass()) {
            Serial.print('[');Serial.print(__radio.GetSender(), DEC);Serial.print("] ");
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

// wait up to RF_ACK_MS for proper RF ACK
// TODO: handle wait for ACK asynchronously
static bool waitForRfAck(byte remoteNodeId) {
    long now = millis();
    while (millis() - now <= RF_ACK_MS) {
        if (__radio.ACKReceived(remoteNodeId)) { return true; }
    }
    // Timeout
    return false;
}

void sendRfMessage(const String& msg, const byte remoteNodeId)
{
    char* buffer = NULL;
    sprintf(buffer, "t");
    __radio.Send(remoteNodeId, buffer, 1, REQUESTACK);
    Serial.print("Request sent to remote node ");
    Serial.print(remoteNodeId);
    Serial.println(waitForRfAck(remoteNodeId) ? " : ACK OK" : " : no ACK reply");
    localBlink();
}

/* SerialEvent occurs whenever a new data comes in the
 * hardware serial RX. 
 */
void serialEvent() {
    while (Serial.available())
    {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        __serial += inChar;
        // if the incoming character is a carriage return, set a flag
        // so the main loop can do something about it:
        if (inChar == '\r') {
            __serialDone = true;
        }
    }
}

int handleCommand(String& input, String& output)
{
    input.trim();
    input.toUpperCase();

    // Local commands
    /* Handle input: SET networkId:nodeId
     * Rationale: the network and node Ids should only be settable via 
     *            the serial interface to avoid accidental isolation
     */
    if (input.startsWith("SET")) {

        String params = input.substring(input.indexOf(' '));

        byte networkId = (byte)(getValue(params, ':', 0)).toInt();
        byte nodeId = (byte)(getValue(params, ':', 1)).toInt();

         if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128)) {
            __r7cfg.setId(networkId, nodeId);
            output = "Set RF network ID: ";
            output += networkId;
            output += ", node ID:";
            output += nodeId;
        }
        else {
            output = "NACK";
        }
    }

    // Distributed commands
    /* Handle input: PRINT networkId:nodeId
     */
    if (input.startsWith("PRINT")) {

        String params = input.substring(input.indexOf(' '));

        byte networkId = (byte)(getValue(params, ':', 0)).toInt();
        byte nodeId = (byte)(getValue(params, ':', 1)).toInt();

        if (((networkId == 0) && (nodeId == 0)) || 
            ((networkId == __r7cfg.getNetworkId()) && (nodeId == __r7cfg.getNodeId()))) {
                __r7cfg.prettyPrint(output);
        }
        else if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128)) {
            // TODO: actual RF work
            output += "Remote node PRINT";
        }
        else {
            output = "NACK";
        }
    }

    // Handle input: RESET networkId:nodeId
    else if (input.startsWith("RESET")) {

        String params = input.substring(input.indexOf(' '));

        byte networkId = (byte)(getValue(params, ':', 0)).toInt();
        byte nodeId = (byte)(getValue(params, ':', 1)).toInt();


        // Validate input values
        if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128)) {
            output = "Soft reset node at network ID: ";
            output += networkId;
            output += ", node ID:";
            output += nodeId;

            // Local node
            if (networkId == __r7cfg.getNetworkId() && nodeId == __r7cfg.getNodeId()) {
                delay(25);
                softReset();
            }
            else {
                // TODO: actual RF work
                output = "Remote node RESET";
            }
        }
        else {
            output = "NACK";
        }
    }

    // Handle input: PULSE networkId:nodeId:pin:seconds
    else if (input.startsWith("PULSE")) {

        String params = input.substring(input.indexOf(' '));

        byte networkId = (byte)(getValue(params, ':', 0)).toInt();
        byte nodeId = (byte)(getValue(params, ':', 1)).toInt();
        byte pin = (byte)(getValue(params, ':', 2)).toInt();
        byte seconds = (byte)(getValue(params, ':', 3)).toInt();

        // Validate input values
        if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128) && 
            (pin > 0) && (pin < 14) && (seconds > 0) && (seconds < 60)) {
            output = "Pulsing pin ";
            output += pin;
            output += " for ";
            output += seconds;
            output += "s on network ID:";
            output += networkId;
            output += ", node ID:";
            output += nodeId;
            output += "Remote node PULSE";
            // TODO: actual RF work
        }
        else {
            output = "NACK";
        }

    }

    // Handle input: SAMPLE networkId:nodeId:pin:samples
    else if (input.startsWith("SAMPLE")) {

        String params = input.substring(input.indexOf(' '));

        byte networkId = (byte)(getValue(params, ':', 0)).toInt();
        byte nodeId = (byte)(getValue(params, ':', 1)).toInt();
        byte pin = (byte)(getValue(params, ':', 2)).toInt();
        byte samples = (byte)(getValue(params, ':', 3)).toInt();

        // Validate input values
        if ((networkId > 0) && (networkId < 128) && (nodeId > 0) && (nodeId < 128) && 
            (pin > 0) && (pin < 14) && (samples > 0) && (samples < 256)) {
            output = "Do sample";
        }
        else {
            output = "NACK";
        }
    }
    else {
        output = "NACK";
    }

    return 0;
}

