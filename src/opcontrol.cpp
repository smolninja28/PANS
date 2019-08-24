#include "PANS/Core.hpp"
#include "PANS/UISystem.hpp"
#include "PANS/VisionSystem.hpp"

Controller master;

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
  master = Controller(ControllerId::master);
  //initialize all systems
  PANS::Core::Initialize(master);
  PANS::UISystem::Initialize();
  PANS::VisionSystem::Initialize(1);
  //test
  PANS::UISystem::MessageController(master, "Entering Opcontrol");
  PANS::UISystem::MessageBrain("Welcome to Opcontrol");
  while(true)
  {
    //post a test number
    if(master.getDigital(ControllerDigital::A))
    {
      PANS::UISystem::MessageBrain(std::to_string(foo));
      ++foo;
    }
    //open the test config
    if(master.getDigital(ControllerDigital::Y))
    {
      PANS::UISystem::ConfigDialog("How will we do this match?", "We will kick butt", &Option1, "We will crash and burn", &Option2);
    }
    //vision averaging test
    if(master.getDigital(ControllerDigital::up))
    {
      std::vector<int> sigs(1);
      sigs[0] = 1;
      PANS::VisionSystem::StartSigAveraging(sigs);
    }
    if(master.getDigital(ControllerDigital::down))
    {
      PANS::VisionSystem::StopSigAveraging();
    }
    //test visualization
    if(master.getDigital(ControllerDigital::right))
    {
      PANS::VisionSystem::VisualizeSignature(1);
    }
    if(master.getDigital(ControllerDigital::left))
    {
      PANS::VisionSystem::VisualizeAveragedSignature(1);
    }
    pros::delay(20);
  }
}
