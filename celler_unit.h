#ifndef CELLER_UNIT_H
#define CELLER_UNIT_H

#define TINY_GSM_MODEM_SIM7600

#define SerialMon Serial

#define MODEM_RX                    4
#define MODEM_TX                    6
#define MODEM_DTR                   5
#define MODEM_RI                    1
#define MODEM_PWRKEY                3

#define GPS_RX_PIN                      39
#define GPS_TX_PIN                      42

#define SerialAT Serial1
#define UART_BAUD_AT 115200

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Your GPRS credentials, if any
const char apn[] = "ppsim.jp";
// const char apn[] = "ibasis.iot";
const char gprsUser[] = "";
const char gprsPass[] = "";

#include "TinyGSM/src/TinyGsmClient.h"

extern TinyGsm modem;
extern unsigned char seller_status;
extern String modem_name;
extern String modemInfo;
extern String sim_ccid;
extern String imei;
extern String imsi;
extern String cop;
extern IPAddress local_ip;
extern int csq;

void celler_unit_power_on();
int celler_unit_init();

#endif
