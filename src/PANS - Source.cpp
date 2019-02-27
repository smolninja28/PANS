#include "../include/controllers.hpp"

//controllers
Controller master = Controller(ControllerId::master); //the drive controller
Controller partner = Controller(ControllerId::partner); //the flywheel controller

namespace Robot
{
  Team_e_t team = Team_e_t::Red;
  StartingTile_e_t startingTile = StartingTile_e_t::Close;
  uint32_t flagSig = 0;
  bool IsCompetition = false;
  bool IsParking = false;
  int gameTime = 0; //the time since game start in miliseconds

  void Timer()
  {
    //increase game time by 20 miliseconds
    gameTime += 20;

    //check if in competition mode
    if(IsCompetition)
    {
      //check for end of game rumble.  Past 15 seconds, every second
      if((gameTime > 90000) && ((gameTime % 1000) == 0))
      {
        //rumble both controllers
        master.rumble(".");
        partner.rumble(".");
      }
    }
  }
}

namespace Motors
{
  //intake
  Motor intake = Motor(Ports::Intake, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
  //flipper
  Motor flipper = Motor(Ports::Flipper, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
}

namespace Chassis
{
  //closed and open loop control for chassis
  ChassisControllerIntegrated controller = ChassisControllerFactory::create({Ports::DriveLeftFront, Ports::DriveLeftBack}, {-Ports::DriveRightFront, -Ports::DriveRightBack}, AbstractMotor::gearset::green, {4_in, 15.4_in});
}

namespace Intake
{
  ADIButton limit(Ports::IntakeSwitch);
  bool IsPressed = false;
  bool IsRunning = false;
  bool IsBackwards = false;
  bool IsBall = false;

  void Controller()
  {
    //check if the intake should be running
    if(IsRunning)
    {
      //ball has exited
      IsBall = false;
      //turn on motor
      //check direction
      if(IsBackwards)
      {
        Motors::intake.move(-127);
      }
      else
      {
        Motors::intake.move(127);
      }
      //check if the limit switch has been hit
      if(limit.changedToPressed())
      {
        //stop the intake
        IsRunning = false;
        //ball is loaded
        IsBall = true;
        //rumble the flywheel controller
        partner.rumble(".");
      }
    }
    else
    {
      //turn off motor
      Motors::intake.move(0);
    }
  }
}

namespace Flywheel
{
  //velocity PID variables
  AsyncVelIntegratedController velController = AsyncControllerFactory::velIntegrated(Ports::Flywheel);
  //velocity management variables
  int _speed = 0;
  Mode mode = Variable;
  int speed = 0;
  bool highSpeed = false;

  void Controller()
  {
    switch(mode)
    {
      case Stopped:
        //check if the motors are stopped
        if(_speed > 20)
        {
          //lower the velocity by 1
          _speed -= 1;
        }
        else
        {
          //set the velocity to 0
          _speed = 0;
        }
        break;
      case Variable:
        //upper bounds checking
        if(speed > MOTOR_GEARSET_06_MAXSPEED)
        {
          //set to limit
          speed = MOTOR_GEARSET_06_MAXSPEED;
        }
        //lower bounds checking
        if(speed < 0)
        {
          //set to limit
          speed = 0;
        }
        //set speed to variableSpeed
        _speed = speed;
        break;
    }
    //set the motors
    velController.setTarget(_speed);
    //Motors::flywheel->moveVelocity(flywheelSpeed);
  }
}

namespace Flipper
{
  ADIButton flipperSwitch(Ports::FlipperSwitch);
  bool IsFlipping = false;
  void StartUp()
  {
    //set the position of the motor to the lowered position
    Motors::flipper.move_absolute(Lowered, 200);
  }
  void RequestFlip()
  {
    //start flipping
    IsFlipping = true;
  }
  void Raise()
  {
    //set the flipper to the raised position
    Motors::flipper.move_absolute(Raised, MaxSpeed);
  }
  void Ramming()
  {
    //set the flipper to the ram position
    Motors::flipper.move_absolute(Ram, MaxSpeed);
  }
  void Controller()
  {
    //check if the limit switch has been hit
    if(flipperSwitch.changedToPressed())
    {
      //reset state
      Motors::flipper.tarePosition();
      Motors::flipper.move_absolute(Raised, MaxSpeed);
    }
    //check if the flipper should be flipping
    if(IsFlipping)
    {
      //check if the flipper is in the raised position
      if(Motors::flipper.get_position() > (Raised-5) && Motors::flipper.get_position() < (Raised+5))
      {
        //lower the flipper
        Motors::flipper.move_absolute(Lowered, MaxSpeed);
        //done sending commands
        IsFlipping = false;
      }
      else
      {
        //raise the flipper
        Motors::flipper.move_absolute(Raised, MaxSpeed);
      }
    }
  }
}

namespace Sensors
{
  /*
  namespace Vision
  {
    //flag sig
    uint32_t flagSig;

    //state variables
    bool IsTargeting = false;

    //the dreaded sensor itself
    pros::Vision sensor = pros::Vision(Ports::Vision);

    //init
    void Initialize()
    {
      //set the flag signature
      flagSig = static_cast<uint32_t>(Robot::team);
    }

    //start targeting
    void StartTargeting()
    {
      IsTargeting = true;
    }

    //stop targeting
    void StopTargeting()
    {
      IsTargeting = false;
    }

    //sorting
    bool VisionObjectSort(pros::vision_object_s_t r, pros::vision_object_s_t l)
    {
      //check if they have the same x position
      if(abs(r.x_middle_coord) != abs(l.x_middle_coord))
      {
        //return true if r goes before l based on x position
        return abs(r.x_middle_coord) < abs(l.x_middle_coord);
      }
      else
      {
        //return true if r goes before l based on size
        return abs(r.height * r.width) > abs(l.height * l.width);
      }
    }

    //targeting controller
    void TargetingController()
    {
      //check if we are targeting
      if(!IsTargeting)
        return;
      Robot::WriteMessage("Targeting");
      //create an array to hold the objects being read
      pros::vision_object_s_t objects_arr[SAMPLE_SIZE];
      //read the objects in
      sensor.read_by_sig(0, flagSig, SAMPLE_SIZE, objects_arr);
      //make sure at least one object has been read
      if(sizeof(objects_arr) < 1)
        return;
      Robot::WriteMessage("Read Objects");
      //create a vector for the objects
      std::vector<pros::vision_object_s_t> objects_vect(objects_arr, objects_arr+sizeof(objects_arr));
      //sort the vector
      std::sort(objects_vect.begin(), objects_vect.end(), VisionObjectSort);
      //get the target object
      pros::vision_object_s_t object = objects_vect.front();
      //make sure the object is within bounds
      if(abs(object.x_middle_coord) > abs(LEFT_BOUND) || abs(object.y_middle_coord) > abs(TOP_BOUND))
        return;
      //get the distance away from center
      int distance = object.x_middle_coord;
      //calculate direction of rotation
      int direction = 0;
      if(distance > THRESHOLD)
      {
        //rotate in the position direction
        direction = 1;
      }
      else if(distance < -THRESHOLD)
      {
        //rotate in the negative direction
        direction = -1;
      }
      else
      {
        //we are on target
        direction = 0;
        return;
      }
      //get the proportion of angle
      double angle = (abs(distance / RIGHT_BOUND)) * MAX_ANGLE * direction;
      //rotate the chassis by angle via a blocking call
      Chassis::controller.turnAngle(angle);
    }

    void TargetingControllerV2()
    {
      //check if we should be running
      if(!IsTargeting)
        return;
      //create a debug log
      std::string messages;
      messages.append("Targeting\n");
      //get the biggest object matching the flag sig
      pros::vision_object_s_t obj = sensor.get_by_sig(0, flagSig);
      //make sure it is large enough to care about
      if((obj.width * obj.width) < 10)
        return;
      //make sure object is in bounds
      if((obj.x_middle_coord > RIGHT_BOUND) || (obj.x_middle_coord < LEFT_BOUND) || (obj.y_middle_coord > TOP_BOUND) || (obj.y_middle_coord < BOTTOM_BOUND))
        return;
      //object is good, big enough and in bounds
      messages.append("Object found\n");
      //calculate direction
      int direction = 0;
      if(obj.x_middle_coord > THRESHOLD)
      {
        //rotate clockwise
        direction = 1;
      }
      else if(obj.x_middle_coord < (THRESHOLD * -(1)))
      {
        //rotate counter clockwise
        direction = -1;
      }
      else
      {
        //we are good
        direction = 0;
      }
      messages.append("Rotating: " + std::to_string(direction) + "\n");
      //figure what how much to rotate by making a proportion between the distance from center and the side bound
    }

    //flywheel speed controller
    void FlywheelController()
    {
      //set flywheel speed
      Flywheel::speed = Flywheel::speed;
    }
  }
  */
}
