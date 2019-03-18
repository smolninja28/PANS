//check if config screen module is active
#ifdef PANS_CONFIG_SCREEN_MODULE
#include "src/PANS/ConfigScreen.hpp"

namespace PANS
{
  namespace ConfigScreen
  {
    class Window
    {
    public:
      std::string title; //title for the window
      std::vector<Button> buttons; //the buttons for the window
    };
    class Button
    {
    public:
        std::string title; //title for the button
        //a callback function to execute if the button is pressed
        Window nextWindow; //the next window, to be rendered after the callback is executed
    };

    //initialization
    ReturnResult_e_t Initialize()
    {
      return ReturnResult_e_t::Success;
    }
  }
}
#endif
