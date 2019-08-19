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
  PANS::VisionSystem::Initialize(1, master);
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
    if(master.getDigital(ControllerDigital::up))
    {
      PANS::UISystem::ConfigDialog("How will we do this match?", "We will kick butt", &Option1, "We will crash and burn", &Option2);
    }
    //test rendering
    if(master.getDigital(ControllerDigital::down))
    {
      PANS::UISystem::PrepareForRendering();
      PANS::UISystem::RenderObject(480, 240, 0, 0);
    }
    if(master.getDigital(ControllerDigital::left))
    {
      PANS::UISystem::StopRendering();
    }
    //test visualization
    if(master.getDigital(ControllerDigital::B))
    {
      PANS::VisionSystem::VisualizeSignature(1, 3);
    }
    pros::delay(20);
  }
}
