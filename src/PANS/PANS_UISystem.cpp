//check if ui system module is active
#ifdef PANS_MODULE_UISYSTEM
#include "src/PANS/PANS_UISystem.hpp"

namespace PANS
{
  namespace UISystem
  {
    //classes
    class UINode
    {
    public:
      //Destroy this screen - this will invoke destroy on all children
      virtual void Destroy()
      {

      }
      //setup this screen - this will invoke setup on all children
      virtual void Setup()
      {

      }
    };
    class Screen : UINode
    {
    public:
      //child elements of the screen
      std::vector<Element> children;
      //destroy this screen
      void Destroy()
      {

      }
      //setup this screen
      void Setup()
      {

      }
    };
    class Element : UINode
    {
    public:
      //position
      int xPos;
      int yPos;
    };
    class ConfigWindow : Screen
    {
    public:
      //title for the config screen
      std::string title;
    };
    class Button : Element
    {
    public:
      //title for the button
      std::string title;
      //callback to execute when button is pressed
      void (*function);
    };
    class UIManager
    {
    public:
      //the current screen being displayed
      Screen currentScreen;
    };
  }
}
#endif
