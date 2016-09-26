#include "EmonLib.h"             // Include Emon Library

#define _BAUDRATE        9600
#define _SAMPLEPIN1      1
#define _SAMPLEPIN2      2
#define _SAMPLEPIN3      3
#define _SAMPLES         1000
#define _ONPIN           10
#define _OFFPIN          11

EnergyMonitor emon1;             // Create an instance
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
    Serial.begin(_BAUDRATE);
    emon1.current(_SAMPLEPIN1, 111.1);     // Current: input pin, calibration.
    pinMode(_ONPIN, OUTPUT);
    pinMode(_OFFPIN, OUTPUT);
    inputString.reserve(64);
}


void loop()
{
    if (stringComplete) {
        inputString.trim();
        if (inputString.equals("SAMPLE")) {
            // Calculate Root-mean-square of the current
            double Irms1 = emon1.calcIrms(_SAMPLES);

            Serial.print("OK:[1:");Serial.print(Irms1);Serial.print(",");
            // TODO: SAMPLE Line 2
            Serial.print("2:");Serial.print("NONE");Serial.print(",");
            // TODO: SAMPLE Line 3
            Serial.print("3:");Serial.print("NONE");Serial.println("]");
        }
        else if (inputString.equals("PULSEON")) {
            // TODO: PULSEON
            Serial.print("EXEC:");
            digitalWrite(_ONPIN, HIGH);
            delay(2000);
            digitalWrite(_ONPIN, LOW);
            Serial.println("OK");
        }
        else if (inputString.equals("PULSEOFF")) {
            // TODO: PULSEOFF
            digitalWrite(_OFFPIN, HIGH);
            delay(2000);
            digitalWrite(_OFFPIN, LOW);
            Serial.println("OK");
        }
        else {
            Serial.println("NACK");
        }

        // clear the string:
        inputString = "";
        stringComplete = false;
      }
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

void serialEvent() {
    while (Serial.available())
    {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\r') {
            stringComplete = true;
        }
    }
}
