#pragma once

namespace PANS
{
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
