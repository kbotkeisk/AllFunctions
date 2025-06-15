/**************************************************************

   TinyGSM Getting Started guide:
     https://tiny.cc/tinygsm-readme

   NOTE:
   Some of the functions may be unavailable for your modem.
   Just comment them out.
   https://simcom.ee/documents/SIM7600C/SIM7500_SIM7600%20Series_AT%20Command%20Manual_V1.01.pdf
 **************************************************************/

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
#define USBBAUDRATE 115200

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS          true
#define TINY_GSM_TEST_TCP           true
#define TINY_GSM_TEST_TEMPERATURE   true
#define TINY_GSM_TEST_TIME          true
#define TINY_GSM_TEST_GPS           true

// Server details to test TCP/SSL
const char server[] = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";

#include <SPI.h>
#include <SD.h>
#include <Ticker.h>
#include "utilities.h"
#include "celler_unit.h"


void setup()
{
  // Set console baud rate
  SerialMon.begin(USBBAUDRATE);
  delay(10);

  SerialMon.println("Program Start");
  
  /*
    The indicator light of the board can be controlled
  */
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  celler_unit_power_on();
  celler_unit_init();
}

void light_sleep(uint32_t sec )
{
  esp_sleep_enable_timer_wakeup(sec * 1000000ULL);
  esp_light_sleep_start();
}

void loop()
{

#if TINY_GSM_TEST_TCP && defined TINY_GSM_MODEM_HAS_TCP
  TinyGsmClient client(modem, 0);
  const int port = 80;
  DBG("Connecting to ", server);
  if (!client.connect(server, port)) {
    DBG("... failed");
  } else {
    // Make a HTTP GET request:
    client.print(String("GET ") + resource + " HTTP/1.0\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.print("Connection: close\r\n\r\n");

    // Wait for data to arrive
    uint32_t start = millis();
    while (client.connected() && !client.available() &&
           millis() - start < 30000L) {
      delay(100);
    };

    // Read data
    start = millis();
    while (client.connected() && millis() - start < 5000L) {
      while (client.available()) {
        SerialMon.write(client.read());
        start = millis();
      }
    }
    client.stop();
  }
#endif

#if TINY_GSM_TEST_GPS && defined TINY_GSM_MODEM_HAS_GPS
  DBG("Enabling GPS/GNSS/GLONASS");
  modem.enableGPS();
  delay(2000);
  float lat2      = 0;
  float lon2      = 0;
  float speed2    = 0;
  float alt2      = 0;
  int   vsat2     = 0;
  int   usat2     = 0;
  float accuracy2 = 0;
  int   year2     = 0;
  int   month2    = 0;
  int   day2      = 0;
  int   hour2     = 0;
  int   min2      = 0;
  int   sec2      = 0;
  DBG("Requesting current GPS/GNSS/GLONASS location");
  int8_t i;
  for (i = 5; i; i--) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2,
                     &year2, &month2, &day2, &hour2, &min2, &sec2)) {
      DBG("Latitude:", String(lat2, 8), "\tLongitude:", String(lon2, 8));
      DBG("Speed:", speed2, "\tAltitude:", alt2);
      DBG("Visible Satellites:", vsat2, "\tUsed Satellites:", usat2);
      DBG("Accuracy:", accuracy2);
      DBG("Year:", year2, "\tMonth:", month2, "\tDay:", day2);
      DBG("Hour:", hour2, "\tMinute:", min2, "\tSecond:", sec2);
      break;
    } else {
      delay(1);
      // light_sleep(2);
    }
  }
  DBG("Retrieving GPS/GNSS/GLONASS location again as a string");
  String gps_raw = modem.getGPSraw();
  DBG("GPS/GNSS Based Location String:", gps_raw);
  DBG("Disabling GPS");
  modem.disableGPS();
  delay(5);
#endif

#if TINY_GSM_TEST_TIME && defined TINY_GSM_MODEM_HAS_TIME
  int year3 = 0;
  int month3 = 0;
  int day3 = 0;
  int hour3 = 0;
  int min3 = 0;
  int sec3 = 0;
  float timezone = 0;
  for (i = 5; i; i--) {
    DBG("Requesting current network time");
    if (modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3,
                             &timezone)) {
      DBG("Year:", year3, "\tMonth:", month3, "\tDay:", day3);
      DBG("Hour:", hour3, "\tMinute:", min3, "\tSecond:", sec3);
      DBG("Timezone:", timezone);
      break;
    } else {
      DBG("Couldn't get network time, retrying in 15s.");
      // light_sleep(15);
    }
  }
  DBG("Retrieving time again as a string");
  String time = modem.getGSMDateTime(DATE_FULL);
  DBG("Current Network Time:", time);
#endif

#if TINY_GSM_TEST_TEMPERATURE && defined TINY_GSM_MODEM_HAS_TEMPERATURE
  float temp = modem.getTemperature();
  DBG("Chip temperature:", temp);
#endif

  delay(5000);
}
