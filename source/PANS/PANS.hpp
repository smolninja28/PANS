#pragma once

namespace PANS
{
  //FUNCTIONALITY DEFINES//
  #define CONFIG_SCREEN_MODULE //pre-game initialization robot configuration screen module

  //core data
  enum class ReturnResult_e_t
  {
    Success,
    GenericFailure
  };

  namespace Core
  {
    //initialization
    ReturnResult_e_t Initialize();
  }
}
