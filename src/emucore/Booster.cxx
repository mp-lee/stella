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
// Copyright (c) 1995-2016 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#include "Event.hxx"
#include "Booster.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BoosterGrip::BoosterGrip(Jack jack, const Event& event, const System& system)
  : Controller(jack, event, system, Controller::BoosterGrip),
    myControlID(-1)
{
  if(myJack == Left)
  {
    myUpEvent      = Event::JoystickZeroUp;
    myDownEvent    = Event::JoystickZeroDown;
    myLeftEvent    = Event::JoystickZeroLeft;
    myRightEvent   = Event::JoystickZeroRight;
    myFireEvent    = Event::JoystickZeroFire;
    myTriggerEvent = Event::JoystickZeroFire5;
    myBoosterEvent = Event::JoystickZeroFire9;
    myXAxisValue   = Event::SALeftAxis0Value;
    myYAxisValue   = Event::SALeftAxis1Value;
  }
  else
  {
    myUpEvent      = Event::JoystickOneUp;
    myDownEvent    = Event::JoystickOneDown;
    myLeftEvent    = Event::JoystickOneLeft;
    myRightEvent   = Event::JoystickOneRight;
    myFireEvent    = Event::JoystickOneFire;
    myTriggerEvent = Event::JoystickOneFire5;
    myBoosterEvent = Event::JoystickOneFire9;
    myXAxisValue   = Event::SARightAxis0Value;
    myYAxisValue   = Event::SARightAxis1Value;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BoosterGrip::update()
{
  // Digital events (from keyboard or joystick hats & buttons)
  myDigitalPinState[One]   = (myEvent.get(myUpEvent) == 0);
  myDigitalPinState[Two]   = (myEvent.get(myDownEvent) == 0);
  myDigitalPinState[Three] = (myEvent.get(myLeftEvent) == 0);
  myDigitalPinState[Four]  = (myEvent.get(myRightEvent) == 0);
  myDigitalPinState[Six]   = (myEvent.get(myFireEvent) == 0);

  // The CBS Booster-grip has two more buttons on it.  These buttons are
  // connected to the inputs usually used by paddles.
  myAnalogPinValue[Five] = (myEvent.get(myTriggerEvent) != 0) ? 
                            minimumResistance : maximumResistance;
  myAnalogPinValue[Nine] = (myEvent.get(myBoosterEvent) != 0) ? 
                            minimumResistance : maximumResistance;

  // Axis events (usually generated by the Stelladaptor)
  int xaxis = myEvent.get(myXAxisValue);
  int yaxis = myEvent.get(myYAxisValue);
  if(xaxis > 16384-4096)
  {
    myDigitalPinState[Four] = false;
    // Stelladaptor sends "half moved right" for L+R pushed together
    if(xaxis < 16384+4096)
      myDigitalPinState[Three] = false;
  }
  else if(xaxis < -16384)
    myDigitalPinState[Three] = false;
  if(yaxis > 16384-4096)
  {
    myDigitalPinState[Two] = false;
    // Stelladaptor sends "half moved down" for U+D pushed together
    if(yaxis < 16384+4096)
      myDigitalPinState[One] = false;
  }
  else if(yaxis < -16384)
    myDigitalPinState[One] = false;

  // Mouse motion and button events
  if(myControlID > -1)
  {
    // The following code was taken from z26
    #define MJ_Threshold 2
    int mousex = myEvent.get(Event::MouseAxisXValue),
        mousey = myEvent.get(Event::MouseAxisYValue);
    if(mousex || mousey)
    {
      if((!(abs(mousey) > abs(mousex) << 1)) && (abs(mousex) >= MJ_Threshold))
      {
        if(mousex < 0)
          myDigitalPinState[Three] = false;
        else if (mousex > 0)
          myDigitalPinState[Four] = false;
      }

      if((!(abs(mousex) > abs(mousey) << 1)) && (abs(mousey) >= MJ_Threshold))
      {
        if(mousey < 0)
          myDigitalPinState[One] = false;
        else if(mousey > 0)
          myDigitalPinState[Two] = false;
      }
    }
    // Get mouse button state
    if(myEvent.get(Event::MouseButtonLeftValue))
      myDigitalPinState[Six] = false;
    if(myEvent.get(Event::MouseButtonRightValue))
      myAnalogPinValue[Nine] = minimumResistance;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BoosterGrip::setMouseControl(
    Controller::Type xtype, int xid, Controller::Type ytype, int yid)
{
  // Currently, the booster-grip takes full control of the mouse, using both
  // axes for its two degrees of movement, and the left/right buttons for
  // fire and booster, respectively
  if(xtype == Controller::BoosterGrip && ytype == Controller::BoosterGrip &&
     xid == yid)
  {
    myControlID = ((myJack == Left && xid == 0) ||
                   (myJack == Right && xid == 1)
                  ) ? xid : -1;
  }
  else
    myControlID = -1;

  return true;
}
