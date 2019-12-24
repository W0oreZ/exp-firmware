#ifndef v2x_controller_h
#define v2x_controller_h
#include "globals.h"


namespace V2X_CONTROLLER
{
  //Controller Handlers
  void cmdHandler(String* msg);
  void networkTaskHandler();
  void ioTaskHandler();
  
  //TASK CORE FUNCTIONS
  void main(void* p);
}

#endif