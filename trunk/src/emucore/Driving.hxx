//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2012 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#ifndef DRIVING_HXX
#define DRIVING_HXX

#include "bspf.hxx"
#include "Control.hxx"
#include "Event.hxx"

/**
  The standard Atari 2600 Indy 500 driving controller.

  @author  Bradford W. Mott
  @version $Id$
*/
class Driving : public Controller
{
  public:
    /**
      Create a new Indy 500 driving controller plugged into 
      the specified jack

      @param jack   The jack the controller is plugged into
      @param event  The event object to use for events
      @param system The system using this controller
    */
    Driving(Jack jack, const Event& event, const System& system);

    /**
      Destructor
    */
    virtual ~Driving();

  public:
    /**
      Update the entire digital and analog pin state according to the
      events currently set.
    */
    void update();

    /**
      Determines how this controller will treat values received from the
      X and Y axis of the mouse.  Since not all controllers use the mouse,
      it's up to the specific class to decide how to use this data.

      If either of the axis is set to 'Automatic', then we automatically
      use this number for the control type as follows:
        0 - paddle 0, joystick 0 (and controllers similar to a joystick)
        1 - paddle 1, joystick 1 (and controllers similar to a joystick)
        2 - paddle 2, joystick 0 (and controllers similar to a joystick)
        3 - paddle 3, joystick 1 (and controllers similar to a joystick)

      @param xaxis   How the controller should use x-axis data
      @param yaxis   How the controller should use y-axis data
      @param ctrlID  The controller ID to use axis 'auto' mode
    */
    void setMouseControl(
        MouseAxisControl xaxis, MouseAxisControl yaxis, int ctrlID = -1);

  private:
    // Counter to iterate through the gray codes
    uInt32 myCounter;

    // Index into the gray code table
    uInt32 myGrayIndex;

    // Y axis value from last yaxis event that was used to generate a new
    // gray code
    int myLastYaxis;

    // Pre-compute the events we care about based on given port
    // This will eliminate test for left or right port in update()
    Event::Type myCWEvent, myCCWEvent, myFireEvent,
                myXAxisValue, myYAxisValue, myAxisMouseMotion;

    // Controller to emulate in mouse axis 'automatic' mode
    int myControlID;  
};

#endif
