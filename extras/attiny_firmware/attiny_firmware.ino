/**
 **************************************************

   @file        attiny_firmware.ino
   @brief       Firmware for GNSS Breakout with easyC

   @note        The user must have the GNSS-L86-M33-SOLDERED library installed in order for this sketch to compile.
                You can find it at https){//github.com/SolderedElectronics/Soldered-GNSS-L86-M33-Arduino-Library


   @authors     Robert @ soldered.com
 ***************************************************/

#include "GNSS-L86-M33-SOLDERED.h"
#include "easyC.h"
#include <Wire.h>

// Serial pins
#define GNSS_RX PA1
#define GNSS_TX PA2

// Variables to store data read from the GPS module
double latitude = 999;  // Set impossible defaults
double longitude = 999; // If they have been overwritten - first reading was completed
uint16_t year = 0;
uint8_t month = 0;
uint8_t day = 0;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;
uint8_t centisecond = 0;
double speedKMPH = 0;
double altitudeMtrs = 0;
uint32_t numSatelites = 0;
double hdop = 0;
uint32_t age = 0;
uint32_t dateAge = 0;
double degCourse = 0;
uint32_t charsProcessed = 0;
uint32_t sentencesWithFix = 0;
uint32_t failedChecksum = 0;

volatile bool isInitialized = false;

// Create the GPS object
GNSS gps(GNSS_TX, GNSS_RX);

// Selector for which data we require from the main device
// Enum defined in main library file
GNSS_DATA_SELECT currentSelect;

// Commands to configure the GPS module
char restart_Cmd[] = {"$PMTK104*37"};
char enable_pps_Cmd[] = {"$PMTK255,1*2C"};
char alwaysLocate_on_Cmd[] = {"$PMTK225,8"};
char alwaysLocate_off_Cmd[] = {"$PMTK225,0*2B"};
char multitoneAIC_on_Cmd[] = {"$PMTK 286,1"};
char multitoneAIC_off_Cmd[] = {"$PMTK 286,0*22"};

// Default I2C Address
int addr = DEFAULT_ADDRESS;

void setup()
{
  initDefault();
  addr = getI2CAddress();

  // Set default value of the selector
  currentSelect = LATITUDE;

  // Begin the GPS and restart
  gps.begin();
  Serial.begin(9600);
  gps.sendCommand(restart_Cmd);
  delay(10);

  Wire.begin(addr);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{
  // Periodically read from serial, decode gps data and store in the buffers
  while (gps.gnssSerial->available() > 0)
  {
    isInitialized = true;

    if (gps.encode(gps.gnssSerial->read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        age = gps.location.age();
      }
      if (gps.date.isValid())
      {
        month = gps.date.month();
        day = gps.date.day();
        year = gps.date.year();
        dateAge = gps.date.age();
      }
      if (gps.time.isValid())
      {
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        centisecond = gps.time.centisecond();
      }
      if (gps.speed.isValid())
      {
        speedKMPH = gps.speed.kmph();
      }
      if (gps.altitude.isValid())
      {
        altitudeMtrs = gps.altitude.meters();
      }
      if (gps.satellites.isValid())
      {
        numSatelites = gps.satellites.value();
      }
      if(gps.hdop.isValid())
      {
        hdop = gps.hdop.hdop();
      }
      if(gps.course.isValid())
      {
        degCourse = gps.course.deg();
      }
      charsProcessed = gps.charsProcessed();
      sentencesWithFix = gps.sentencesWithFix();
      failedChecksum = gps.failedChecksum();
    }
  }
}

// On recieve, set the correct selector to the one of interest
void receiveEvent(int howMany)
{
  // Remember the data sent as a selector for which data to send next
  currentSelect = (GNSS_DATA_SELECT)Wire.read();

  // If a command has been requested by the main device, execute the command
  if (currentSelect == ALWAYS_LOCATE_ON)
  {
    gps.sendCommand(alwaysLocate_on_Cmd);
  }
  else if (currentSelect == ALWAYS_LOCATE_OFF)
  {
    gps.sendCommand(alwaysLocate_off_Cmd);
  }
  else if (currentSelect == MULTI_TONE_AIC_ON)
  {
    gps.sendCommand(multitoneAIC_on_Cmd);
  }
  else if (currentSelect == MULTI_TONE_AIC_OFF)
  {
    gps.sendCommand(multitoneAIC_off_Cmd);
  }
}

// Send different data depending on the last request
// The data has to be cast accordingly in order to be read on the main device
void requestEvent()
{
  if (currentSelect == LATITUDE)
  {
    char buf[16];
    dtostrf(latitude, 10, 6, buf);
    Wire.write(buf, 16);
  }

  else if (currentSelect == LONGITUDE)
  {
    char buf[16];
    dtostrf(longitude, 10, 6, buf);
    Wire.write(buf, 16);
  }

  else if (currentSelect == YEAR)
  {
    uint16_t buf[1] = {year};
    Wire.write((uint8_t *)buf, 2);
  }

  else if (currentSelect == MONTH_DAY)
  {
    uint8_t buf[2] = {month, day};
    Wire.write(buf, 2);
  }

  else if (currentSelect == HOUR_MINUTE)
  {
    uint8_t buf[2] = {hour, minute};
    Wire.write(buf, 2);
  }

  else if (currentSelect == SECOND_CENTISECOND)
  {
    uint8_t buf[2] = {second, centisecond};
    Wire.write(buf, 2);
  }

  else if (currentSelect == SPEED)
  {
    char buf[16];
    dtostrf(speedKMPH, 10, 6, buf);
    Wire.write(buf, 16);
  }

  else if (currentSelect == ALTITUDE)
  {
    char buf[16];
    dtostrf(altitudeMtrs, 10, 6, buf);
    Wire.write(buf, 16);
  }

  else if (currentSelect == IS_AVAILABLE)
  {
    char buf[1];
    // Check if the values are still set to default
    // If so, data isn't available
    if (latitude == 999 && longitude == 999)
      buf[0] = '0';
    else
      buf[0] = '1';
    Wire.write(buf, 1);
  }
  else if (currentSelect == IS_INITIALIZED)
  {
    char buf[1];
    if (isInitialized)
      buf[0] = '1';
    else
      buf[0] = '0';
    Wire.write(buf, 1);
  }

  else if (currentSelect == NUM_SATS)
  {
    // Split uint32_t to 4 uint8_t so it can be sent
    uint8_t buf[4];
    buf[0] = numSatelites >> 24;
    buf[1] = numSatelites >> 16;
    buf[2] = numSatelites >>  8;
    buf[3] = numSatelites;
    Wire.write(buf, 32);
  }

  else if (currentSelect == HDOP)
  {
    char buf[16];
    dtostrf(hdop, 10, 6, buf);
    Wire.write(buf, 16);
  }

  else if (currentSelect == AGE)
  {
    uint8_t buf[4];
    buf[0] = age >> 24;
    buf[1] = age >> 16;
    buf[2] = age >>  8;
    buf[3] = age;
    Wire.write(buf, 32);
  }

  else if (currentSelect == DATE_AGE)
  {
    uint8_t buf[4];
    buf[0] = dateAge >> 24;
    buf[1] = dateAge >> 16;
    buf[2] = dateAge >>  8;
    buf[3] = dateAge;
    Wire.write(buf, 32);
  }

  else if (currentSelect == DEG_COURSE)
  {
    char buf[16];
    dtostrf(degCourse, 10, 6, buf);
    Wire.write(buf, 16);
  }

  else if (currentSelect == CHARS_PROCESSED)
  {
    uint8_t buf[4];
    buf[0] = charsProcessed >> 24;
    buf[1] = charsProcessed >> 16;
    buf[2] = charsProcessed >>  8;
    buf[3] = charsProcessed;
    Wire.write(buf, 32);
  }

  else if (currentSelect == SENTENCES_W_FIX)
  {
    uint8_t buf[4];
    buf[0] = sentencesWithFix >> 24;
    buf[1] = sentencesWithFix >> 16;
    buf[2] = sentencesWithFix >>  8;
    buf[3] = sentencesWithFix;
    Wire.write(buf, 32);
  }

  else if (currentSelect == FAILED_CHECKSUM)
  {
    uint8_t buf[4];
    buf[0] = failedChecksum >> 24;
    buf[1] = failedChecksum >> 16;
    buf[2] = failedChecksum >>  8;
    buf[3] = failedChecksum;
    Wire.write(buf, 32);
  }
}
