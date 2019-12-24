#ifndef FORMATER_h
#define FORMATER_h

#include "globals.h"
#include "src/vendor/TinyGSM/src/TinyGsmClient.h"

namespace FORMATER
{
  bool vGPS(TinyGsm* modem,String* lat, String *lon, String *speed, String *alt, String *vsat, String *usat);
  String RAWGPS_TO_JSON(TinyGsm* modem, String imei, bool debug = false);
  String GPS_TO_JSON(TinyGsm* modem, String imei, bool debug = false);
}

#endif