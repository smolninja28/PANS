#include "Core.hpp"

namespace PANS
{
  namespace Data
  {
    pros::Controller masterController(pros::E_CONTROLLER_MASTER);
    pros::Controller partnerController(pros::E_CONTROLLER_PARTNER);
    bool VisionSystem = false;
    bool UISystem = false;
  }

  namespace Core
  {
    //initialize PANS systems with no vision sensor
    ReturnResult Initialize()
    {
      //vision system cannot be used
      Data::VisionSystem = false;
      Data::UISystem = true;
      //init
      UISystem::Initialize();
      return ReturnResult::Success;
    }

    //initialize all PANS systems
    ReturnResult InitializeAll(int visionPort)
    {
      //all systems good
      Data::VisionSystem = true;
      Data::UISystem = true;
      //init
      UISystem::Initialize();
      VisionSystem::Initialize(visionPort);
      return ReturnResult::Success;
    }
  }
}
