#include "celler_unit.h"

st_modem modem_data;

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

// #ifdef DUMP_AT_COMMANDS
// #include <StreamDebugger.h>
// StreamDebugger debugger(SerialAT, SerialMon);
// TinyGsm modem(debugger);
// #else
// TinyGsm modem(SerialAT);
// #endif

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

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
    StreamDebugger debugger(SerialAT, SerialMon);
    // TinyGsm modem_data.cellerdata.modem(debugger);
    modem_data.cellerdata.modem = new TinyGsm(debugger);
#else
    // TinyGsm modem_data.cellerdata.modem(SerialAT);
    modem_data.cellerdata.modem = new TinyGsm(SerialAT);
#endif

    celler_variable_init();

    DBG("Initializing modem...");
    if (!modem_data.cellerdata.modem->init()) {
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
    ret = modem_data.cellerdata.modem->setNetworkMode(2);
    DBG("setNetworkMode:", ret);


    //https://github.com/vshymanskyy/TinyGSM/pull/405
    uint8_t mode = modem_data.cellerdata.modem->getGNSSMode();
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
    modem_data.cellerdata.modem->setGNSSMode(1, 1);
    DBG("setGNSSMode Done");
    // light_sleep(1);

    DBG("getModemName ...");
    String modem_name = modem_data.cellerdata.modem->getModemName();
    DBG("Modem Name:", modem_name);

    DBG("getModemInfo ...");
    String modemInfo = modem_data.cellerdata.modem->getModemInfo();
    DBG("Modem Info:", modemInfo);


    DBG("Waiting for network...");
    if (!modem_data.cellerdata.modem->waitForNetwork(600000L)) {
        // light_sleep(10);
        return 2;
    }

    unsigned char celler_status = 0;
    if (modem_data.cellerdata.modem->isNetworkConnected()) {
        DBG("Network connected");
        celler_status = 1;
    }

    DBG("Connecting to", apn);
    if (!modem_data.cellerdata.modem->gprsConnect(apn, gprsUser, gprsPass)) {
        // light_sleep(10);
        return 3;
    }

    res = modem_data.cellerdata.modem->isGprsConnected();
    DBG("GPRS status:", res ? "connected" : "not connected");

    String sim_ccid = modem_data.cellerdata.modem->getSimCCID();
    DBG("CCID:", sim_ccid);

    String imei = modem_data.cellerdata.modem->getIMEI();
    DBG("IMEI:", imei);

    String imsi = modem_data.cellerdata.modem->getIMSI();
    DBG("IMSI:", imsi);

    String cop = modem_data.cellerdata.modem->getOperator();
    DBG("Operator:", cop);

    IPAddress local_ip = modem_data.cellerdata.modem->localIP();
    DBG("Local IP:", local_ip);

    int csq = modem_data.cellerdata.modem->getSignalQuality();
    DBG("Signal quality:", csq);

    // GNSS 有効化
    DBG("Enabling GPS/GNSS/GLONASS");
    delay(100);
    modem_data.cellerdata.modem->enableGPS();

    modem_data.cellerdata = modem_cellerdata_init(modem_data.cellerdata.modem, 
                                                celler_status, modem_name, 
                                                modemInfo, sim_ccid, imei, imsi, cop, local_ip, csq);

    return 0;
}

void celler_variable_init(){
    modem_data.cellerdata = modem_cellerdata_init(modem_data.cellerdata.modem,
                                                0, "", "", "", "", "", "", IPAddress(0,0,0,0), 0);
    // unsigned char c = 0;
    // String modem_name = "";
    // String modemInfo = "";
    // String sim_ccid = "";
    // String imei = "";
    // String imsi = "";
    // String cop = "";
    // IPAddress local_ip;
    // int csq = 0;
    modem_data.ntpdata = modem_ntpdata_init(0, 0, 0, 0, 0, 0, 0);
    // int ntp_year = 0;
    // int ntp_month = 0;
    // int ntp_day = 0;
    // int ntp_hour = 0;
    // int ntp_min = 0;
    // int ntp_sec = 0;
    // float ntp_timezone = 0;
    modem_data.gnssdata = modem_gnssdata_init(0.0, 0.0, 0.0, 0.0, 0, 0, 0.0, 0, 0, 0, 0, 0, 0);
    // float lat2      = 0;
    // float lon2      = 0;
    // float speed2    = 0;
    // float alt2      = 0;
    // int   vsat2     = 0;
    // int   usat2     = 0;
    // float accuracy2 = 0;
    // int   year2     = 0;
    // int   month2    = 0;
    // int   day2      = 0;
    // int   hour2     = 0;
    // int   min2      = 0;
    // int   sec2      = 0;
}


// modem_cellerdata一括代入用の関数
modem_cellerdata modem_cellerdata_init(TinyGsm* modem, 
                    unsigned char c, String modem_name, 
                    String modemInfo, String sim_ccid, String imei, String imsi, 
                    String cop, IPAddress local_ip, int csq){
    modem_cellerdata cellerdata;
    cellerdata.modem = modem;
    cellerdata.modem_name = modem_name;
    cellerdata.modemInfo = modemInfo;
    cellerdata.sim_ccid = sim_ccid;
    cellerdata.imei = imei;
    cellerdata.cop = cop;
    cellerdata.local_ip = local_ip;
    cellerdata.csq = csq;
    return cellerdata;
}

void connect_server(){
    TinyGsmClient client(*modem_data.cellerdata.modem, 0);
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
}

void getTemperature(){
    float temp = modem_data.cellerdata.modem->getTemperature();
    DBG("Chip temperature:", temp);
}

void getNetworkTime(){
    int year3 = 0;
    int month3 = 0;
    int day3 = 0;
    int hour3 = 0;
    int min3 = 0;
    int sec3 = 0;
    float timezone = 0;
    int i = 0;
    for (i = 5; i; i--) {
        DBG("Requesting current network time");
        if (modem_data.cellerdata.modem->getNetworkTime(&year3, &month3, &day3, 
                                                        &hour3, &min3, &sec3,&timezone)) {
            DBG("Year:", year3, "\tMonth:", month3, "\tDay:", day3);
            DBG("Hour:", hour3, "\tMinute:", min3, "\tSecond:", sec3);
            DBG("Timezone:", timezone);
            modem_data.ntpdata = modem_ntpdata_init(year3, month3, day3, hour3, min3, sec3, timezone);
            break;
        } else {
            delay(10);
            DBG("Couldn't get network time, retrying in 15s.");
        // light_sleep(15);
        }
    }
    DBG("Retrieving time again as a string");
    String time = modem_data.cellerdata.modem->getGSMDateTime(DATE_FULL);
    DBG("Current Network Time:", time);
}

// modem_ntpdata一括代入用の関数
modem_ntpdata modem_ntpdata_init(int year, int month, int day, 
                    int hour, int min, int sec, float timezone){
    modem_ntpdata ntpdata;
    ntpdata.year = year;
    ntpdata.month = month;
    ntpdata.day = day;
    ntpdata.hour = hour;
    ntpdata.min = min;
    ntpdata.sec = sec;
    ntpdata.timezone = timezone;
    return ntpdata;
}

void getGNSSposition(){
    int i = 0;
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
    for (i = 600; i; i--) {
        if (modem_data.cellerdata.modem->getGPS(&lat2, &lon2, &speed2, &alt2, 
                                                &vsat2, &usat2, &accuracy2,
                                                &year2, &month2, &day2, 
                                                &hour2, &min2, &sec2)) {
            DBG("Latitude:", String(lat2, 8), "\tLongitude:", String(lon2, 8));
            DBG("Speed:", speed2, "\tAltitude:", alt2);
            DBG("Visible Satellites:", vsat2, "\tUsed Satellites:", usat2);
            DBG("Accuracy:", accuracy2);
            DBG("Year:", year2, "\tMonth:", month2, "\tDay:", day2);
            DBG("Hour:", hour2, "\tMinute:", min2, "\tSecond:", sec2);
            modem_data.gnssdata = modem_gnssdata_init(lat2, lon2, speed2, alt2, vsat2, usat2, accuracy2,
                                                    year2, month2, day2, hour2, min2, sec2);
            break;
        } else {
            delay(10);
            // light_sleep(2);
        }
    }
    DBG("Retrieving GPS/GNSS/GLONASS location again as a string");
    String gps_raw = modem_data.cellerdata.modem->getGPSraw();
    DBG("GPS/GNSS Based Location String:", gps_raw);
    // DBG("Disabling GPS");
    // modem.disableGPS();
    // delay(5);
}

// modem_gnssdata一括代入用の関数
modem_gnssdata modem_gnssdata_init(float lat, float lon, float speed, 
                    float alt, int vsat, int usat, float accuracy,
                    int year, int month, int day, int hour, int min, int sec) {
    modem_gnssdata gnssdata;
    gnssdata.lat = lat;
    gnssdata.lon = lon;
    gnssdata.speed = speed;
    gnssdata.alt = alt;
    gnssdata.vsat = vsat;
    gnssdata.usat = usat;
    gnssdata.accuracy = accuracy;
    gnssdata.year = year;
    gnssdata.month = month;
    gnssdata.day = day;
    gnssdata.hour = hour;
    gnssdata.min = min;
    gnssdata.sec = sec;
    return gnssdata;
}