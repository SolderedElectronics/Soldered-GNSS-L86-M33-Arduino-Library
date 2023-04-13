/**
 **************************************************
 *
 * @file        GNSS-L86-M33-SOLDERED.cpp
 * @brief       Source file for L86-M33 GNSS breakout (native and easyC version)
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     borna@soldered.com
 ***************************************************/


#include "GNSS-L86-M33-SOLDERED.h"

/**
 * @brief                   L86-M33 GNSS Library easyC constructor.
 *
 */
GNSS::GNSS()
{
    native = 0;
}

/**
 * @brief                   Get the last measured latitude by the GPS module
 *
 * @returns                 float value of the latitude
 */
float GNSS::getLatitude()
{
    char buf[16];
    sendAddress(LATITUDE);
    readData(buf, 16);
    float latitude = atof(buf);
    return latitude;
}

/**
 * @brief                   Get the last measured longitude by the GPS module
 *
 * @returns                 float value of the longitude
 */
float GNSS::getLongitude()
{
    char buf[16];
    sendAddress(LONGITUDE);
    readData(buf, 16);
    float longitude = atof(buf);
    return longitude;
}

/**
 * @brief                   Get the last measured year by the GPS module
 *
 * @returns                 uint16_t value of the year
 */
uint16_t GNSS::getYear()
{
    char buf[2];
    uint16_t year;
    sendAddress(YEAR);
    readData(buf, 2);
    memcpy(&year, buf, 2);
    return year;
}

/**
 * @brief                   Get the last measured month by the GPS module
 *
 * @returns                 uint8_t value of the month
 */
uint8_t GNSS::getMonth()
{
    char buf[2];
    uint8_t month;
    sendAddress(MONTH_DAY);
    readData(buf, 2);
    memcpy(&month, buf, 1);
    return month;
}

/**
 * @brief                   Get the last measured day by the GPS module
 *
 * @returns                 uint8_t value of the day
 */
uint8_t GNSS::getDay()
{
    char buf[2];
    uint8_t day;
    sendAddress(MONTH_DAY);
    readData(buf, 2);
    memcpy(&day, buf + 1, 1);
    return day;
}

/**
 * @brief                   Get the last measured hour by the GPS module
 *
 * @returns                 uint8_t value of the hour
 */
uint8_t GNSS::getHour()
{
    char buf[2];
    uint8_t hour;
    sendAddress(HOUR_MINUTE);
    readData(buf, 2);
    memcpy(&hour, buf, 1);
    return hour;
}

/**
 * @brief                   Get the last measured minute (in current hour) by the GPS module
 *
 * @returns                 uint8_t value of the minute
 */
uint8_t GNSS::getMinute()
{
    char buf[2];
    uint8_t minute;
    sendAddress(HOUR_MINUTE);
    readData(buf, 2);
    memcpy(&minute, buf + 1, 1);
    return minute;
}

/**
 * @brief                   Get the last measured seconds (in current minute) by the GPS module
 *
 * @returns                 float value of the seconds
 */
uint8_t GNSS::getSeconds()
{
    char buf[2];
    uint8_t seconds;
    sendAddress(SECOND_CENTISECOND);
    readData(buf, 2);
    memcpy(&seconds, buf, 1);
    return seconds;
}

/**
 * @brief                   Get the last measured speed by the GPS module
 *
 * @returns                 float value of the speed
 */
float GNSS::getSpeed()
{
    char buf[16];
    sendAddress(SPEED);
    readData(buf, 16);
    float speed = atof(buf);
    return speed;
}

/**
 * @brief                   Get the last measured altitude by the GPS module
 *
 * @returns                 float value of the altitude in meters
 */
float GNSS::getAltitude()
{
    char buf[16];
    sendAddress(ALTITUDE);
    readData(buf, 16);
    float altitude = atof(buf);
    return altitude;
}

/**
 * @brief                   Get the number of GPS Satellites available
 *
 * @returns                 uint32_t value of the number of Satellites available
 */
uint32_t GNSS::getNumSatellites()
{
    char buf[4];
    sendAddress(NUM_SATS);
    readData(buf, 4);
    uint32_t numSats;
    numSats = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
    return numSats;
}

/**
 * @brief                   Get HDOP value (precision indicator of GPS)
 *
 * @returns                 float value of HDOP
 */
float GNSS::getHDOP()
{
    char buf[16];
    sendAddress(HDOP);
    readData(buf, 16);
    float hdop = atof(buf);
    return hdop;
}

/**
 * @brief                   Get how long ago the last location was measured (in ms)
 *
 * @returns                 uint32_t number of ms since last DateTime
 */
uint32_t GNSS::getAge()
{
    char buf[4];
    sendAddress(AGE);
    readData(buf, 4);
    uint32_t age;
    age = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
    return age;
}

/**
 * @brief                   Get how long ago the last GPS DateTime was recorded (in ms)
 *
 * @returns                 uint32_t number of ms since last DateTime
 */
uint32_t GNSS::getDateTimeAge()
{
    char buf[4];
    sendAddress(DATE_AGE);
    readData(buf, 4);
    uint32_t dateTimeAge;
    dateTimeAge = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
    return dateTimeAge;
}

/**
 * @brief                   Get how long ago the last GPS DateTime was recorded (in ms)
 *
 * @returns                 uint32_t number of ms since last DateTime
 */
float GNSS::getCourseDeg()
{
    char buf[16];
    sendAddress(DEG_COURSE);
    readData(buf, 16);
    float courseDeg = atof(buf);
    return courseDeg;
}

/**
 * @brief                   Get the number of processed chars sent by the GPS module
 *
 * @returns                 uint32_t number of processed chars
 */
uint32_t GNSS::getNumCharsProcessed()
{
    char buf[4];
    sendAddress(CHARS_PROCESSED);
    readData(buf, 4);
    uint32_t numProcessedChars;
    numProcessedChars = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
    return numProcessedChars;
}

/**
 * @brief                   Get the number of $GPRMC or $GPGGA sentences that had a fix
 *
 * @returns                 uint32_t number of sentences that had a fix
 */
uint32_t GNSS::getNumSentencesWithFix()
{
    char buf[4];
    sendAddress(SENTENCES_W_FIX);
    readData(buf, 4);
    uint32_t numSentencesWithFix;
    numSentencesWithFix = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
    return numSentencesWithFix;
}

/**
 * @brief                   Get the number of sentences of all types that failed the checksum test
 *
 * @returns                 uint32_t number of sentences of all types that failed the checksum test
 */
uint32_t GNSS::getNumFailedChecksums()
{
    char buf[4];
    sendAddress(FAILED_CHECKSUM);
    readData(buf, 4);
    uint32_t numFailedChecksums;
    numFailedChecksums = (uint32_t)buf[0] << 24 | (uint32_t)buf[1] << 16 | (uint32_t)buf[2] << 8 | (uint32_t)buf[3];
    return numFailedChecksums;
}


/**
 * @brief                   Get if the GPS module has sucessfuly decoded some data
 *
 * @returns                 bool true if yes, false if no
 */
bool GNSS::GNSSAvailable()
{
    char buf[1];
    sendAddress(IS_AVAILABLE);
    readData(buf, 1);
    if (buf[0] == '1')
        return true;
    else
        return false;
}

/**
 * @brief                   Turn Multi Tone AIC (Active Interference Cancellation) on or off
 *
 * @param                   _b True if it's to be turned on, false to turn off
 *
 * @returns                 None
 */
void GNSS::setMultiToneAIC(bool _b)
{
    char buf[1];
    if (_b)
    {
        sendAddress(MULTI_TONE_AIC_ON);
    }
    else
    {
        sendAddress(MULTI_TONE_AIC_OFF);
    }
}

/**
 * @brief                   Turn AlwaysLocateTM mode on or off
 *
 * @param                   _b True if it's to be turned on, false to turn off
 *
 * @returns                 None
 */
void GNSS::setAlwaysLocate(bool _b)
{
    char buf[1];
    if (_b)
    {
        sendAddress(ALWAYS_LOCATE_ON);
    }
    else
    {
        sendAddress(ALWAYS_LOCATE_OFF);
    }
}

/**
 * @brief                   L86-M33 GNSS Library constructor.
 *
 * @param int _rx           UART receive data pin (connected to the TX data pin on GNSS module)
 * @param int _tx           UART transmit data pin (connected to the RX data pin on GNSS module)
 */
GNSS::GNSS(int _rx, int _tx)
{
    // Set RX and TX pin
    rxPin = _rx;
    txPin = _tx;

    // Create new SoftwareSerial
    gnssSerial = new SoftwareSerial(_rx, _tx);

    native = 1;
}

/**
 * @brief                    Initializer function for L86-M33 breakout board.
 */
void GNSS::initializeNative()
{
    // Init software serial
    gnssSerial->begin(9600);
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
    gnssSerial->print(_cmd);

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
    gnssSerial->print('*');
    gnssSerial->write(intToHexChar(_sum / 16));
    gnssSerial->write(intToHexChar(_sum % 16));
    gnssSerial->println();
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
