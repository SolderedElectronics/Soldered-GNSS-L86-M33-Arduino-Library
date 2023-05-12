/**
 **************************************************
 *
 * @file        L86_M33_Basic_Readings.ino
 * @brief       This code will try to read GNSS Time and Date as well as GPS Latitude and Longitude and display it on
 *the Arduino Serial Monitor. Connect GNSS module and one of the Dasduino boards, upload the code and open serial
 *monitor at 9600 bauds to see the data. At first GNSS will send out incorrect data (such as wrong time and date) until
 *gets the vaild ones.
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

// Create an object for the library called gps
GNSS gps(GNSS_TX, GNSS_RX);

// Variable that keeps track when the last time GNSS data has been display on serial.
unsigned long lastGnssDisplay = 0;

void setup()
{
    // Init serial communication for Serial Monitor at 9600 bauds. Higher baud rate can cause problems while sending a
    // command.
    Serial.begin(9600);

    // Init L86-M33 library.
    gps.begin();
}

void loop()
{
    // If there is any data on the UART of the GNSS, read it and send every char to the library
    while (gps.gnssSerial->available() > 0)
    {
        // Is something is successfully decoded, display new data.
        if (gps.encode(gps.gnssSerial->read()))
        {
            // Check if the 500 milliseconds passed from the last data display.
            if ((unsigned long)(millis() - lastGnssDisplay) > 500UL)
            {
                // Capture new timestap.
                lastGnssDisplay = millis();

                // Display new data.
                displayInfo();
            }
        }
    }

    // No data in the first 5 seconds from the startup? Something is wrong... Check wires!
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        while (true)
        {
            // Delay is needed for the ESP8266.
            delay(10);
        }
    }
}

// Function shows decoded data from the GNSS library.
void displayInfo()
{
    // Print out GPS LAT & LON. If there is no valid data, show an error message.
    Serial.print(F("Location: "));
    if (gps.location.isValid())
    {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    // Print out time and date. If there is no valid data, show an error message.
    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid())
    {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    // Go to the new line, ready to print new data.
    Serial.println();
}