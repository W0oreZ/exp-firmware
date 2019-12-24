#ifndef v2x_sensors_h
#define v2x_sensors_h
#include "globals.h"

namespace V2X_SENSORS
{
    bool rfidRead(String* rfid);

    //core functions
    void main(void* p);
    void setup();
    void loop();
}

#endif