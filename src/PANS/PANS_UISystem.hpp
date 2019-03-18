#pragma once
#include "PANS.hpp"

//module define
#define PANS_MODULE_UISYSTEM

namespace PANS
{
  namespace UISystem
  {
    //classes
    class UINode
    {
    public:
      //Destroy this screen - this will invoke destroy on all children
      virtual void Destroy() const;
      //setup this screen - this will invoke setup on all children
      virtual void Setup() const;
    };
    class Screen : UINode
    {
    public:
      //child elements of the screen
      std::vector<Element> childElements;
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
    };
  }
}
