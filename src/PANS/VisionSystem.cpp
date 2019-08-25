#include "VisionSystem.hpp"

namespace PANS
{
  namespace VisionSystem
  {
    //general variables
    int sensorPort = 0;
    //vision FOV to brain screen scale factors
    int brainWidth = 480; //width of the brain screen
    int brainHeight = 240; //height of the brain screen
    float xScale = 1.519; //(brain x)/(vision x) = 480/316 = 1.519
    float yScale = 1.132; //(brain y)/(vision y) = 240/212 = 1.132
    //compute average data structures
    struct compute_object_data
    {
      //the vision object data
      pros::vision_object data;
    };
    struct compute_sig_data
    {
      //the signature this data came from
      int signature;
      //the objects
      std::vector<compute_object_data> objects;
    };
    //compute average variables
    int computeSamples = 10; //how many samples to run while averaging
    int computeObjects = 1; //how many objects to compute
    bool isComputing = false; //is the task currently computing
    bool isComputeInitialized = false; //has the compute system been initialized
    std::vector<int> computeSignatures; //a list of the signatures to compute
    pros::Mutex computeInputMutex; //a mutex lock for computeObjects and computeSignatures and computeSamples
    std::vector<compute_sig_data> computeData; //the output data of the compute function
    pros::Mutex computeOutputMutex; //a mutex lock for computeData

    //signature averaging task
    void ComputeSigAveragesTaskFunction(void* param)
    {
      //create data containers
      std::deque<std::vector<compute_sig_data>> samples(computeSamples); //push to back, pop front
      //create a vision sensor
      pros::Vision sensor(sensorPort);

      while(true) //loop
      {
        //check whether we have work to do
        if(isComputing)
        {
          //grab the input data lock
          computeInputMutex.take(TIMEOUT_MAX);
          //grab the output data lock
          computeOutputMutex.take(TIMEOUT_MAX);
          //create a data container for this sample
          std::vector<compute_sig_data> sample;

          //check if we need to run the processing loop repeatedly to init
          int runs = 1;
          if(isComputeInitialized)
          {
            runs = 1;
            isComputeInitialized = true;
          }
          else
          {
            runs = computeSamples;
          }
          for(int processRun = 0; processRun < runs; ++processRun)
          {
            //processing loop
            for(int sigNum = 0; sigNum < computeSignatures.size(); ++sigNum)
            {
              //create a holder for the data
              compute_sig_data sig;
              //set the sig id
              sig.signature = computeSignatures[sigNum];
              //loop over all objects
              for(int objNum = 0; objNum < computeObjects; ++objNum)
              {
                //create a holder for the data
                compute_object_data obj;
                //read the object data
                obj.data = sensor.get_by_sig(objNum, computeSignatures[sigNum]);
                //add the object to the sig
                sig.objects.push_back(obj);
              }
              //add the sig to the data
              sample.push_back(sig);
            }
            //reassign samples
            samples.pop_front();
            samples.push_back(sample);
          }

          //create result container
          std::vector<compute_sig_data> result;

          //averaging loop
          for(int sigNum = 0; sigNum < computeSignatures.size(); ++sigNum)
          {
            //create a holder for the data
            compute_sig_data resSig;
            //set the sig id
            resSig.signature = computeSignatures[sigNum];
            //loop over all objects
            for(int objNum = 0; objNum < computeObjects; ++objNum)
            {
              //create vision data
              pros::vision_object temp;
              //create container
              compute_object_data resObj;
              //set signature
              temp.signature = computeSignatures[sigNum];
              //temp vars to hold sums
              int left_coord = 0, top_coord = 0, width = 0, height = 0, angle = 0, x_mid = 0, y_mid = 0;
              for (auto n : samples)
              {
                //assign all sum values
                left_coord += n[sigNum].objects[objNum].data.left_coord;
                top_coord += n[sigNum].objects[objNum].data.top_coord;
                width += n[sigNum].objects[objNum].data.width;
                height += n[sigNum].objects[objNum].data.height;
                angle += n[sigNum].objects[objNum].data.angle;
                x_mid += n[sigNum].objects[objNum].data.x_middle_coord;
                y_mid += n[sigNum].objects[objNum].data.y_middle_coord;
              }
              //compute all values
              temp.type = samples[0][sigNum].objects[objNum].data.type;
              temp.left_coord = left_coord / computeSamples;
              temp.top_coord = top_coord / computeSamples;
              temp.width = width / computeSamples;
              temp.height = height / computeSamples;
              temp.angle = angle / computeSamples;
              temp.x_middle_coord = x_mid / computeSamples;
              temp.y_middle_coord = y_mid / computeSamples;
              //set data
              resObj.data = temp;
              //add the object to the sig
              resSig.objects.push_back(resObj);
            }
            //add the res sig to the res data
            result.push_back(resSig);
          }
          //set variable
          computeData = result;
          //release the output data lock
          computeOutputMutex.give();
          //release the input data lock
          computeInputMutex.give();
        }
        pros::delay(20); //delay to free task scheduler
      }
    }

    pros::vision_object GetAveragedObject(int size, int sig)
    {
      //search for the data with the signature
      //std::vector<compute_sig_data>::iterator it = std::find_if(computeData.begin(), computeData.end(), [sig](const compute_sig_data & val){if(val.signature == sig){PANS::UISystem::MessageBrain("Equal");return true;}return false;});
      //get the result
      //compute_sig_data data = *it;
      //the object
      //compute_object_data obj = data.objects[size];
      //lock the input data mutex
      computeInputMutex.take(TIMEOUT_MAX);
      //search for the signature in the input signatures
      std::vector<int>::iterator it = std::find(computeSignatures.begin(), computeSignatures.end(), sig);
      //get it's index
      int index = std::distance(computeSignatures.begin(), it);
      //release Mutex
      computeInputMutex.give();
      //lock the output mutex
      computeOutputMutex.take(TIMEOUT_MAX);
      //get the data
      compute_sig_data data = computeData[index];
      //release mutex
      computeOutputMutex.give();
      //return the vision object
      return data.objects[size].data;
    }

    //start averaging vision signatures. Pass a vector of signature ids as ints
    ReturnResult StartSigAveraging(std::vector<int> sigs, int numObjects, int samples)
    {
      computeInputMutex.take(TIMEOUT_MAX); //grab the lock
      isComputing = true; //start computing averages
      isComputeInitialized = false; //reset buffers
      computeObjects = numObjects; //set object count
      computeSamples = samples; //set number of samples
      computeSignatures = sigs; //set signatures
      computeInputMutex.give(); //release the lock
      //give the compute thread time to initially process by hanging the main thread
      pros::delay(20);
      return ReturnResult::Success;
    }

    //stop averaging all vision signatures
    ReturnResult StopSigAveraging()
    {
      isComputing = false; //stop computing averages
      isComputeInitialized = false; //reset buffers
      return ReturnResult::Success;
    }

    //initialize vision systems.  VisionPort is the port the vision sensor is plugged into
    ReturnResult Initialize(int visionPort)
    {
      sensorPort = visionPort; //store the port
      pros::Task averageTask(ComputeSigAveragesTaskFunction, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ComputeSigAveragesTask"); //start the signature average computational task
      return ReturnResult::Success;
    }

    //visualize sensor detection on the brain screen.
    ReturnResult VisualizeSignature(int signature)
    {
      //set the controller text
      PANS::UISystem::MessageController(Data::masterController, "Visual|Done:X");
      //clear the brain of messages
      PANS::UISystem::ClearBrain();
      //create a vision sensor reference
      pros::Vision sensor(sensorPort);
      //prepare the ui system for rendering
      PANS::UISystem::PrepareForRendering();
      while(true) //control loop
      {
        //clear the render screen of objects
        PANS::UISystem::ClearRendering();
        if(Data::masterController.getDigital(okapi::ControllerDigital::X)) //check for abort
        {
          PANS::UISystem::MessageController(Data::masterController, ""); //clear the controller
          //shutdown the rendering ui system
          PANS::UISystem::StopRendering();
          return ReturnResult::Success; //begone
        }
        //loop over how many objects need to be rendered
        for(int i = 0; i < sensor.get_object_count(); ++i)
        {
          //read the vision data in
          pros::vision_object rtn = sensor.get_by_sig(i, signature);
          //tell the ui system to render the object
          PANS::UISystem::RenderObject(rtn.width * xScale, rtn.height * yScale, rtn.left_coord * xScale, rtn.top_coord * yScale);
        }
        pros::delay(50); //wait
      }
    }

    //visualize sensor detection on the brain screen.  signature is the sig to read.
    ReturnResult VisualizeAveragedSignature(int signature)
    {
      //set the controller text
      PANS::UISystem::MessageController(Data::masterController, "Visual|Done:X");
      //clear the brain of messages
      PANS::UISystem::ClearBrain();
      //create a vision sensor reference
      pros::Vision sensor(sensorPort);
      //prepare the ui system for rendering
      PANS::UISystem::PrepareForRendering();
      //start the averaging system
      int numObjects = 1;
      std::vector<int> sigs(1, 1);
      StartSigAveraging(sigs, numObjects);
      while(true) //control loop
      {
        //clear the render screen of objects
        PANS::UISystem::ClearRendering();
        if(Data::masterController.getDigital(okapi::ControllerDigital::X)) //check for abort
        {
          PANS::UISystem::MessageController(Data::masterController, ""); //clear the controller
          //shutdown the rendering ui system
          PANS::UISystem::StopRendering();
          StopSigAveraging(); //stop averaging
          return ReturnResult::Success; //begone
        }
        //loop over how many objects need to be rendered
        for(int i = 0; i < numObjects; ++i)
        {
          //get the averaged data
          //pros::vision_object rtn = sensor.get_by_sig(i, signature);
          pros::vision_object rtn = GetAveragedObject(i, signature);
          //tell the ui system to render the object
          PANS::UISystem::RenderObject(rtn.width * xScale, rtn.height * yScale, rtn.left_coord * xScale, rtn.top_coord * yScale);
        }
        pros::delay(50); //wait
      }
    }
  }
}
