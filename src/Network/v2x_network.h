#ifndef v2x_network_h
#define v2x_network_h
#include "globals.h"

namespace V2X_NETWORK
{
  void main(void* p);
  void BOOTSTRAP_MODEM();
  boolean StartConnection();
  void MessageHandler(char* topic, byte* payload, unsigned int len);
}

#endif