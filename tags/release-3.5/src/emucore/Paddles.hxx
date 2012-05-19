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
// Copyright (c) 1995-2011 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#ifndef PADDLES_HXX
#define PADDLES_HXX

#include "bspf.hxx"
#include "Control.hxx"
#include "Event.hxx"

/**
  The standard Atari 2600 pair of paddle controllers.

  @author  Bradford W. Mott
  @version $Id$
*/
class Paddles : public Controller
{
  public:
    /**
      Create a new pair of paddle controllers plugged into the specified jack

      @param jack   The jack the controller is plugged into
      @param event  The event object to use for events
      @param system The system using this controller

      @param swapport  Whether to swap the paddles plugged into this jack
      @param swapaxis  Whether to swap the axis on the paddle (x <-> y)
      @param swapdir   Whether to swap the direction for which an axis
                       causes movement (lesser axis values cause paddle
                       resistance to decrease instead of increase)
    */
    Paddles(Jack jack, const Event& event, const System& system,
            bool swapport, bool swapaxis, bool swapdir);

    /**
      Destructor
    */
    virtual ~Paddles();

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

    /**
      Sets the sensitivity for digital emulation of paddle movement.
      This is only used for *digital* events (ie, buttons or keys,
      or digital joystick axis events); Stelladaptors or the mouse are
      not modified.

      @param sensitivity  Value from 1 to 10, with larger values
                          causing more movement
    */
    static void setDigitalSensitivity(int sensitivity);

    /**
      Sets the sensitivity for analog emulation of paddle movement
      using a mouse.

      @param sensitivity  Value from 1 to 10, with larger values
                          causing more movement
    */
    static void setMouseSensitivity(int sensitivity);

  private:
    // Range of values over which digital and mouse movement is scaled
    // to paddle resistance
    enum {
      TRIGRANGE = 4096,
      TRIGMAX   = 3856,
      TRIGMIN   = 1
    };

    // Pre-compute the events we care about based on given port
    // This will eliminate test for left or right port in update()
    Event::Type myP0AxisValue, myP1AxisValue,
                myP0DecEvent1, myP0DecEvent2, myP0IncEvent1, myP0IncEvent2,
                myP1DecEvent1, myP1DecEvent2, myP1IncEvent1, myP1IncEvent2,
                myP0FireEvent1, myP0FireEvent2, myP1FireEvent1, myP1FireEvent2,
                myAxisMouseMotion;

    // The following are used for the various mouse-axis modes
    int myMPaddleID;                // paddle to emulate in 'automatic' mode
    int myMPaddleIDX, myMPaddleIDY; // paddles to emulate in 'specific axis' mode

    bool myKeyRepeat0, myKeyRepeat1;
    int myPaddleRepeat0, myPaddleRepeat1;
    int myCharge[2], myLastCharge[2];
    int myLastAxisX, myLastAxisY;
    int myAxisDigitalZero, myAxisDigitalOne;

    static int _DIGITAL_SENSITIVITY, _DIGITAL_DISTANCE;
    static int _MOUSE_SENSITIVITY;

    // Lookup table for associating paddle buttons with controller pins
    // Yes, this is hideously complex
    static const Controller::DigitalPin ourButtonPin[2];
};

#endif