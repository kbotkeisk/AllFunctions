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

// Server details to test TCP/SSL
const char server[] = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";

#include "TinyGSM/src/TinyGsmClient.h"

typedef struct{
    TinyGsm* modem;
    unsigned char celler_status;
    String modem_name;
    String modemInfo;
    String sim_ccid;
    String imei;
    String imsi;
    String cop;
    IPAddress local_ip;
    int csq;
}modem_cellerdata;

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    float timezone;
}modem_ntpdata;

typedef struct{
    float lat;
    float lon;
    float speed;
    float alt;
    int   vsat;
    int   usat;
    float accuracy;
    int   year;
    int   month;
    int   day;
    int   hour;
    int   min;
    int   sec;
}modem_gnssdata;

typedef struct{
    modem_cellerdata cellerdata;
    modem_ntpdata ntpdata;
    modem_gnssdata gnssdata;
}st_modem;

extern st_modem modem_data;

void celler_unit_power_on();
int celler_unit_init();
modem_cellerdata modem_cellerdata_init(TinyGsm* modem, 
                    unsigned char celler_status, String modem_name, 
                    String modemInfo, String sim_ccid, String imei, String imsi, 
                    String cop, IPAddress local_ip, int csq);
void celler_variable_init();
void connect_server();
void getTemperature();
void getNetworkTime();
modem_ntpdata modem_ntpdata_init(int year, int month, int day, 
                    int hour, int min, int sec, float timezone);
void getGNSSposition();
modem_gnssdata modem_gnssdata_init(float lat, float lon, float speed, 
                    float alt, int vsat, int usat, float accuracy,
                    int year, int month, int day, int hour, int min, int sec);
#endif
