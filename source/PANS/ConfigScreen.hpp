#pragma once
#include "PANS.hpp"

namespace PANS
{
  //check if config screen module is active
  #ifdef CONFIG_SCREEN_MODULE
    namespace ConfigScreen
    {
      //initialization
      ReturnResult_e_t Initialize();
    }
  #endif
}
