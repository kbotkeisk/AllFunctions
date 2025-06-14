
#pragma once

// Product Link : https://www.lilygo.cc/products/t-internet-poe
// #define LILYGO_T_INTERNET_POE

// Product Link : https://www.lilygo.cc/products/t-poe-pro
// #define LILYGO_T_ETH_POE_PRO

// Product Link : https://www.lilygo.cc/products/t-internet-com
// #define LILYGO_T_INTER_COM

// Product Link : https://www.lilygo.cc/products/t-eth-lite?variant=43120880746677
// #define LILYGO_T_ETH_LITE_ESP32

// Product Link : https://www.lilygo.cc/products/t-eth-lite?variant=43120880779445
// #define LILYGO_T_ETH_LITE_ESP32S3
// Product Link : N.A
#define LILYGO_T_ETH_ELITE_ESP32S3

#if   defined(LILYGO_T_INTERNET_POE)
#define uS_TO_S_FACTOR      1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP       30          /* Time ESP32 will go to sleep (in seconds) */

#define UART_BAUD           115200

#define MODEM_TX            27
#define MODEM_RX            26
#define MODEM_PWRKEY        4
#define MODEM_DTR           32
#define MODEM_RI            33
#define MODEM_FLIGHT        25
#define MODEM_STATUS        34

#define SD_MISO             2
#define SD_MOSI             15
#define SD_SCLK             14
#define SD_CS               13

#define LED_PIN             12

#elif defined(LILYGO_T_ETH_ELITE_ESP32S3)

#define uS_TO_S_FACTOR      1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP       30          /* Time ESP32 will go to sleep (in seconds) */

#define UART_BAUD           115200

#define ETH_MISO_PIN                     47
#define ETH_MOSI_PIN                     21
#define ETH_SCLK_PIN                     48
#define ETH_CS_PIN                       45
#define ETH_INT_PIN                      14
#define ETH_RST_PIN                      -1
#define ETH_ADDR                         1

#define SPI_MISO_PIN                     9
#define SPI_MOSI_PIN                     11
#define SPI_SCLK_PIN                     10

#define SD_MISO                     SPI_MISO_PIN
#define SD_MOSI                     SPI_MOSI_PIN
#define SD_SCLK                     SPI_SCLK_PIN
#define SD_CS                       12

#define I2C_SDA_PIN                     17
#define I2C_SCL_PIN                     18

#define RADIO_MISO_PIN                  SPI_MISO_PIN
#define RADIO_MOSI_PIN                  SPI_MOSI_PIN
#define RADIO_SCLK_PIN                  SPI_SCLK_PIN
#define RADIO_CS_PIN                    40
#define RADIO_RST_PIN                   46
// #define RADIO_DIO1_PIN                  16
#define RADIO_IRQ_PIN                   8
#define RADIO_BUSY_PIN                  16

#define ADC_BUTTONS_PIN                 7

#define MODEM_RX                    4
#define MODEM_TX                    6
#define MODEM_DTR                   5
#define MODEM_RI                    1
#define MODEM_PWRKEY                3

#define GPS_RX_PIN                      39
#define GPS_TX_PIN                      42

#define LED_PIN                         38

#else
#error "Use ArduinoIDE, please open the macro definition corresponding to the board above <utilities.h>"
#endif