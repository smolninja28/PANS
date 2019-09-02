# PANS: PROS Anti-Nuisance Systems
PANS is a code library for V5 PROS that aims to simplify some difficult tasks by creating easy-to-use systems.
https://pros.cs.purdue.edu/

PANS contains several systems:  
UISystem:
- Robust functions for printing messages to a V5 controller, or to the V5 Brain screen.  
- A function for creating configuration dialogs to set up your robot's state before the match.  Uses include selecting which autonomous to run.
- A function for printing the status of the robot to the V5 Brain screen and the master V5 controller.

VisionSystem:
- A systen for drawing what the V5 Vision Sensor "sees" on the V5 Brain screen by rendering all the objects it registers for a given signature.  
- A system to average all objects retrived from the V5 Vision Sensor, and then make it possible to access these objects.  This provides a "smoother" data stream that is more accurate.

PANS is completely open source, so anyone can use, modify, and distribute this code.

Head over to the Wiki to learn more!
https://github.com/0X4B-Developer/PANS/wiki
