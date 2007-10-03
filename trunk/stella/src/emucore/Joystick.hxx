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
// Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: Joystick.hxx,v 1.7 2007-10-03 21:41:18 stephena Exp $
//============================================================================

#ifndef JOYSTICK_HXX
#define JOYSTICK_HXX

#include "bspf.hxx"
#include "Control.hxx"
#include "Event.hxx"

/**
  The standard Atari 2600 joystick controller.

  @author  Bradford W. Mott
  @version $Id: Joystick.hxx,v 1.7 2007-10-03 21:41:18 stephena Exp $
*/
class Joystick : public Controller
{
  public:
    /**
      Create a new joystick controller plugged into the specified jack

      @param jack The jack the controller is plugged into
      @param event The event object to use for events
    */
    Joystick(Jack jack, const Event& event);

    /**
      Destructor
    */
    virtual ~Joystick();

  public:
    /**
      Update the entire digital and analog pin state according to the
      events currently set.
    */
    virtual void update();

  private:
    // Pre-compute the events we care about based on given port
    // This will eliminate test for left or right port in update()
    Event::Type myUpEvent, myDownEvent, myLeftEvent, myRightEvent, myFireEvent;
};

#endif
