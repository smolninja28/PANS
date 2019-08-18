#include "Core.hpp"

namespace PANS
{
  namespace Data
  {
    Controller masterController;
  }

  namespace Core
  {
    //initialization
    ReturnResult Initialize(Controller controllerMaster)
    {
      //set data
      PANS::Data::masterController = controllerMaster;

      return ReturnResult::Success;
    }
  }
}
