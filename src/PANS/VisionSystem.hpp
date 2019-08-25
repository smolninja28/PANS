#pragma once
#include "Core.hpp"
#include "UISystem.hpp"
#include <deque>
#include <algorithm>

namespace PANS
{
  namespace VisionSystem
  {
    //initialize vision systems.  VisionPort is the port the vision sensor is plugged into
    ReturnResult Initialize(int visionPort);

    //visualize sensor detection on the brain screen.  signature is the sig to read.
    ReturnResult VisualizeSignature(int signature);

    //visualize sensor detection on the brain screen.  signature is the sig to read.
    ReturnResult VisualizeAveragedSignature(int signature);

    //start averaging vision signatures. Pass a vector of signature ids as ints
    ReturnResult StartSigAveraging(std::vector<int> sigs, int numObjects = 1, int samples = 10);

    //get an averaged vision object.  Object size, object signature
    pros::vision_object GetAveragedObject(int size, int sig);

    //stop averaging all vision signatures
    ReturnResult StopSigAveraging();
  }
}
