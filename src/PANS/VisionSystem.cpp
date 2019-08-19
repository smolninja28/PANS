#include "VisionSystem.hpp"

namespace PANS
{
  namespace VisionSystem
  {
    //general variables
    int sensorPort = 0;
    Controller masterController;
    //vision FOV to brain screen scale factors
    int brainWidth = 480; //width of the brain screen
    int brainHeight = 240; //height of the brain screen
    float xScale = 1.519; //(brain x)/(vision x) = 480/316 = 1.519
    float yScale = 1.132; //(brain y)/(vision y) = 240/212 = 1.132

    //initialize vision systems.  VisionPort is the port the vision sensor is plugged into
    ReturnResult Initialize(int visionPort, Controller master)
    {
      sensorPort = visionPort; //store the port
      masterController = master; //store the master controller
      return ReturnResult::Success;
    }

    //visualize sensor detection on the brain screen.
    ReturnResult VisualizeSignature(int signature, int numObjects)
    {
      //set the controller text
      PANS::UISystem::MessageController(masterController, "Visual|Done:X");
      //clear the brain of messages
      PANS::UISystem::ClearBrain();
      //create a vision sensor reference
      pros::Vision sensor(sensorPort);
      //prepare the ui system for rendering
      PANS::UISystem::PrepareForRendering();
      while(true) //control loop
      {
        //clear the render screen of objects
        PANS::UISystem::ClearRendering();
        if(masterController.getDigital(ControllerDigital::X)) //check for abort
        {
          PANS::UISystem::MessageController(masterController, ""); //clear the controller
          //shutdown the rendering ui system
          PANS::UISystem::StopRendering();
          return ReturnResult::Success; //begone
        }
        //loop over how many objects need to be rendered
        for(int i = 0; i < numObjects; ++i)
        {
          //read the vision data in
          pros::vision_object rtn = sensor.get_by_sig(i, signature);
          //tell the ui system to render the object
          PANS::UISystem::RenderObject(rtn.width * xScale, rtn.height * yScale, rtn.left_coord * xScale, rtn.top_coord * yScale);
        }
        pros::delay(50); //wait
      }
    }
  }
}
