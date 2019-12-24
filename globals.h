#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include "STM32FreeRTOSConfig.h"
#define TINY_GSM_MODEM_SIM868     //SIM module Version
#define GSM_AUTOBAUD_MIN 115200   //BAUDRATE 
#define GSM_AUTOBAUD_MAX 115200   //BAUDRATE
#include "src/vendor/TinyGSM/src/TinyGsmClient.h"
#include "src/vendor/PubSubClient/src/PubSubClient.h"
#include <EEPROM.h>
#include "src/vendor/OneWire/OneWire.h"


extern HardwareSerial Serial2;
extern HardwareSerial Serial3;

extern TaskHandle_t CONTROLLER_TASK_HANDLE;
extern TaskHandle_t NET_TASK_HANDLE;
extern TaskHandle_t IO_TASK_HANDLE;

extern SemaphoreHandle_t xSerial;
extern SemaphoreHandle_t xIoQ;

extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient Connection;

#define AI1 PA4         //Analog input 1
#define AI2 PA5         //Analog input 2
#define DI1 PB12        //Digital input 1
#define DI2 PB13        //Digital input 2
#define DO1 PB14        //Digital output 1
#define DO2 PB15        //Digital output 2
#define WIR PA8         //RFID
#define FTDI PB6        //PC Connection Digital Detector
#define LED_NET PB3     //Led indicator
#define RFID PA8        //RFID reader
#define SIMPIN PA12     //SIMon/off pin

typedef struct SIM_CONFIG
{ 
  char apn[25];
  char gprsUser[15];
  char gprsPass[15];
  char broker[25];
  int port;
  int interval;
} SIM_CONFIG;

extern SIM_CONFIG simconfig;

extern OneWire ibutton;
extern String  rf;
extern String  ai1;
extern String  ai2;
extern String  di1;
extern String  di2;

#include "src/vendor/Serialization/Serialization.h"
#include "src/utils/formater/formater.h"
#include "src/utils/pclink/pclink.h"
#include "src/utils/io/io.h"


#include "src/Controller/v2x_controller.h"
#include "src/Network/v2x_network.h"
#include "src/Sensors/v2x_sensors.h"


//#include "src/Sensors/v2x_sensors.h"

#endif
