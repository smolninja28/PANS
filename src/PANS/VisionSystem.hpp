#pragma once
#include "Core.hpp"
#include "UISystem.hpp"

namespace PANS
{
  namespace VisionSystem
  {
    //initialize vision systems.  VisionPort is the port the vision sensor is plugged into
    ReturnResult Initialize(int visionPort, Controller master);

    //visualize sensor detection on the brain screen.
    ReturnResult VisualizeSignature(int signature, int numObjects = 3);
  }
}
