#include "PANS/Core.hpp"
#include "PANS/UISystem.hpp"
#include "PANS/Vision.hpp"

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
  master = Controller(ControllerId::master);
  PANS::Core::Initialize(master);
  PANS::UISystem::Initialize();
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
    pros::delay(200);
  }
}
