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
    // Set RX and TX pin
    rxPin = _rx;
    txPin = _tx;
    native = 1;
}

/**
 * @brief                    Initializer function for L86-M33 breakout board.
 */
void GNSS::initializeNative()
{
    // Init software serial
    gnssSerial.begin(9600);
}

/**
 * @brief                   Sends a custom command to the GNSS module
 *
 * @param char *_cmd        Command string
 *
 * @note                    There must be no checksum at the end of the command. It will be calculated and added
 * automatically.
 */
void GNSS::sendCommand(char *_cmd)
{
    // First send the command
    gnssSerial.print(_cmd);

    // Calculate and send the checksum (and also add LF & CR)
    sendChecksum(_cmd);
}

/**
 * @brief                   Sends a checksum to the GNSS module for the sent command.
 *
 * @param char *_s          Command string
 *
 * @note                    There must be no checksum at the end of the command. It will be calculated and added
 * automatically.
 */
void GNSS::sendChecksum(char *_s)
{
    // Skip '$' in NMEA message
    int i = 1;

    // Variable for the checksum result
    int _sum = 0;

    // Calculate the checksum with XOR operation (do XOR with every byte in the message).
    while (_s[i] != 0)
    {
        _sum ^= _s[i];
        i++;
    }

    // Limit che checksum to 8 bit.
    _sum &= 0xff;

    // Send checksum to the GNSS module.
    gnssSerial.print('*');
    gnssSerial.write(intToHexChar(_sum / 16));
    gnssSerial.write(intToHexChar(_sum % 16));
    gnssSerial.println();
}

/**
 * @brief                   Converts a integer into one HEX Char (for 10 as input it will return 'A')
 *
 * @param int _c            Integer that needs to be converted into HEX Char
 *
 * @return char             HEX Char (for 10 as input, result will be char 'A')
 */
char GNSS::intToHexChar(int _c)
{
    return (_c > 10 ? 'A' + (_c - 10) : '0' + _c);
}