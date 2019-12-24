#include "v2x_sensors.h"

byte address[8];

void V2X_SENSORS::setup()
{
	
}

void V2X_SENSORS::loop()
{
  if(xIoQ != NULL)
  {
    if(xSemaphoreTake(xIoQ, 200) == pdTRUE)
    {
      if(!V2X_SENSORS::rfidRead(&rf))
	    {
	    	rf = "00 00 00 00 00 00 00";
	    }
	    ai1 = String(vAnalogRead(AI1));
	    ai2 = String(vAnalogRead(AI2));
	    di1 = String(vAnalogRead(DI1));
	    di2 = String(vAnalogRead(DI2));

      if( xSerial != NULL )
      {
        if( xSemaphoreTake( xSerial, ( TickType_t ) 10 ) == pdTRUE )
        {
          vSensorLog("ANALOGIQUE","ai1",ai1);
          vSensorLog("ANALOGIQUE","ai2",ai2);
          vSensorLog("DIGITAL","di1",di1);
          vSensorLog("DIGITAL","di2",di2);
          vSensorLog("RFID","rfid",rf);
          xSemaphoreGive( xSerial );
        }
      }

      xSemaphoreGive(xIoQ);
    }
  }
  
	
  vTaskDelay(2000);
}

void V2X_SENSORS::main(void* p)
{
  //Serial.println("IO Task Started");
	if( xSerial != NULL )
  {
    if( xSemaphoreTake( xSerial, ( TickType_t ) 10 ) == pdTRUE )
    {
      vLog("PROGRAME RUNNING SENSORS TASK");
      xSemaphoreGive( xSerial );
    }
  }
  for(;;)
  {
    V2X_SENSORS::loop();
    vTaskDelay(1);
  }
}

bool V2X_SENSORS::rfidRead(String* rfid)
{
  if (!ibutton.search(address)){
		ibutton.reset_search();
		return false;
	}
  else
  {
    *rfid = "";
	  for (byte x = 7; x>0; x--){  
      if (address[x]<16) {
        *rfid += "0";
      }
      *rfid += String(address[x],HEX)+" ";
	  }
    ibutton.reset_search();
    
    return true;
  }
}