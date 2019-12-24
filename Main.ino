#include "globals.h"
HardwareSerial Serial2(PA3, PA2);

TinyGsm modem(Serial2);
TinyGsmClient client(modem);
PubSubClient Connection(client);
SIM_CONFIG simconfig;

TaskHandle_t CONTROLLER_TASK_HANDLE = NULL;
TaskHandle_t NET_TASK_HANDLE = NULL;
TaskHandle_t IO_TASK_HANDLE = NULL;

SemaphoreHandle_t xSerial;
SemaphoreHandle_t xIoQ;

OneWire ibutton(RFID);
String rf = "00 00 00 00 00 00 00";
String ai1 = "0.00";
String ai2 = "0.00";
String di1 = "0";
String di2 = "0";

void setup()
{
  SETUP_HARDWARE();
  LOAD_CONFIGURATION();
  delay(500);

  vLog("PROGRAME STARTING");

  //start uart semaphore
  xSerial = xSemaphoreCreateMutex();
  xIoQ = xSemaphoreCreateMutex();

  //Start the IDLE Task
  xTaskCreate(V2X_CONTROLLER::main, (const portCHAR *)"CONTROLLER_TASK_HANDLE", 512, NULL, 2, &CONTROLLER_TASK_HANDLE);

  vTaskStartScheduler();
  while(1);
}

void SETUP_HARDWARE()
{
  //SETUP UART
  Serial.begin(115200);
  Serial2.begin(115200);
  while(!Serial || !Serial2);
  //SETUP IOs
	pinMode(AI1, INPUT_ANALOG);
	pinMode(AI2, INPUT_ANALOG);
	pinMode(DI1, INPUT);
	pinMode(DI2, INPUT);
	pinMode(DO1, OUTPUT);
	pinMode(DO2, OUTPUT);
}

void LOAD_CONFIGURATION()
{
  if(!vDeserialize(&simconfig))
  {
    vLog("No Configuration Detected");
    vLog("Creating Factory Default Configuration");
    strcpy(simconfig.apn,"www.inwi.ma");
    strcpy(simconfig.broker,"41.140.242.182");
    simconfig.port = 1338;
    simconfig.interval = 5000;
    strcpy(simconfig.gprsPass,"");
    strcpy(simconfig.gprsUser,"");

    vLog("Saving Configuration");
    vSerialize(&simconfig);
  }
  else
  {
    vLog("Configuration Loaded");
  }
}

void loop(){}
