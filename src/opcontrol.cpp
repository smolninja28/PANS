#include "main.h"
#include "PANS/Core.hpp"
#include "PANS/UISystem.hpp"
#include "PANS/VisionSystem.hpp"
#include "../include/okapi/api.hpp"
using namespace okapi;

okapi::Controller master;

//testing functions
void Option1()
{
  PANS::UISystem::MessageBrain("Option0 was selected");
}
void Option2()
{
  PANS::UISystem::MessageBrain("Option1 was selected");
}
int foo = 0; //random number for testing
void opcontrol()
{
  //set the controller
  master = okapi::Controller(okapi::ControllerId::master);
  //initialize all systems
  PANS::UISystem::Initialize();
  PANS::VisionSystem::Initialize(1);
  //test
  PANS::UISystem::PrintRobotStatus();
  while(true)
  {
    //post a test number
    if(master.getDigital(okapi::ControllerDigital::A))
    {
      PANS::UISystem::MessageBrain(std::to_string(foo));
      ++foo;
    }
    //open the test config
    if(master.getDigital(okapi::ControllerDigital::Y))
    {
      PANS::UISystem::ConfigDialog("How will we do this match?", "We will kick butt", &Option1, "We will crash and burn", &Option2);
    }
    //vision averaging test
    if(master.getDigital(okapi::ControllerDigital::up))
    {
      std::vector<int> sigs(1);
      sigs[0] = 1;
      PANS::VisionSystem::StartSigAveraging(sigs);
    }
    if(master.getDigital(okapi::ControllerDigital::down))
    {
      PANS::VisionSystem::StopSigAveraging();
    }
    //test visualization
    if(master.getDigital(okapi::ControllerDigital::right))
    {
      PANS::VisionSystem::VisualizeSignature(1);
    }
    if(master.getDigital(okapi::ControllerDigital::left))
    {
      PANS::VisionSystem::VisualizeAveragedSignature(1);
    }
    pros::delay(20);
  }
}
