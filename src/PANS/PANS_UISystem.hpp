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
    class Screen:public UINode
    {
    public:
      //child elements of the screen
      std::vector<Element> children;
    };
    class Element:public UINode
    {
    public:
      //position
      int xPos;
      int yPos;
    };
    class ConfigWindow:public Screen
    {
    public:
      //title for the config screen
      std::string title;
    };
    class Button:public Element
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
