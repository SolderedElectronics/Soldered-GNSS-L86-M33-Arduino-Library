/**
 **************************************************
 *
 * @file        GNSS-L86-M33-SOLDERED.h
 * @brief       Header file for sensor specific code.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     borna@soldered.com
 ***************************************************/

#ifndef __GNSS_L86_M33_SOLDERED_H__
#define __GNSS_L86_M33_SOLDERED_H__

#include "Arduino.h"
#include "libs/Generic-easyC/easyC.hpp"
#include "libs/TinyGPSPlus/src/TinyGPSPlus.h"

#if defined(ARDUINO_ESP32_DEV)
#include "libs/ESPSoftwareSerial/ESPSoftwareSerial.h"
#else
#include "SoftwareSerial.h"
#endif

// Enum used for selecting which data to retrieve from EasyC module
// Numbers are arbitrary, as it's just a selector
enum GNSS_DATA_SELECT
{
    IS_INITIALIZED = 30,
    LATITUDE = 0,
    LONGITUDE = 1,
    YEAR = 2,
    MONTH_DAY = 3,
    HOUR_MINUTE = 4,
    SECOND_CENTISECOND = 5,
    SPEED = 6,
    ALTITUDE = 7,
    IS_AVAILABLE = 8,
    ALWAYS_LOCATE_ON = 9,
    ALWAYS_LOCATE_OFF = 10,
    MULTI_TONE_AIC_ON = 11,
    MULTI_TONE_AIC_OFF = 12,
    NUM_SATS = 13,
    HDOP = 14,
    AGE = 15,
    DATE_AGE = 16,
    DEG_COURSE = 17,
    CHARS_PROCESSED = 18,
    SENTENCES_W_FIX = 19,
    FAILED_CHECKSUM = 20
};

class GNSS : public EasyC, public TinyGPSPlus
{
  public:
    // easyC Functions
    GNSS();
    float getLatitude();
    float getLongitude();
    uint16_t getYear();
    uint8_t getMonth();
    uint8_t getDay();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSeconds();
    float getSpeed();
    float getAltitude();
    uint32_t getNumSatellites();
    float getHDOP();
    uint32_t getAge();
    uint32_t getDateTimeAge();
    float getCourseDeg();
    uint32_t getNumCharsProcessed();
    uint32_t getNumSentencesWithFix();
    uint32_t getNumFailedChecksums();
    bool GNSSAvailable();
    void setMultiToneAIC(bool _b);
    void setAlwaysLocate(bool _b);

    // Native Functions, uses TinyGPSPlus library
    GNSS(int _rx, int _tx);
    SoftwareSerial *gnssSerial;
    void sendCommand(char *_cmd);
    void sendChecksum(char *_s);
    char intToHexChar(int _c);

  protected:
    void initializeNative();

  private:
    int rxPin;
    int txPin;
};

#endif
