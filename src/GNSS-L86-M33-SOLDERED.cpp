/**
 **************************************************
 *
 * @file        GNSS-L86-M33-SOLDERED.cpp
 * @brief       Example functions to overload in base class.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     borna@soldered.com
 ***************************************************/


#include "GNSS-L86-M33-SOLDERED.h"

/**
 * @brief                   Sensor specific native constructor.
 *
 * @param int _pin          Example parameter.
 */
GNSS::GNSS(int _rx, int _tx) : gnssSerial(_rx, _tx)
{
    rxPin = _rx;
    txPin = _tx;
    native = 1;
}

/**
 * @brief                   Overloaded function for virtual in base class to initialize sensor specific.
 */
void GNSS::initializeNative()
{
    gnssSerial.begin(9600);
}
