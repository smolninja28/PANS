#pragma once
#include "../../include/main.h"

namespace PANS
{
  namespace Data
  {
    extern pros::Controller masterController;
    extern pros::Controller partnerController;
  }
  //core data
  enum class ReturnResult
  {
    Success,
    InvaildParameters,
    GenericFailure,
    UserAborted
  };
}
