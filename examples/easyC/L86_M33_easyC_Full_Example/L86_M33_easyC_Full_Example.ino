/**
 **************************************************
 *
 * @file        L86_M33_easyC_Full_Example.ino
 * @brief       This code will show all options that library provides to use with easyC. It will show classic GPS Lat &
 *Lon and time & date, but it will also show speed and fix status (and also checksum data) on the Arduino Serial
 *Monitor.
 *
 *              For best results, GNSS module must be outside!
 *
 *              Soldered L86-M33 GNSS easyC Breakout: www.solde.red/333213
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 *
 * @authors     robert@soldered.com
 ***************************************************/

#include "GNSS-L86-M33-SOLDERED.h"

// Create the GNSS object
GNSS gnss;

// The default I2C address is 0x30
// You may change the address via the switches on the front of the board
// In this table, 'x' represents a switch toggled 'UP'
// Address | 1 | 2 | 3 |
// 0x30    |   |   |   |
// 0x31    |   |   | x |
// 0x32    |   | x |   |
// 0x33    |   | x | x |
// 0x34    | x |   |   |
// 0x35    | x |   | x |
// 0x36    | x | x |   |
// 0x37    | x | x | x |

void setup()
{
    // Enable Serial output
    Serial.begin(115200);

    // To use a custom I2C address, use begin(0x34), for example
    if (!gnss.begin())
    {
        Serial.println("GNSS not found!");
        while (true)
        {
            delay(100);
        }
    }

    Serial.println("GNSS initialized!");

    // Turn on AlwaysLocateTM
    gnss.setAlwaysLocate(true);

    // Show start of the table with all data.
    Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  "
                     "Distance Course Card  Chars Sentences Checksum"));
    Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- "
                     "to ZAGREB  ----  RX    RX        Fail"));
    Serial.println(F("-------------------------------------------------------------------------------------------------"
                     "---------------------------------------"));
}

void loop()
{
    // Wait while the GNSS module has not yet read any useful data
    // Code will halt here until first good Latitude and Longitude data is read and stored
    // Depending on your antenna, it could take a couple seconds
    while (!gnss.GNSSAvailable())
    {
        delay(1);
    }

    static const double osijekLat = 40.7128, osijekLon = -74.0060;

    // Get all the data via easyC
    // Note the data types
    uint32_t numSats = gnss.getNumSatellites(); // How many satellites are available
    float hdop = gnss.getHDOP();                // GPS HDOP value
    float latitude = gnss.getLatitude();
    float longitude = gnss.getLongitude();
    uint32_t age = gnss.getAge(); // How old is the last written locaiton (in ms)

    // Get DateTime
    uint8_t month = gnss.getMonth();
    uint8_t day = gnss.getDay();
    uint16_t year = gnss.getYear();
    uint8_t hour = gnss.getHour();
    uint8_t minute = gnss.getMinute();
    uint8_t second = gnss.getSeconds();
    uint32_t dateTimeAge = gnss.getDateTimeAge(); // How old is the last written date (in ms)

    // Get misc data
    float altitude = gnss.getAltitude();
    float courseDeg = gnss.getCourseDeg();
    float speed = gnss.getSpeed();

    // Get data related to communication between the easyC board (ATTiny) and L86
    uint32_t charsProcessed = gnss.getNumCharsProcessed();
    uint32_t sentencesWithFix = gnss.getNumSentencesWithFix();
    uint32_t failedChecksum = gnss.getNumFailedChecksums();


    // Print all available data into the serial monitor (lat, lon, GNSS Fix Age, T&D, speed, altitude)
    printInt(gnss.getNumSatellites(), 5);
    printFloat(gnss.getHDOP(), 6, 1);
    printFloat(gnss.getLatitude(), 11, 6);
    printFloat(gnss.getLongitude(), 11, 6);
    printInt(gnss.getAge(), 5);
    printDateTime(month, day, year, hour, minute, second, dateTimeAge);
    printFloat(altitude, 7, 2);
    printFloat(courseDeg, 7, 2);
    printFloat(speed, 7, 2);
    printStr(GNSS::cardinal(courseDeg), 6);

    unsigned long distanceKmToOsijek =
        (unsigned long)GNSS::distanceBetween(latitude, longitude, osijekLat, osijekLon) / 1000;
    printInt(distanceKmToOsijek, 9);

    double courseToOsijek = GNSS::courseTo(latitude, longitude, osijekLat, osijekLon);

    printFloat(courseToOsijek, 7, 2);

    const char *cardinalToOsijek = GNSS::cardinal(courseToOsijek);

    printStr(cardinalToOsijek, 6);

    printInt(charsProcessed, 6);
    printInt(sentencesWithFix, 10);
    printInt(failedChecksum, 9);
    Serial.println();

    delay(1000);
}

// Functions used for printing floating numbers more easily
static void printFloat(float val, int len, int prec)
{

    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
        Serial.print(' ');
}

// Function that prints integers (with unsigned long data type).
static void printInt(unsigned long val, int len)
{
    char sz[32] = "*****************";
    sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    Serial.print(sz);
}

// Function that print date and time from TinyGPS library date format.
static void printDateTime(uint8_t month, uint8_t day, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second,
                          uint32_t dateTimeAge)
{
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", month, day, year);
    Serial.print(sz);

    sprintf(sz, "%02d:%02d:%02d ", hour, minute, second);
    Serial.print(sz);

    printInt(dateTimeAge, 5);
}

// Function used to print strings more easly.
static void printStr(const char *str, int len)
{
    int slen = strlen(str);
    for (int i = 0; i < len; ++i)
        Serial.print(i < slen ? str[i] : ' ');
}
