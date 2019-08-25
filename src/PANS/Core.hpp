#pragma once
#include "../../include/main.h"
#include "../../include/okapi/api.hpp"

namespace PANS
{
  namespace Data
  {
    extern okapi::Controller masterController;
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
    ReturnResult Initialize(okapi::Controller controllerMaster);
  }
}
