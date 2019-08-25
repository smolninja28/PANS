#pragma once
#include "Core.hpp"
#include <vector>
#include <deque>

namespace PANS
{
  namespace UISystem
  {
    //get ready
    ReturnResult Initialize();
    //make a config dialog on the brain screen. Title is the title for the screen, text is inside the button, calbacks execute upon selection
    ReturnResult ConfigDialog(std::string title, std::string option0, void (*callback0)(), std::string option1, void (*callback1)());
    //post a message to the controller.  Text must be 16 characters or less.  If master is false, partner controller is used
    ReturnResult MessageController(std::string text, bool master = true);
    //appends a message to the brain
    ReturnResult MessageBrain(std::string text);
    //clears the screen of the brain
    ReturnResult ClearBrain();
    //prepares the ui system for vision object rendering
    ReturnResult PrepareForRendering();
    //asks the ui system to render an object
    ReturnResult RenderObject(int width, int height, int x, int y);
    //asks the ui system to clear the screen of all rendered objects
    ReturnResult ClearRendering();
    //shuts down object rendering
    ReturnResult StopRendering();
    //prints the robot's status to the brain and the master controller
    ReturnResult PrintRobotStatus();
  }
}
