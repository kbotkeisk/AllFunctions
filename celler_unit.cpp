#include "celler_unit.h"

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

unsigned char seller_status = 0;
String modem_name = "";
String modemInfo = "";
String sim_ccid = "";
String imei = "";
String imsi = "";
String cop = "";
IPAddress local_ip;
int csq = 0;

void celler_unit_power_on(){
    // Set GSM module baud rate
    SerialAT.begin(UART_BAUD_AT, SERIAL_8N1, MODEM_RX, MODEM_TX);
    /*
    MODEM_PWRKEY IO:4 The power-on signal of the modulator must be given to it,
    otherwise the modulator will not reply when the command is sent
    */
    pinMode(MODEM_PWRKEY, OUTPUT);
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(300); //Need delay
    digitalWrite(MODEM_PWRKEY, LOW);

    DBG("MODEM_START");
}

int celler_unit_init(){
    DBG("Initializing modem...");
    if (!modem.init()) {
        DBG("Failed to restart modem, delaying 10s and retrying");
        return 1;
    }

    bool res;

    /*  Preferred mode selection : AT+CNMP
        2 – Automatic
        13 – GSM Only
        14 – WCDMA Only
        38 – LTE Only
        59 – TDS-CDMA Only
        9 – CDMA Only
        10 – EVDO Only
        19 – GSM+WCDMA Only
        22 – CDMA+EVDO Only
        48 – Any but LTE
        60 – GSM+TDSCDMA Only
        63 – GSM+WCDMA+TDSCDMA Only
        67 – CDMA+EVDO+GSM+WCDMA+TDSCDMA Only
        39 – GSM+WCDMA+LTE Only
        51 – GSM+LTE Only
        54 – WCDMA+LTE Only
    */
    String ret;
    //  do {
    //    ret = modem.setNetworkMode(2);
    //    delay(500);
    //  } while (ret != "OK");
    ret = modem.setNetworkMode(2);
    DBG("setNetworkMode:", ret);


    //https://github.com/vshymanskyy/TinyGSM/pull/405
    uint8_t mode = modem.getGNSSMode();
    DBG("GNSS Mode:", mode);

    /**
        CGNSSMODE: <gnss_mode>,<dpo_mode>
        This command is used to configure GPS, GLONASS, BEIDOU and QZSS support mode.
        gnss_mode:
            0 : GLONASS
            1 : BEIDOU
            2 : GALILEO
            3 : QZSS
        dpo_mode :
            0 disable
            1 enable
    */
    modem.setGNSSMode(1, 1);
    DBG("setGNSSMode Done");
    // light_sleep(1);

    DBG("getModemName ...");
    modem_name = modem.getModemName();
    DBG("Modem Name:", modem_name);

    DBG("getModemInfo ...");
    modemInfo = modem.getModemInfo();
    DBG("Modem Info:", modemInfo);


    DBG("Waiting for network...");
    if (!modem.waitForNetwork(600000L)) {
        // light_sleep(10);
        return 2;
    }

    if (modem.isNetworkConnected()) {
        DBG("Network connected");
        seller_status = 1;
    }

    DBG("Connecting to", apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        // light_sleep(10);
        return 3;
    }

    res = modem.isGprsConnected();
    DBG("GPRS status:", res ? "connected" : "not connected");

    sim_ccid = modem.getSimCCID();
    DBG("CCID:", sim_ccid);

    imei = modem.getIMEI();
    DBG("IMEI:", imei);

    imsi = modem.getIMSI();
    DBG("IMSI:", imsi);

    cop = modem.getOperator();
    DBG("Operator:", cop);

    local_ip = modem.localIP();
    DBG("Local IP:", local_ip);

    csq = modem.getSignalQuality();
    DBG("Signal quality:", csq);

    return 0;
}
