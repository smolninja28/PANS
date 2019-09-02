#ifndef PANS_CORE
#define PANS_CORE
#include "api.h"
#include <vector>
#include <deque>
#include <algorithm>

namespace PANS
{
  namespace Data
  {
    extern pros::Controller masterController;
    extern pros::Controller partnerController;
    extern bool VisionSystem;
    extern bool UISystem;
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
    //initialize PANS systems with no vision sensor
    ReturnResult Initialize();

    //initialize all PANS systems
    ReturnResult InitializeAll(int visionPort);
  }
}
//include systems
#include "UISystem.hpp"
#include "VisionSystem.hpp" //VisionSystem has dependency on UISystem so include it after
#endif
