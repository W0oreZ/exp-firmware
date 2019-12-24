#include "v2x_controller.h"

void V2X_CONTROLLER::cmdHandler(String* msg)
{
  if(msg->indexOf("ee_length")>=0)
    vLog("EEPROM SIZE : "+ String(EEPROM.length()));
  else if (msg->indexOf("ee_show")>=0)
  {
    vLog("V2X EEPROM");
    for (int i = 0; i < EEPROM.length(); i++)
    {
      Serial.print(EEPROM[i]);
    }
    Serial.println();
  }
  else if (msg->indexOf("load_cfg")>=0)
  {
    vSendConfig(&simconfig);
  }
  
  if (msg->indexOf("infos")>=0)
  {
    vInfo("version","exp-0.2.0a");
    vInfo("netId","exTracker101");
  }
  else if(msg->indexOf("conf#")>=0)
  {
    vSetConfig(&simconfig, msg->substring(5));
    vLog("Saving Configuration");
    vSerialize(&simconfig);
    vSendConfig(&simconfig);
  }
  else
  {
    if(*msg != "")
    {
      vLog("[PC - Unhandled] > "+*msg);
    }
  }
}

void V2X_CONTROLLER::networkTaskHandler()
{
  if(NET_TASK_HANDLE == NULL)
  {
    xTaskCreate(V2X_NETWORK::main, (const portCHAR *)"NET_TASK_HANDLE", 1024, NULL, 2, &NET_TASK_HANDLE);
    vTaskDelay(100);
  }
}

void V2X_CONTROLLER::ioTaskHandler()
{
  if(IO_TASK_HANDLE == NULL)
  {
    xTaskCreate(V2X_SENSORS::main, (const portCHAR *)"IO_TASK_HANDLE", 128, NULL, 2, &IO_TASK_HANDLE);
    vTaskDelay(100);
  }
}

void V2X_CONTROLLER::main(void* p)
{
  String msg = "";
  vLog("Controller is Running");
  
  
  
  xTaskCreate(V2X_SENSORS::main, (const portCHAR *)"IO_TASK_HANDLE", 256, NULL, 2, &IO_TASK_HANDLE);
  xTaskCreate(V2X_NETWORK::main, (const portCHAR *)"NET_TASK_HANDLE", 1024, NULL, 2, &NET_TASK_HANDLE);
  //
  //V2X_CONTROLLER::ioTaskHandler();
  //V2X_CONTROLLER::networkTaskHandler();

  for(;;)
  {
    if( xSemaphoreTake( xSerial, ( TickType_t ) 10 ) == pdTRUE )
    {
      if(Serial.available()>0)
      {
        msg = Serial.readString();
      }
      V2X_CONTROLLER::cmdHandler(&msg);
      
      xSemaphoreGive( xSerial );
    }

    msg = "";
    vTaskDelay(1);
  }
}

