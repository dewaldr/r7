/***********************************************************************
 *
 * call the spiWriteRead function on the a2d object 20 times. Each time make sure
 * that conversion is configured for single ended conversion on CH0
 * i.e. transmit ->  byte1 = 0b00000001 (start bit)
 *                   byte2 = 0b1000000  (SGL/DIF = 1, D2=D1=D0=0)
 *                   byte3 = 0b00000000  (Don't care)
 *      receive  ->  byte1 = junk
 *                   byte2 = junk + b8 + b9
 *                   byte3 = b7 - b0
 *    
 * after conversion must merge data[1] and data[2] to get final result
 *
 * *********************************************************************/
#include <iostream>
#include "mcp3008Spi.h"

// The bus speed seems to affect the ADC resolution
int spi_speed = 1000000 * 1;
float factor = 80.2;

using namespace std;

int main(void)
{
    mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, spi_speed, 8);
    int a2dVal = 0;
    int a2dChannel = 0;
    unsigned char data[3];

    data[0] = 1;  //  first byte transmitted -> start bit
    data[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
    data[2] = 0; // third byte transmitted....don't care

    a2d.spiWriteRead(data, sizeof(data) );

    a2dVal = 0;
    a2dVal = (data[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
    a2dVal |=  (data[2] & 0xff);

    // cout << "The ADC returned: " << a2dVal << endl;

    float voltage = 0;

    // Use simple factor conversion, accurate at the top end of the scale
    if (a2dVal > 0) {
        voltage = a2dVal / factor;
    }

    cout << fixed;
    cout.precision(1);
    cout << "The measured voltage is: " << voltage << endl;

    return 0;
}
