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
#include "libs/TinyGPSPlus/src/TinyGPSPlus.h"
#include "libs/Generic-easyC/easyC.hpp"

#if defined(ARDUINO_ESP32_DEV)
#include "libs/ESPSoftwareSerial/ESPSoftwareSerial.h"
#else
#include "SoftwareSerial.h"
#endif

class GNSS : public EasyC, public TinyGPSPlus
{
  public:
    GNSS(int _rx, int _tx);
    SoftwareSerial gnssSerial;
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
