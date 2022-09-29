/**
 **************************************************
 *
 * @file        GNSS-L86-M33-SOLDERED.cpp
 * @brief       Source file for L86-M33 GNSS breakout
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     borna@soldered.com
 ***************************************************/


#include "GNSS-L86-M33-SOLDERED.h"

/**
 * @brief                   L86-M33 GNSS Library constructor.
 *
 * @param int _rx           UART receive data pin (connected to the TX data pin on GNSS module)
 * @param int _tx           UART transmit data pin (connected to the RX data pin on GNSS module)
 */
GNSS::GNSS(int _rx, int _tx) : gnssSerial(_rx, _tx)
{
    rxPin = _rx;
    txPin = _tx;
    native = 1;
}

/**
 * @brief                    Initializer function for L86-M33 breakout board.
 */
void GNSS::initializeNative()
{
    gnssSerial.begin(9600);
}
