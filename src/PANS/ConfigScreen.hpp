#pragma once
#include "PANS.hpp"

//check if config screen module is active
#ifdef PANS_CONFIG_SCREEN_MODULE
namespace PANS
{
  namespace ConfigScreen
  {
    //initialization
    ReturnResult_e_t Initialize();
  }
}
#endif
