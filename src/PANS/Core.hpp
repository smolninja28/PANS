#pragma once
#include "../../include/main.h"

namespace PANS
{
  namespace Data
  {
    extern Controller masterController;
  }
  //core data
  enum class ReturnResult
  {
    Success,
    InvaildParameters,
    GenericFailure,
    UserAborted
  };

  namespace Core
  {
    //initialization
    ReturnResult Initialize(Controller controllerMaster);
  }
}
