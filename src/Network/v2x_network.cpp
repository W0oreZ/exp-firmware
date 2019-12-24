#include "src/Network/v2x_network.h"

char* DeviceID = "expV010_0001";
char* topic = "exp/v010/data";
String imei = "";

void V2X_NETWORK::main(void* p)
{
  if( xSerial != NULL )
  {
    if( xSemaphoreTake( xSerial, ( TickType_t ) 10 ) == pdTRUE )
    {
      vLog("PROGRAME RUNNING NET TASK");
      xSemaphoreGive( xSerial );
    }
  }
  uint32_t lastReconnectAttempt = 0;
  uint32_t lastTransmition = 0;
  
  V2X_NETWORK::BOOTSTRAP_MODEM();
  for(;;)
  {
    uint32_t time = millis();

    if (!Connection.connected()) 
    {
      if (time - lastReconnectAttempt > 5000) 
      {
        lastReconnectAttempt = time;
        if (V2X_NETWORK::StartConnection()) {
          lastReconnectAttempt = 0;
        }
      }
      vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
    else
    {
      if(time - lastTransmition > simconfig.interval)
      {
        String gpsData;
        byte buffer[1024];

        if(xIoQ != NULL)
        {
          if(xSemaphoreTake(xIoQ, 999) == pdTRUE)
          {
            gpsData = FORMATER::GPS_TO_JSON(&modem, imei);
            xSemaphoreGive(xIoQ);
          }
        }
        

        gpsData.getBytes(buffer,gpsData.length()+1);

        if(Connection.beginPublish(topic,gpsData.length()+1,false) == 1)
        {
          Connection.write(buffer,gpsData.length()+1);
          if(Connection.endPublish()==1)
          {
            lastTransmition = time;
          }
          else
          {
            if( xSerial != NULL )
            {
              if( xSemaphoreTake( xSerial, ( TickType_t ) 100 ) == pdTRUE )
              {
                vError("SEND FAIL");
                xSemaphoreGive( xSerial );
              }
            }
          }
        }
      }
    }
    
    Connection.loop();

    vTaskDelay(1);
  }
}

void V2X_NETWORK::BOOTSTRAP_MODEM()
{
  modem.init();
  imei = modem.getIMEI();
  modem.gprsConnect(simconfig.apn, simconfig.gprsUser, simconfig.gprsPass);

  if (!modem.isNetworkConnected()) 
  {
    while(!modem.waitForNetwork(5000)) 
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS );
    }
  }

  while(!modem.gprsConnect(simconfig.apn, simconfig.gprsUser, simconfig.gprsPass)) 
  {
    vTaskDelay(5000 / portTICK_PERIOD_MS );
  }

  modem.isGprsConnected();
  Connection.setServer(simconfig.broker, simconfig.port);
  Connection.setCallback(V2X_NETWORK::MessageHandler);
  modem.enableGPS();
}

void V2X_NETWORK::MessageHandler(char* topic, byte* payload, unsigned int len) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, len);
  Serial.println();
}

boolean V2X_NETWORK::StartConnection() 
{
  if (!Connection.connect(DeviceID)) {
    return false;
  }

  Connection.publish("etrack", "exp20 connected");
  Connection.subscribe("stm32/topic");

  return Connection.connected();
}


/*

    if( xSerial != NULL )
    {
      if( xSemaphoreTake( xSerial, ( TickType_t ) 100 ) == pdTRUE )
      {
        //here
        xSemaphoreGive( xSerial );
      }
    }


*/