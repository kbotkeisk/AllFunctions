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
#define TINY_GSM_TEST_TCP           false
#define TINY_GSM_TEST_TEMPERATURE   true
#define TINY_GSM_TEST_TIME          true
#define TINY_GSM_TEST_GPS           true


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

// #if TINY_GSM_TEST_TCP && defined TINY_GSM_MODEM_HAS_TCP
//   connect_server();
// #endif

// #if TINY_GSM_TEST_GPS && defined TINY_GSM_MODEM_HAS_GPS
//   getGNSSposition();
// #endif

getNetworkTime();

// #if TINY_GSM_TEST_TEMPERATURE && defined TINY_GSM_MODEM_HAS_TEMPERATURE
//   getTemperature();
// #endif

  delay(5000);
}
