#include "UISystem.hpp"

namespace PANS
{
  namespace UISystem
  {
    //variables
    bool isInitialized = false; //is the UI system initialized
    lv_obj_t* labelTitle; //the screen title
    lv_obj_t* btnOption0; //the first option button
    lv_obj_t* labelOption0; //the first option label
    lv_obj_t* btnOption1; //the second option button
    lv_obj_t* labelOption1; //the second option label
    bool doneDialog = false; //trigger to shutdown the config dialog
    static lv_style_t disableStyle; //style to disable the buttons
    lv_obj_t* labelMessages; //the lable for all messages posted to the brain
    std::deque<std::string> messageStrings(10); //vector to store all messages posted to the brain
    void (*option0Callback)(); //the callback for option 0
    void (*option1Callback)(); //the callback for option 1
    //rendering variables
    std::vector<lv_obj_t*> renderObjects;

    void CloseDialog()
    {
      doneDialog = true; //the dialog can close
      MessageController(""); //clear the controller screen
      //set the button labels to empty
      lv_label_set_text(labelOption0, "");
      lv_label_set_text(labelOption1, "");
      //deactivate the buttons
      lv_btn_set_state(btnOption0, LV_BTN_STATE_INA);
      lv_btn_set_state(btnOption1, LV_BTN_STATE_INA);
      //set the title label to empty
      lv_label_set_text(labelTitle, ""); //set text
      lv_obj_align(labelTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); //realign
    }

    lv_res_t Option0Click(lv_obj_t* btn)
    {
      CloseDialog(); //close the dialog ui
      (*option0Callback)(); //execute the callback
      //message brain
      MessageBrain("Dialog Completed, Left Option Chosen");
      return LV_RES_OK;
    }
    lv_res_t Option1Click(lv_obj_t* btn)
    {
      CloseDialog(); //close the dialog ui
      (*option1Callback)(); //execute the callback
      //message brain
      MessageBrain("Dialog Completed, Right Option Chosen");
      return LV_RES_OK;
    }

    //prints the robot's status to the brain and the master controller
    ReturnResult PrintRobotStatus()
    {
      //create a string
      std::string large;
      std::string small;
      //are we connected to field control
      if(pros::competition::is_connected())
      {
        //are we disabled
        if(pros::competition::is_disabled())
        {
          large.append("Field|Off|");
          small.append("Off|");
        }
        else
        {
          large.append("Field|On|");
          small.append("On|");
        }
      }
      //check mode
      (pros::competition::is_autonomous()) ? large.append("Auto|") : large.append("Opcontrol|");
      (pros::competition::is_autonomous()) ? small.append("Aut|") : small.append("Drv|");
      //check controllers
      (PANS::Data::masterController.is_connected()) ? large.append("MasterOK|") : large.append("");
      (PANS::Data::partnerController.is_connected()) ? large.append("PartnerOK|") : large.append("");
      (PANS::Data::masterController.is_connected()) ? small.append("M|") : small.append("");
      (PANS::Data::partnerController.is_connected()) ? small.append("P|") : small.append("");
      //battery level
      large.append("Battery:" + std::to_string(static_cast<int>(pros::battery::get_capacity())) + "%,");
      small.append(std::to_string(static_cast<int>(pros::battery::get_capacity())) + "%");
      //battery temp
      large.append(std::to_string(static_cast<int>(pros::battery::get_temperature())) + "°");
      //print it
      MessageBrain(large);
      MessageController(small);
      return ReturnResult::Success;
    }

    //init
    ReturnResult Initialize()
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      if(isInitialized) //check if init has already run
        return ReturnResult::Success;
      labelTitle = lv_label_create(lv_scr_act(), NULL); //create the title
      btnOption0 = lv_btn_create(lv_scr_act(), NULL); //create the first option button
      lv_cont_set_fit(btnOption0, true, true); //enable button resizing
      lv_obj_align(btnOption0, NULL, LV_ALIGN_IN_LEFT_MID, 50, 0); //align the button to the left side
      lv_obj_set_free_num(btnOption0, 0); //give the button an id of 0
      labelOption0 = lv_label_create(btnOption0, NULL); //create the label inside the button
      lv_btn_set_action(btnOption0, LV_BTN_ACTION_CLICK, Option0Click); //add a on_click event to the button

      btnOption1 = lv_btn_create(lv_scr_act(), NULL); //create the second option button
      lv_cont_set_fit(btnOption1, true, true); //enable button resizing
      lv_obj_align(btnOption1, NULL, LV_ALIGN_IN_RIGHT_MID, -50, 0); //align the button to the right side
      lv_obj_set_free_num(btnOption1, 1); //give the button an id of 1
      labelOption1 = lv_label_create(btnOption1, NULL); //create the text inside the button
      isInitialized = true; //we are now initialized
      lv_btn_set_action(btnOption1, LV_BTN_ACTION_CLICK, Option1Click); //add a on_click event to the button

      lv_scr_act()->style_p = &disableStyle; //change the screen to black

      lv_style_copy(&disableStyle, &lv_style_btn_ina); //init the values
      disableStyle.body.main_color = LV_COLOR_BLACK; //set the main color to black
      disableStyle.body.border.color = LV_COLOR_BLACK; //set the border color to black
      disableStyle.body.grad_color = LV_COLOR_BLACK; //set the grad color to black
      lv_btn_set_style(btnOption0, LV_BTN_STYLE_INA, &disableStyle); //set the inactive style for the first option
      lv_btn_set_style(btnOption1, LV_BTN_STYLE_INA, &disableStyle); //set the inactive style for the second option

      labelMessages = lv_label_create(lv_scr_act(), NULL); //create the messages label
      lv_label_set_text(labelTitle, ""); //empty label
      lv_label_set_text(labelOption0, ""); //empty label
      lv_label_set_text(labelOption1, ""); //empty label
      lv_label_set_text(labelMessages, ""); //empty label

      //deactivate the buttons
      lv_btn_set_state(btnOption0, LV_BTN_STATE_INA);
      lv_btn_set_state(btnOption1, LV_BTN_STATE_INA);

      isInitialized = true; //we are now ready
      return ReturnResult::Success;
    }

    //make a config dialog on the brain screen. Title is the title for the screen, text is inside the button, calbacks execute upon selection
    ReturnResult ConfigDialog(std::string title, std::string option0, void (*callback0)(), std::string option1, void (*callback1)())
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      doneDialog = false; //in dialog
      //set the text on the controller
      MessageController("Config|Abort:X");
      //clear all messages
      ClearBrain();
      //set the title
      lv_label_set_text(labelTitle, title.c_str()); //set text
      lv_obj_align(labelTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); //realign
      //set the button labels
      lv_label_set_text(labelOption0, option0.c_str());
      lv_label_set_text(labelOption1, option1.c_str());
      //activate the buttons
      lv_btn_set_state(btnOption0, LV_BTN_STATE_REL);
      lv_btn_set_state(btnOption1, LV_BTN_STATE_REL);
      //assign callbacks
      option0Callback = callback0;
      option1Callback = callback1;
      //enter a processing loop
      while(true)
      {
        //check if the controller has commanded us to abort dialog
        if(Data::masterController.get_digital(pros::E_CONTROLLER_DIGITAL_X))
        {
          MessageController(""); //clear the controller screen
          //set the button labels to empty
          lv_label_set_text(labelOption0, "");
          lv_label_set_text(labelOption1, "");
          //deactivate the buttons
          lv_btn_set_state(btnOption0, LV_BTN_STATE_INA);
          lv_btn_set_state(btnOption1, LV_BTN_STATE_INA);
          //set the title label to empty
          lv_label_set_text(labelTitle, ""); //set text
          lv_obj_align(labelTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); //realign
          //begone
          return ReturnResult::UserAborted;
        }
        //check to see if the dialog is done
        if(doneDialog)
        {
          //begone
          return ReturnResult::Success;
        }
        //delay to wait
        pros::delay(20);
      }
    }

    //post a message to the controller.  Text must be 16 characters or less
    ReturnResult MessageController(std::string text, bool master)
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      //resize the string to fit on the screen
      text.resize(16, ' ');
      //send the message to the proper controller
      (master) ? PANS::Data::masterController.set_text(0, 0, text.c_str()) : PANS::Data::partnerController.set_text(0, 0, text.c_str());;
      return ReturnResult::Success;
    }

    //appends a message to the brain
    ReturnResult MessageBrain(std::string text)
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      messageStrings.push_front(text); //add the text and a newline
      //check if the deque is getting too long
      if(messageStrings.size() == 11)
        messageStrings.pop_back(); //remove the first element
      std::string message = ""; //create a string to hold all the text
      for(int i = 0; i < messageStrings.size(); ++i) //iterate over all messages
      {
        message.append(messageStrings[i] + "\n"); //add the message to the string
      }
      lv_label_set_text(labelMessages, message.c_str()); //set the label text to the string
      lv_obj_align(labelMessages, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); //realign
      return ReturnResult::Success;
    }

    //clears the screen of the brain
    ReturnResult ClearBrain()
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      lv_label_set_text(labelMessages, ""); //set text
      lv_label_set_text(labelTitle, ""); //clear the title as well
      lv_obj_align(labelMessages, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); //realign
      lv_obj_align(labelTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); //realign
      return ReturnResult::Success;
    }

    //prepares the UI system for vision object rendering
    ReturnResult PrepareForRendering()
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      ClearBrain(); //clear the brain of messages
      return ReturnResult::Success;
    }

    //asks the UI system to render an object
    ReturnResult RenderObject(int width, int height, int x, int y)
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      //check to make sure arguments are okay
      if(width < 0 || height < 0 || x < 0 || y < 0)
        return ReturnResult::InvaildParameters;
      //clear messages
      ClearBrain();
      //create a new object
      lv_obj_t* obj;
      obj = lv_cont_create(lv_scr_act(), NULL);
      //size it
      lv_obj_set_size(obj, width, height);
      //position it
      lv_obj_set_pos(obj, x, y);
      //add it to the vector
      renderObjects.push_back(obj);
      return ReturnResult::Success;
    }

    //asks the UI system to clear the screen of all rendered objects
    ReturnResult ClearRendering()
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      int size = renderObjects.size(); //get the number of objects
      for(int i = 0; i < size; ++i) //loop over all objects
      {
        lv_obj_del(renderObjects[i]); //delete the object
      }
      renderObjects.clear(); //empty the vector
      ClearBrain(); //clear messages while we are at it
      return ReturnResult::Success;
    }

    //shuts down object rendering
    ReturnResult StopRendering()
    {
      if(!Data::UISystem) //check if this system is allowed to run
        return ReturnResult::UserAborted;
      ClearRendering(); //clear off all objects
      MessageBrain("Visualization completed."); //go back to messaging
      return ReturnResult::Success;
    }
  }
}
