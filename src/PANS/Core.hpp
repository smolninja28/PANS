#pragma once
#include "../../include/main.h"

namespace PANS
{
  //core data
  enum class ReturnResult_e_t
  {
    Success,
    InvaildParameters,
    GenericFailure
  };

  namespace Core
  {
    //initialization
    ReturnResult_e_t Initialize();
  }
}
