/**
 **************************************************
 *
 * @file        L86_M33_Full_Example.ino
 * @brief       This code will show all options that library provides. It will show classic GPS Lat & Lon and time &
 *date, but it will also show speed and fix status (and also checksum data) on the Arduino Serial Monitor. At first GNSS
 *will send out incorrect data (such as wrong time and date) until gets the vaild ones.
 *
 *              For best results, GNSS module must be outside!
 *
 *              Soldered L86-M33 GNSS Breakout: www.solde.red/333201
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 *              Connections:
 *              L86-L33         Dasduino Core
 *              RX              D3
 *              TX              D4
 *              5V              VCC
 *              GND             GND
 *
 * @note        This library uses Software Serial for UART communication. Also, this GNSS has an on-board antenna, so
 *it's not needed to connect an external one. Using both at the same time can cause problems (it will take a much longer time
 *to get GNSS fix). The battery is only for RTC clock and data backup.
 *
 * @authors     Borna Biro for Soldered.com
 ***************************************************/

#include "GNSS-L86-M33-SOLDERED.h" // Include L86-L33 GNSS Library

// Define pins for the GNSS module
#define GNSS_RX 3
#define GNSS_TX 4

// Calculate the distance between the current GNSS location and the custom city (or place).
// In this case, calculate the distance between your current GNSS location and Osijek, Croatia.
#define CITY_LAT 45.5550
#define CITY_LON 18.6955;

// Create an object for the library called gps
GNSS gps(GNSS_TX, GNSS_RX);

void setup()
{
    // Init serial communication for Serial Monitor at 9600 bauds. Higher baud rate can cause problems while sending a
    // command.
    Serial.begin(9600);

    // Init L86-M33 library.
    gps.begin();

    // Print out a few new lines so the header can be displayed correctly after MCU reset.
    Serial.println("\n\n");

    // Show start of the table with all data.
    Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  "
                     "Distance Course Card  Chars Sentences Checksum"));
    Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- "
                     "to Osijek  ----  RX    RX        Fail"));
    Serial.println(F("-------------------------------------------------------------------------------------------------"
                     "---------------------------------------"));
}

void loop()
{
    static const double cityLat = CITY_LAT, cityLon = CITY_LON;

    // Print all available data into the serial monitor (lat, lon, GNSS Fix Age, T&D, speed, altitude)
    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.location.age(), gps.location.isValid(), 5);
    printDateTime(gps.date, gps.time);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
    printStr(gps.course.isValid() ? GNSS::cardinal(gps.course.deg()) : "*** ", 6);

    unsigned long distanceKmToOsijek =
        (unsigned long)GNSS::distanceBetween(gps.location.lat(), gps.location.lng(), cityLat, cityLon) / 1000;
    printInt(distanceKmToOsijek, gps.location.isValid(), 9);

    double courseToOsijek = GNSS::courseTo(gps.location.lat(), gps.location.lng(), cityLat, cityLon);

    printFloat(courseToOsijek, gps.location.isValid(), 7, 2);

    const char *cardinalToOsijek = GNSS::cardinal(courseToOsijek);

    printStr(gps.location.isValid() ? cardinalToOsijek : "*** ", 6);

    printInt(gps.charsProcessed(), true, 6);
    printInt(gps.sentencesWithFix(), true, 10);
    printInt(gps.failedChecksum(), true, 9);
    Serial.println();

    // Use this instead of the classic delay(). Using a delay insead of smartDelay() will cause
    // library to not work properly and it will start losing GPS data from the breakout.
    smartDelay(1000);

    // If no data has been received for more then 5 seconds from board power up, something is wrong... Check wiring!
    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed". Use this insead of delay. Otherwise GNSS won't wor properly.
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (gps.gnssSerial->available())
            gps.encode(gps.gnssSerial->read());
    } while (millis() - start < ms);
}

// Functions used for printing floating numbers more easly.
static void printFloat(float val, bool valid, int len, int prec)
{
    if (!valid)
    {
        while (len-- > 1)
            Serial.print('*');
        Serial.print(' ');
    }
    else
    {
        Serial.print(val, prec);
        int vi = abs((int)val);
        int flen = prec + (val < 0.0 ? 2 : 1); // . and -
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
        for (int i = flen; i < len; ++i)
            Serial.print(' ');
    }
    smartDelay(0);
}

// Functions prints integers (with unsigned long data type).
static void printInt(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    Serial.print(sz);
    smartDelay(0);
}

// Functions print date and time from TinyGPS library date format.
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
    if (!d.isValid())
    {
        Serial.print(F("********** "));
    }
    else
    {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
        Serial.print(sz);
    }

    if (!t.isValid())
    {
        Serial.print(F("******** "));
    }
    else
    {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        Serial.print(sz);
    }

    printInt(d.age(), d.isValid(), 5);
    smartDelay(0);
}

// Function used to print strings more easly.
static void printStr(const char *str, int len)
{
    int slen = strlen(str);
    for (int i = 0; i < len; ++i)
        Serial.print(i < slen ? str[i] : ' ');
    smartDelay(0);
}