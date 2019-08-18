#pragma once
#include "Core.hpp"

namespace PANS
{
  namespace UISystem
  {
    //get ready
    ReturnResult Initialize();
    //make a config dialog on the brain screen. Title is the title for the screen, text is inside the button, calbacks execute upon selection
    ReturnResult ConfigDialog(std::string title, std::string option0, void (*callback0)(), std::string option1, void (*callback1)());
    //post a message to the controller.  Text must be 16 characters or less
    ReturnResult MessageController(Controller controller, std::string text);
    //appends a message to the brain
    ReturnResult MessageBrain(std::string text);
    //clears the screen of the brain
    ReturnResult ClearBrain();
  }
}
