#pragma once
#include "PANS.hpp"

//module define
#define PANS_CONFIG_SCREEN_MODULE

namespace PANS
{
  namespace ConfigScreen
  {
    //classes
    class Node;
    class Child;
    class Window;
    class Button;

    //the end node window
    Window endNode;

    //initialization
    ReturnResult_e_t Initialize();
  }
}
