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
// Copyright (c) 1995-2005 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: OSystemWin32.hxx,v 1.6 2006-11-13 00:21:41 stephena Exp $
//============================================================================

#ifndef OSYSTEM_WIN32_HXX
#define OSYSTEM_WIN32_HXX

#include "bspf.hxx"

/**
  This class defines Windows system specific settings.

  @author  Stephen Anthony
  @version $Id: OSystemWin32.hxx,v 1.6 2006-11-13 00:21:41 stephena Exp $
*/
class OSystemWin32 : public OSystem
{
  public:
    /**
      Create a new Win32 operating system object
    */
    OSystemWin32(const string& path);

    /**
      Destructor
    */
    virtual ~OSystemWin32();

  public:
    /**
      This method runs the main loop.  Since different platforms
      may use different timing methods and/or algorithms, this method has
      been abstracted to each platform.
    */
    virtual void mainLoop();

    /**
      This method returns number of ticks in microseconds.

      @return Current time in microseconds.
    */
    virtual uInt32 getTicks();

    /**
      This method queries the dimensions of the screen for this hardware.
    */
    virtual void getScreenDimensions(int& width, int& height);
};

#endif
