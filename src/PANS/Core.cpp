#include "Core.hpp"

namespace PANS
{
  namespace Data
  {
    okapi::Controller masterController;
  }

  namespace Core
  {
    //initialization
    ReturnResult Initialize(okapi::Controller controllerMaster)
    {
      //set data
      PANS::Data::masterController = controllerMaster;

      return ReturnResult::Success;
    }
  }
}
