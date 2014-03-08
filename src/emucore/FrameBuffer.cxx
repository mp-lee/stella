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
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#include <algorithm>
#include <sstream>

#include "bspf.hxx"

#include "CommandMenu.hxx"
#include "Console.hxx"
#include "EventHandler.hxx"
#include "Event.hxx"
#include "Font.hxx"
#include "StellaFont.hxx"
#include "StellaMediumFont.hxx"
#include "StellaLargeFont.hxx"
#include "ConsoleFont.hxx"
#include "Launcher.hxx"
#include "Menu.hxx"
#include "OSystem.hxx"
#include "Settings.hxx"
#include "TIA.hxx"

#include "FrameBuffer.hxx"

#ifdef DEBUGGER_SUPPORT
  #include "Debugger.hxx"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBuffer::FrameBuffer(OSystem* osystem)
  : myOSystem(osystem),
    myRedrawEntireFrame(true),
    myUsePhosphor(false),
    myPhosphorBlend(77),
    myInitializedCount(0),
    myPausedCount(0)
{
  myMsg.surface = myStatsMsg.surface = NULL;
  myMsg.enabled = myStatsMsg.enabled = false;

  // Load NTSC filter settings
  myNTSCFilter.loadConfig(myOSystem->settings());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBuffer::~FrameBuffer(void)
{
  delete myFont;
  delete myInfoFont;
  delete mySmallFont;
  delete myLauncherFont;

  // Free all allocated surfaces
  while(!mySurfaceList.empty())
  {
    delete (*mySurfaceList.begin()).second;
    mySurfaceList.erase(mySurfaceList.begin());
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FrameBuffer::initialize()
{
  // Get desktop resolution and supported renderers
  queryHardware(myDesktopWidth, myDesktopHeight, myRenderers);

  // Check the 'maxres' setting, which is an undocumented developer feature
  // that specifies the desktop size (not normally set)
  const GUI::Size& s = myOSystem->settings().getSize("maxres");
  if(s.w > 0 && s.h > 0)
  {
    myDesktopWidth  = s.w;
    myDesktopHeight = s.h;
  }

  // Various parts of the codebase assume a minimum screen size of 320x240
  myDesktopWidth = BSPF_max(myDesktopWidth, 320u);
  myDesktopHeight = BSPF_max(myDesktopHeight, 240u);
  if(!(myDesktopWidth >= 320 && myDesktopHeight >= 240))
  {
    myOSystem->logMessage("ERROR: video init failed, "
                          "window 320x240 or larger required", 0);
    return false;
  }

  ////////////////////////////////////////////////////////////////////
  // Create fonts to draw text
  // NOTE: the logic determining appropriate font sizes is done here,
  //       so that the UI classes can just use the font they expect,
  //       and not worry about it
  //       This logic should also take into account the size of the
  //       framebuffer, and try to be intelligent about font sizes
  //       We can probably add ifdefs to take care of corner cases,
  //       but that means we've failed to abstract it enough ...
  ////////////////////////////////////////////////////////////////////
  bool smallScreen = myDesktopWidth < 640 || myDesktopHeight < 480;

  // This font is used in a variety of situations when a really small
  // font is needed; we let the specific widget/dialog decide when to
  // use it
  mySmallFont = new GUI::Font(GUI::stellaDesc);

  // The general font used in all UI elements
  // This is determined by the size of the framebuffer
  myFont = new GUI::Font(smallScreen ? GUI::stellaDesc : GUI::stellaMediumDesc);

  // The info font used in all UI elements
  // This is determined by the size of the framebuffer
  myInfoFont = new GUI::Font(smallScreen ? GUI::stellaDesc : GUI::consoleDesc);

  // The font used by the ROM launcher
  // Normally, this is configurable by the user, except in the case of
  // very small screens
  if(!smallScreen)
  {    
    const string& lf = myOSystem->settings().getString("launcherfont");
    if(lf == "small")
      myLauncherFont = new GUI::Font(GUI::consoleDesc);
    else if(lf == "medium")
      myLauncherFont = new GUI::Font(GUI::stellaMediumDesc);
    else
      myLauncherFont = new GUI::Font(GUI::stellaLargeDesc);
  }
  else
    myLauncherFont = new GUI::Font(GUI::stellaDesc);

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FBInitStatus FrameBuffer::createDisplay(const string& title,
                                        uInt32 width, uInt32 height)
{
  myInitializedCount++;

  // A 'windowed' system is defined as one where the window size can be
  // larger than the screen size, as there's some sort of window manager
  // that takes care of it (all current desktop systems fall in this category)
  // However, some systems have no concept of windowing, and have hard limits
  // on how large a window can be (ie, the size of the 'desktop' is the
  // absolute upper limit on window size)
  //
  // If the WINDOWED_SUPPORT macro is defined, we treat the system as the
  // former type; if not, as the latter type

  bool useFullscreen = false;
#ifdef WINDOWED_SUPPORT
  // We assume that a desktop size of at least 640x480 means that we're
  // running on a 'large' system, and the window size requirements can
  // be relaxed
  // Otherwise, we treat the system as if WINDOWED_SUPPORT is not defined
  if(myOSystem->desktopWidth() < 640 && myOSystem->desktopHeight() < 480 &&
      (myOSystem->desktopWidth() < width || myOSystem->desktopHeight() < height))
    return kFailTooLarge;

  if(myOSystem->settings().getString("fullscreen") == "1")
  {
    if(myOSystem->desktopWidth() < width || myOSystem->desktopHeight() < height)
      return kFailTooLarge;

    useFullscreen = true;
  }
  else
    useFullscreen = false;
#else
  // Make sure this mode is even possible
  // We only really need to worry about it in non-windowed environments,
  // where requesting a window that's too large will probably cause a crash
  if(myOSystem->desktopWidth() < width || myOSystem->desktopHeight() < height)
    return kFailTooLarge;
#endif

  // Set the available video modes for this framebuffer
  setAvailableVidModes(width, height);

  // Initialize video subsystem (make sure we get a valid mode)
  string pre_about = about();
  VideoMode mode = getSavedVidMode();
  if(width <= mode.screen_w && height <= mode.screen_h)
  {
    if(setVideoMode(title, mode, useFullscreen))
    {
      myImageRect.setWidth(mode.image_w);
      myImageRect.setHeight(mode.image_h);
      myImageRect.moveTo(mode.image_x, mode.image_y);

      myScreenRect.setWidth(mode.screen_w);
      myScreenRect.setHeight(mode.screen_h);

      // Did we get the requested fullscreen state?
      const string& fullscreen = myOSystem->settings().getString("fullscreen");
      if(fullscreen != "-1")
        myOSystem->settings().setValue("fullscreen", fullScreen() ? "1" : "0");
      setCursorState();
    }
    else
    {
      myOSystem->logMessage("ERROR: Couldn't initialize video subsystem", 0);
      return kFailNotSupported;
    }
  }
  else
    return kFailTooLarge;

  // Erase any messages from a previous run
  myMsg.counter = 0;

  // Create surfaces for TIA statistics and general messages
  myStatsMsg.color = kBtnTextColor;
  myStatsMsg.w = infoFont().getMaxCharWidth() * 24 + 2;
  myStatsMsg.h = (infoFont().getFontHeight() + 2) * 2;

 if(myStatsMsg.surface == NULL)
  {
    uInt32 surfaceID = allocateSurface(myStatsMsg.w, myStatsMsg.h);
    myStatsMsg.surface = surface(surfaceID);
  }
  if(myMsg.surface == NULL)
  {
    uInt32 surfaceID = allocateSurface(640, font().getFontHeight()+10);
    myMsg.surface = surface(surfaceID);
  }

  // Take care of some items that are only done once per framebuffer creation.
  if(myInitializedCount == 1)
  {
    myOSystem->logMessage(about(), 1);
    setUIPalette();
    setWindowIcon();
  }
  else
  {
    string post_about = about();
    if(post_about != pre_about)
      myOSystem->logMessage(post_about, 1);
  }

  return kSuccess;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::update()
{
  // Determine which mode we are in (from the EventHandler)
  // Take care of S_EMULATE mode here, otherwise let the GUI
  // figure out what to draw
  switch(myOSystem->eventHandler().state())
  {
    case EventHandler::S_EMULATE:
    {
      // Run the console for one frame
      // Note that the debugger can cause a breakpoint to occur, which changes
      // the EventHandler state 'behind our back' - we need to check for that
      myOSystem->console().tia().update();
  #ifdef DEBUGGER_SUPPORT
      if(myOSystem->eventHandler().state() != EventHandler::S_EMULATE) break;
  #endif
      if(myOSystem->eventHandler().frying())
        myOSystem->console().fry();

      // And update the screen
      drawTIA(myRedrawEntireFrame);

      // Show frame statistics
      if(myStatsMsg.enabled)
      {
        const ConsoleInfo& info = myOSystem->console().about();
        char msg[30];
        BSPF_snprintf(msg, 30, "%3u @ %3.2ffps => %s",
                myOSystem->console().tia().scanlines(),
                myOSystem->console().getFramerate(), info.DisplayFormat.c_str());
        myStatsMsg.surface->fillRect(0, 0, myStatsMsg.w, myStatsMsg.h, kBGColor);
        myStatsMsg.surface->drawString(infoFont(),
          msg, 1, 1, myStatsMsg.w, myStatsMsg.color, kTextAlignLeft);
        myStatsMsg.surface->drawString(infoFont(),
          info.BankSwitch, 1, 15, myStatsMsg.w, myStatsMsg.color, kTextAlignLeft);
        myStatsMsg.surface->addDirtyRect(0, 0, 0, 0);  // force a full draw
        myStatsMsg.surface->setPos(myImageRect.x() + 1, myImageRect.y() + 1);
        myStatsMsg.surface->update();
      }
      break;  // S_EMULATE
    }

    case EventHandler::S_PAUSE:
    {
      // Only update the screen if it's been invalidated
      if(myRedrawEntireFrame)
        drawTIA(true);

      // Show a pause message every 5 seconds
      if(myPausedCount++ >= 7*myOSystem->frameRate())
      {
        myPausedCount = 0;
        showMessage("Paused", kMiddleCenter);
      }
      break;  // S_PAUSE
    }

    case EventHandler::S_MENU:
    {
      // When onscreen messages are enabled in double-buffer mode,
      // a full redraw is required
      myOSystem->menu().draw(myMsg.enabled && isDoubleBuffered());
      break;  // S_MENU
    }

    case EventHandler::S_CMDMENU:
    {
      // When onscreen messages are enabled in double-buffer mode,
      // a full redraw is required
      myOSystem->commandMenu().draw(myMsg.enabled && isDoubleBuffered());
      break;  // S_CMDMENU
    }

    case EventHandler::S_LAUNCHER:
    {
      // When onscreen messages are enabled in double-buffer mode,
      // a full redraw is required
      myOSystem->launcher().draw(myMsg.enabled && isDoubleBuffered());
      break;  // S_LAUNCHER
    }

#ifdef DEBUGGER_SUPPORT
    case EventHandler::S_DEBUGGER:
    {
      // When onscreen messages are enabled in double-buffer mode,
      // a full redraw is required
      myOSystem->debugger().draw(myMsg.enabled && isDoubleBuffered());
      break;  // S_DEBUGGER
    }
#endif

    default:
      return;
  }

  // Draw any pending messages
  if(myMsg.enabled)
    drawMessage();

  // Do any post-frame stuff
  postFrameUpdate();

  // The frame doesn't need to be completely redrawn anymore
  myRedrawEntireFrame = false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::showMessage(const string& message, MessagePosition position,
                              bool force)
{
  // Only show messages if they've been enabled
  if(!(force || myOSystem->settings().getBool("uimessages")))
    return;

  // Erase old messages on the screen
  if(myMsg.counter > 0)
  {
    myRedrawEntireFrame = true;
    refresh();
  }

  // Precompute the message coordinates
  myMsg.text    = message;
  myMsg.counter = uInt32(myOSystem->frameRate()) << 1; // Show message for 2 seconds
  myMsg.color   = kBtnTextColor;

  myMsg.w = font().getStringWidth(myMsg.text) + 10;
  myMsg.h = font().getFontHeight() + 8;
  myMsg.surface->setWidth(myMsg.w);
  myMsg.surface->setHeight(myMsg.h);
  myMsg.position = position;
  myMsg.enabled = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::toggleFrameStats()
{
  showFrameStats(!myOSystem->settings().getBool("stats"));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::showFrameStats(bool enable)
{
  myOSystem->settings().setValue("stats", enable);
  myStatsMsg.enabled = enable;
  refresh();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::enableMessages(bool enable)
{
  if(enable)
  {
    // Only re-enable frame stats if they were already enabled before
    myStatsMsg.enabled = myOSystem->settings().getBool("stats");
  }
  else
  {
    // Temporarily disable frame stats
    myStatsMsg.enabled = false;

    // Erase old messages on the screen
    myMsg.enabled = false;
    myMsg.counter = 0;

    refresh();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
inline void FrameBuffer::drawMessage()
{
  // Draw the bounded box and text
  switch(myMsg.position)
  {
    case kTopLeft:
      myMsg.x = 5;
      myMsg.y = 5;
      break;

    case kTopCenter:
      myMsg.x = (myImageRect.width() - myMsg.w) >> 1;
      myMsg.y = 5;
      break;

    case kTopRight:
      myMsg.x = myImageRect.width() - myMsg.w - 5;
      myMsg.y = 5;
      break;

    case kMiddleLeft:
      myMsg.x = 5;
      myMsg.y = (myImageRect.height() - myMsg.h) >> 1;
      break;

    case kMiddleCenter:
      myMsg.x = (myImageRect.width() - myMsg.w) >> 1;
      myMsg.y = (myImageRect.height() - myMsg.h) >> 1;
      break;

    case kMiddleRight:
      myMsg.x = myImageRect.width() - myMsg.w - 5;
      myMsg.y = (myImageRect.height() - myMsg.h) >> 1;
      break;

    case kBottomLeft:
      myMsg.x = 5;
      myMsg.y = myImageRect.height() - myMsg.h - 5;
      break;

    case kBottomCenter:
      myMsg.x = (myImageRect.width() - myMsg.w) >> 1;
      myMsg.y = myImageRect.height() - myMsg.h - 5;
      break;

    case kBottomRight:
      myMsg.x = myImageRect.width() - myMsg.w - 5;
      myMsg.y = myImageRect.height() - myMsg.h - 5;
      break;
  }

  myMsg.surface->setPos(myMsg.x + myImageRect.x(), myMsg.y + myImageRect.y());
  myMsg.surface->fillRect(1, 1, myMsg.w-2, myMsg.h-2, kBtnColor);
  myMsg.surface->box(0, 0, myMsg.w, myMsg.h, kColor, kShadowColor);
  myMsg.surface->drawString(font(), myMsg.text, 4, 4,
                            myMsg.w, myMsg.color, kTextAlignLeft);
  myMsg.counter--;

  // Either erase the entire message (when time is reached),
  // or show again this frame
  if(myMsg.counter == 0)  // Force an immediate update
  {
    myMsg.enabled = false;
    refresh();
  }
  else
  {
    myMsg.surface->addDirtyRect(0, 0, 0, 0);
    myMsg.surface->update();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::refresh()
{
  // This method partly duplicates the behaviour in ::update()
  // Here, however, make sure to redraw *all* surfaces applicable to the
  // current EventHandler state
  // We also check for double-buffered modes, and when present
  // update both buffers accordingly
  //
  // This method is in essence a FULL refresh, putting all rendering
  // buffers in a known, fully redrawn state

  switch(myOSystem->eventHandler().state())
  {
    case EventHandler::S_EMULATE:
    case EventHandler::S_PAUSE:
      invalidate();
      drawTIA(true);
      if(isDoubleBuffered())
      {
        postFrameUpdate();
        invalidate();
        drawTIA(true);
      }
      break;

    case EventHandler::S_MENU:
      invalidate();
      drawTIA(true);
      myOSystem->menu().draw(true);
      if(isDoubleBuffered())
      {
        postFrameUpdate();
        invalidate();
        drawTIA(true);
        myOSystem->menu().draw(true);
      }
      break;

    case EventHandler::S_CMDMENU:
      invalidate();
      drawTIA(true);
      myOSystem->commandMenu().draw(true);
      if(isDoubleBuffered())
      {
        postFrameUpdate();
        invalidate();
        drawTIA(true);
        myOSystem->commandMenu().draw(true);
      }
      break;

    case EventHandler::S_LAUNCHER:
      invalidate();
      myOSystem->launcher().draw(true);
      if(isDoubleBuffered())
      {
        postFrameUpdate();
        invalidate();
        myOSystem->launcher().draw(true);
      }
      break;

  #ifdef DEBUGGER_SUPPORT
    case EventHandler::S_DEBUGGER:
      invalidate();
      myOSystem->debugger().draw(true);
      if(isDoubleBuffered())
      {
        postFrameUpdate();
        invalidate();
        myOSystem->debugger().draw(true);
      }
      break;
  #endif

    default:
      break;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setNTSC(NTSCFilter::Preset preset, bool show)
{
  ostringstream buf;
  if(preset == NTSCFilter::PRESET_OFF)
  {
    enableNTSC(false);
    buf << "TV filtering disabled";
  }
  else
  {
    enableNTSC(true);
    const string& mode = myNTSCFilter.setPreset(preset);
    buf << "TV filtering (" << mode << " mode)";
  }
  myOSystem->settings().setValue("tv.filter", (int)preset);

  if(show) showMessage(buf.str());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setScanlineIntensity(int amount)
{
  ostringstream buf;
  if(ntscEnabled())
  {
    uInt32 intensity = enableScanlines(amount);
    buf << "Scanline intensity at " << intensity  << "%";
    myOSystem->settings().setValue("tv.scanlines", intensity);
  }
  else
    buf << "Scanlines only available in TV filtering mode";

  showMessage(buf.str());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::toggleScanlineInterpolation()
{
  ostringstream buf;
  if(ntscEnabled())
  {
    bool enable = !myOSystem->settings().getBool("tv.scaninter");
    enableScanlineInterpolation(enable);
    buf << "Scanline interpolation " << (enable ? "enabled" : "disabled");
    myOSystem->settings().setValue("tv.scaninter", enable);
  }
  else
    buf << "Scanlines only available in TV filtering mode";

  showMessage(buf.str());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt32 FrameBuffer::allocateSurface(int w, int h)
{
  // Create a new surface
  FBSurface* surface = createSurface(w, h);

  // Add it to the list
  mySurfaceList.insert(make_pair((uInt32)mySurfaceList.size(), surface));

  // Return a reference to it
  return mySurfaceList.size() - 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FBSurface* FrameBuffer::surface(uInt32 id) const
{
  map<uInt32,FBSurface*>::const_iterator iter = mySurfaceList.find(id);
  return iter != mySurfaceList.end() ? iter->second : NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::resetSurfaces(FBSurface* tiasurface)
{
  // Free all resources for each surface, then reload them
  // Due to possible timing and/or synchronization issues, all free()'s
  // are done first, then all reload()'s
  // Any derived FrameBuffer classes that call this method should be
  // aware of these restrictions, and act accordingly

  map<uInt32,FBSurface*>::iterator iter;
  for(iter = mySurfaceList.begin(); iter != mySurfaceList.end(); ++iter)
    iter->second->free();
  if(tiasurface)
    tiasurface->free();
  for(iter = mySurfaceList.begin(); iter != mySurfaceList.end(); ++iter)
    iter->second->reload();
  if(tiasurface)
    tiasurface->reload();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt32 FrameBuffer::tiaPixel(uInt32 idx, uInt8 shift) const
{
  uInt8 c = *(myOSystem->console().tia().currentFrameBuffer() + idx) | shift;
  uInt8 p = *(myOSystem->console().tia().previousFrameBuffer() + idx) | shift;

  return (!myUsePhosphor ? myDefPalette[c] : myAvgPalette[c][p]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setTIAPalette(const uInt32* palette)
{
  int i, j;

  // Set palette for normal fill
  for(i = 0; i < 256; ++i)
  {
    Uint8 r = (palette[i] >> 16) & 0xff;
    Uint8 g = (palette[i] >> 8) & 0xff;
    Uint8 b = palette[i] & 0xff;

    myDefPalette[i] = mapRGB(r, g, b);
  }

  // Set palette for phosphor effect
  for(i = 0; i < 256; ++i)
  {
    for(j = 0; j < 256; ++j)
    {
      uInt8 ri = (palette[i] >> 16) & 0xff;
      uInt8 gi = (palette[i] >> 8) & 0xff;
      uInt8 bi = palette[i] & 0xff;
      uInt8 rj = (palette[j] >> 16) & 0xff;
      uInt8 gj = (palette[j] >> 8) & 0xff;
      uInt8 bj = palette[j] & 0xff;

      Uint8 r = (Uint8) getPhosphor(ri, rj);
      Uint8 g = (Uint8) getPhosphor(gi, gj);
      Uint8 b = (Uint8) getPhosphor(bi, bj);

      myAvgPalette[i][j] = mapRGB(r, g, b);
    }
  }

  myRedrawEntireFrame = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setUIPalette()
{
  // Set palette for GUI
  for(int i = 0, j = 256; i < kNumColors-256; ++i, ++j)
  {
    Uint8 r = (ourGUIColors[i] >> 16) & 0xff;
    Uint8 g = (ourGUIColors[i] >> 8) & 0xff;
    Uint8 b = ourGUIColors[i] & 0xff;

    myDefPalette[j] = mapRGB(r, g, b);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::stateChanged(EventHandler::State state)
{
  // Make sure any onscreen messages are removed
  myMsg.enabled = false;
  myMsg.counter = 0;

  myRedrawEntireFrame = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::toggleFullscreen()
{
  setFullscreen(!fullScreen());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setFullscreen(bool enable)
{
#if 0 //FIXSDL
#ifdef WINDOWED_SUPPORT
  // '-1' means fullscreen mode is completely disabled
  bool full = enable && myOSystem->settings().getString("fullscreen") != "-1";
  setHint(kFullScreen, full);

  // Do a dummy call to getSavedVidMode to set up the modelists
  // and have it point to the correct 'current' mode
  getSavedVidMode();

  // Do a mode change to the 'current' mode by not passing a '+1' or '-1'
  // to changeVidMode()
  changeVidMode(0);
#endif
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FrameBuffer::changeVidMode(int direction)
{
#if 0 //FIXSDL
  EventHandler::State state = myOSystem->eventHandler().state();
  bool inUIMode = (state == EventHandler::S_DEBUGGER ||
                   state == EventHandler::S_LAUNCHER);

  // Ignore any attempts to change video size while in UI mode
  if(inUIMode && direction != 0)
    return false;

  // Only save mode changes in TIA mode with a valid selector
  bool saveModeChange = !inUIMode && (direction == -1 || direction == +1);

  if(direction == +1)
    myCurrentModeList->next();
  else if(direction == -1)
    myCurrentModeList->previous();

  VideoMode vidmode = myCurrentModeList->current(myOSystem->settings(), fullScreen());
  if(setVidMode(vidmode))
  {
    myImageRect.setWidth(vidmode.image_w);
    myImageRect.setHeight(vidmode.image_h);
    myImageRect.moveTo(vidmode.image_x, vidmode.image_y);

    myScreenRect.setWidth(vidmode.screen_w);
    myScreenRect.setHeight(vidmode.screen_h);

    // Did we get the requested fullscreen state?
    const string& fullscreen = myOSystem->settings().getString("fullscreen");
    if(fullscreen != "-1")
      myOSystem->settings().setValue("fullscreen", fullScreen() ? "1" : "0");
    setCursorState();

    if(!inUIMode)
    {
      if(direction != 0)  // only show message when mode actually changes
        showMessage(vidmode.gfxmode.description);
    }
    if(saveModeChange)
      myOSystem->settings().setValue("tia.filter", vidmode.gfxmode.name);

    refresh();
  }
  else
    return false;
#endif
  return true;
/*
cerr << "New mode:" << endl
	<< "    screen w = " << newmode.screen_w << endl
	<< "    screen h = " << newmode.screen_h << endl
	<< "    image x  = " << newmode.image_x << endl
	<< "    image y  = " << newmode.image_y << endl
	<< "    image w  = " << newmode.image_w << endl
	<< "    image h  = " << newmode.image_h << endl
	<< "    zoom     = " << newmode.zoom << endl
	<< "    name     = " << newmode.name << endl
	<< endl;
*/
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setCursorState()
{
  // Always grab mouse in fullscreen or during emulation (if enabled),
  // and don't show the cursor during emulation
  bool emulation =
      myOSystem->eventHandler().state() == EventHandler::S_EMULATE;
  grabMouse(fullScreen() ||
    (emulation && myOSystem->settings().getBool("grabmouse")));
  showCursor(!emulation);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::toggleGrabMouse()
{
  bool state = myOSystem->settings().getBool("grabmouse");
  myOSystem->settings().setValue("grabmouse", !state);
  setCursorState();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 FrameBuffer::getPhosphor(uInt8 c1, uInt8 c2) const
{
  if(c2 > c1)
    BSPF_swap(c1, c2);

  return ((c1 - c2) * myPhosphorBlend)/100 + c2;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const VariantList& FrameBuffer::supportedTIAFilters()
{
  uInt32 max_zoom = maxWindowSizeForScreen(320, 210,
                    myOSystem->desktopWidth(), myOSystem->desktopHeight());

  uInt32 firstmode = 1;
  if(myOSystem->desktopWidth() < 640 || myOSystem->desktopHeight() < 480)
    firstmode = 0;

  myTIAFilters.clear();
  for(uInt32 i = firstmode; i < GFX_NumModes; ++i)
  {
    if(ourGraphicsModes[i].zoom <= max_zoom)
    {
      myTIAFilters.push_back(ourGraphicsModes[i].description,
                             ourGraphicsModes[i].name);
    }
  }
  return myTIAFilters;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt32 FrameBuffer::maxWindowSizeForScreen(uInt32 baseWidth, uInt32 baseHeight,
                    uInt32 screenWidth, uInt32 screenHeight)
{
  uInt32 multiplier = 1;
  for(;;)
  {
    // Figure out the zoomed size of the window
    uInt32 width  = baseWidth * multiplier;
    uInt32 height = baseHeight * multiplier;

    if((width > screenWidth) || (height > screenHeight))
      break;

    ++multiplier;
  }
  return multiplier > 1 ? multiplier - 1 : 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::setAvailableVidModes(uInt32 baseWidth, uInt32 baseHeight)
{
  // Modelists are different depending on what state we're in
  EventHandler::State state = myOSystem->eventHandler().state();
  bool inUIMode = (state == EventHandler::S_DEBUGGER ||
                   state == EventHandler::S_LAUNCHER);

  myWindowedModeList.clear();
  myFullscreenModeList.clear();

  // In UI/windowed mode, there's only one valid video mode we can use
  // We don't use maxWindowSizeForScreen here, since UI mode has to open its
  // window at the requested size
  if(inUIMode)
  {
    VideoMode m;
    m.image_x = m.image_y = 0;
    m.image_w = m.screen_w = baseWidth;
    m.image_h = m.screen_h = baseHeight;
    m.gfxmode = ourGraphicsModes[0];  // this should be zoom1x

    addVidMode(m);
  }
  else
  {
    // Scan list of filters, adding only those which are appropriate
    // for the given dimensions
    uInt32 max_zoom = maxWindowSizeForScreen(baseWidth, baseHeight,
                      myOSystem->desktopWidth(), myOSystem->desktopHeight());

    // Figure our the smallest zoom level we can use
    uInt32 firstmode = 1;
    if(myOSystem->desktopWidth() < 640 || myOSystem->desktopHeight() < 480)
      firstmode = 0;

    for(uInt32 i = firstmode; i < GFX_NumModes; ++i)
    {
      uInt32 zoom = ourGraphicsModes[i].zoom;
      if(zoom <= max_zoom)
      {
        VideoMode m;
        m.image_x = m.image_y = 0;
        m.image_w = m.screen_w = baseWidth * zoom;
        m.image_h = m.screen_h = baseHeight * zoom;
        m.gfxmode = ourGraphicsModes[i];

        addVidMode(m);
      }
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::addVidMode(VideoMode& mode)
{
  // The are minimum size requirements on a screen, no matter is in fullscreen
  // or windowed mode
  // Various part of the UI system depend on having at least 320x240 pixels
  // available, so we must enforce it here

  // Windowed modes can be sized exactly as required, since there's normally
  // no restriction on window size (between the minimum and maximum size)
  mode.screen_w = BSPF_max(mode.screen_w, 320u);
  mode.screen_h = BSPF_max(mode.screen_h, 240u);
  mode.image_x = (mode.screen_w - mode.image_w) >> 1;
  mode.image_y = (mode.screen_h - mode.image_h) >> 1;
  myWindowedModeList.add(mode);

#if 0 //FIXSDL
  // There are often stricter requirements on fullscreen modes, and they're
  // normally different depending on the OSystem in use
  // As well, we usually can't get fullscreen modes in the exact size
  // we want, so we need to calculate image offsets
  for(uInt32 i = 0; i < myResolutions.size(); ++i)
  {
    if(mode.screen_w <= myResolutions[i].width &&
       mode.screen_h <= myResolutions[i].height)
    {
      // Auto-calculate 'smart' centering; platform-specific framebuffers are
      // free to ignore or augment it
      mode.screen_w = BSPF_max(myResolutions[i].width, 320u);
      mode.screen_h = BSPF_max(myResolutions[i].height, 240u);
      mode.image_x = (mode.screen_w - mode.image_w) >> 1;
      mode.image_y = (mode.screen_h - mode.image_h) >> 1;
      break;
    }
  }
#endif
  myFullscreenModeList.add(mode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBuffer::VideoMode FrameBuffer::getSavedVidMode()
{
  EventHandler::State state = myOSystem->eventHandler().state();

  if(fullScreen())
    myCurrentModeList = &myFullscreenModeList;
  else
    myCurrentModeList = &myWindowedModeList;

  // Now select the best resolution depending on the state
  // UI modes (launcher and debugger) have only one supported resolution
  // so the 'current' one is the only valid one
  if(state == EventHandler::S_DEBUGGER || state == EventHandler::S_LAUNCHER)
  {
    myCurrentModeList->setByGfxMode(GFX_Zoom1x);
  }
  else
  {
    const string& name = myOSystem->settings().getString("tia.filter");
    myCurrentModeList->setByGfxMode(name);
  }

  return myCurrentModeList->current(myOSystem->settings(), fullScreen());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBuffer::VideoModeList::VideoModeList()
  : myIdx(-1)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBuffer::VideoModeList::~VideoModeList()
{
  clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::add(VideoMode mode)
{
  myModeList.push_back(mode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::clear()
{
  myModeList.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FrameBuffer::VideoModeList::isEmpty() const
{
  return myModeList.isEmpty();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt32 FrameBuffer::VideoModeList::size() const
{
  return myModeList.size();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::previous()
{
  --myIdx;
  if(myIdx < 0) myIdx = myModeList.size() - 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const FrameBuffer::VideoMode FrameBuffer::
  VideoModeList::current(const Settings& settings, bool isFullscreen) const
{
  // Fullscreen modes are related to the 'fullres' setting
  //   If it's 'auto', we just use the mode as already previously defined
  //   If it's not 'auto', attempt to fit the mode into the resolution
  //   specified by 'fullres' (if possible)
  if(isFullscreen && !BSPF_equalsIgnoreCase(settings.getString("fullres"), "auto"))
  {
    // Only use 'fullres' if it's *bigger* than the requested mode
    const GUI::Size& s = settings.getSize("fullres");

    if(s.w != -1 && s.h != -1 && (uInt32)s.w >= myModeList[myIdx].screen_w &&
      (uInt32)s.h >= myModeList[myIdx].screen_h)
    {
      VideoMode mode = myModeList[myIdx];
      mode.screen_w = s.w;
      mode.screen_h = s.h;
      mode.image_x = (mode.screen_w - mode.image_w) >> 1;
      mode.image_y = (mode.screen_h - mode.image_h) >> 1;

      return mode;
    }
  }

  // Otherwise, we just use the mode has it was defined in ::addVidMode()
  return myModeList[myIdx];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::next()
{
  myIdx = (myIdx + 1) % myModeList.size();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::setByGfxMode(GfxID id)
{
  // First we determine which graphics mode is being requested
  bool found = false;
  GraphicsMode gfxmode;
  for(uInt32 i = 0; i < GFX_NumModes; ++i)
  {
    if(ourGraphicsModes[i].type == id)
    {
      gfxmode = ourGraphicsModes[i];
      found = true;
      break;
    }
  }
  if(!found) gfxmode = ourGraphicsModes[0];

  // Now we scan the list for the applicable video mode
  set(gfxmode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::setByGfxMode(const string& name)
{
  // First we determine which graphics mode is being requested
  bool found = false;
  GraphicsMode gfxmode;
  for(uInt32 i = 0; i < GFX_NumModes; ++i)
  {
    if(BSPF_equalsIgnoreCase(ourGraphicsModes[i].name, name) ||
       BSPF_equalsIgnoreCase(ourGraphicsModes[i].description, name))
    {
      gfxmode = ourGraphicsModes[i];
      found = true;
      break;
    }
  }
  if(!found) gfxmode = ourGraphicsModes[0];

  // Now we scan the list for the applicable video mode
  set(gfxmode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::set(const GraphicsMode& gfxmode)
{
  // Attempt to point the current mode to the one given
  myIdx = -1;

  // First search for the given gfx id
  for(unsigned int i = 0; i < myModeList.size(); ++i)
  {
    if(myModeList[i].gfxmode.type == gfxmode.type)
    {
      myIdx = i;
      return;
    }
  }

  // If we get here, then the gfx type couldn't be found, so we search
  // for the first mode with the same zoomlevel (making sure that the
  // requested mode can fit inside the current screen)
  if(gfxmode.zoom > myModeList[myModeList.size()-1].gfxmode.zoom)
  {
    myIdx = myModeList.size()-1;
    return;
  }
  else
  {
    for(unsigned int i = 0; i < myModeList.size(); ++i)
    {
      if(myModeList[i].gfxmode.zoom == gfxmode.zoom)
      {
        myIdx = i;
        return;
      }
    }
  }

  // Finally, just pick the lowest video mode
  myIdx = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::print()
{
  cerr << "VideoModeList: " << endl << endl;
  for(Common::Array<VideoMode>::const_iterator i = myModeList.begin();
      i != myModeList.end(); ++i)
  {
    cerr << "  Mode " << i << endl;
    print(*i);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FrameBuffer::VideoModeList::print(const VideoMode& mode)
{
  cerr << "    screen w = " << mode.screen_w << endl
       << "    screen h = " << mode.screen_h << endl
       << "    image x  = " << mode.image_x << endl
       << "    image y  = " << mode.image_y << endl
       << "    image w  = " << mode.image_w << endl
       << "    image h  = " << mode.image_h << endl
       << "    gfx id   = " << mode.gfxmode.type << endl
       << "    gfx name = " << mode.gfxmode.name << endl
       << "    gfx desc = " << mode.gfxmode.description << endl
       << "    gfx zoom = " << mode.gfxmode.zoom << endl
       << endl;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FBSurface::box(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
                    uInt32 colorA, uInt32 colorB)
{
  hLine(x + 1, y,     x + w - 2, colorA);
  hLine(x,     y + 1, x + w - 1, colorA);
  vLine(x,     y + 1, y + h - 2, colorA);
  vLine(x + 1, y,     y + h - 1, colorA);

  hLine(x + 1,     y + h - 2, x + w - 1, colorB);
  hLine(x + 1,     y + h - 1, x + w - 2, colorB);
  vLine(x + w - 1, y + 1,     y + h - 2, colorB);
  vLine(x + w - 2, y + 1,     y + h - 1, colorB);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FBSurface::frameRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
                          uInt32 color, FrameStyle style)
{
  switch(style)
  {
    case kSolidLine:
      hLine(x,         y,         x + w - 1, color);
      hLine(x,         y + h - 1, x + w - 1, color);
      vLine(x,         y,         y + h - 1, color);
      vLine(x + w - 1, y,         y + h - 1, color);
      break;

    case kDashLine:
      unsigned int i, skip, lwidth = 1;

      for(i = x, skip = 1; i < x+w-1; i=i+lwidth+1, ++skip)
      {
        if(skip % 2)
        {
          hLine(i, y,         i + lwidth, color);
          hLine(i, y + h - 1, i + lwidth, color);
        }
      }
      for(i = y, skip = 1; i < y+h-1; i=i+lwidth+1, ++skip)
      {
        if(skip % 2)
        {
          vLine(x,         i, i + lwidth, color);
          vLine(x + w - 1, i, i + lwidth, color);
        }
      }
      break;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FBSurface::drawString(const GUI::Font& font, const string& s,
                           int x, int y, int w,
                           uInt32 color, TextAlignment align,
                           int deltax, bool useEllipsis)
{
  const int leftX = x, rightX = x + w;
  unsigned int i;
  int width = font.getStringWidth(s);
  string str;
	
  if(useEllipsis && width > w)
  {
    // String is too wide. So we shorten it "intelligently", by replacing
    // parts of it by an ellipsis ("..."). There are three possibilities
    // for this: replace the start, the end, or the middle of the string.
    // What is best really depends on the context; but unless we want to
    // make this configurable, replacing the middle probably is a good
    // compromise.
    const int ellipsisWidth = font.getStringWidth("...");
		
    // SLOW algorithm to remove enough of the middle. But it is good enough for now.
    const int halfWidth = (w - ellipsisWidth) / 2;
    int w2 = 0;
		
    for(i = 0; i < s.size(); ++i)
    {
      int charWidth = font.getCharWidth(s[i]);
      if(w2 + charWidth > halfWidth)
        break;

      w2 += charWidth;
      str += s[i];
    }

    // At this point we know that the first 'i' chars are together 'w2'
    // pixels wide. We took the first i-1, and add "..." to them.
    str += "...";
		
    // The original string is width wide. Of those we already skipped past
    // w2 pixels, which means (width - w2) remain.
    // The new str is (w2+ellipsisWidth) wide, so we can accomodate about
    // (w - (w2+ellipsisWidth)) more pixels.
    // Thus we skip ((width - w2) - (w - (w2+ellipsisWidth))) =
    // (width + ellipsisWidth - w)
    int skip = width + ellipsisWidth - w;
    for(; i < s.size() && skip > 0; ++i)
      skip -= font.getCharWidth(s[i]);

    // Append the remaining chars, if any
    for(; i < s.size(); ++i)
      str += s[i];

    width = font.getStringWidth(str);
  }
  else
    str = s;

  if(align == kTextAlignCenter)
    x = x + (w - width - 1)/2;
  else if(align == kTextAlignRight)
    x = x + w - width;

  x += deltax;
  for(i = 0; i < str.size(); ++i)
  {
    w = font.getCharWidth(str[i]);
    if(x+w > rightX)
      break;
    if(x >= leftX)
      drawChar(font, str[i], x, y, color);

    x += w;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FrameBuffer::GraphicsMode FrameBuffer::ourGraphicsModes[GFX_NumModes] = {
  { GFX_Zoom1x,  "zoom1x",  "Zoom 1x",  1  },
  { GFX_Zoom2x,  "zoom2x",  "Zoom 2x",  2  },
  { GFX_Zoom3x,  "zoom3x",  "Zoom 3x",  3  },
  { GFX_Zoom4x,  "zoom4x",  "Zoom 4x",  4  },
  { GFX_Zoom5x,  "zoom5x",  "Zoom 5x",  5  },
  { GFX_Zoom6x,  "zoom6x",  "Zoom 6x",  6  },
  { GFX_Zoom7x,  "zoom7x",  "Zoom 7x",  7  },
  { GFX_Zoom8x,  "zoom8x",  "Zoom 8x",  8  },
  { GFX_Zoom9x,  "zoom9x",  "Zoom 9x",  9  },
  { GFX_Zoom10x, "zoom10x", "Zoom 10x", 10 }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
  Palette is defined as follows:
    // Base colors
    kColor            Normal foreground color (non-text)
    kBGColor          Normal background color (non-text)
    kShadowColor      Item is disabled
    kTextColor        Normal text color
    kTextColorHi      Highlighted text color
    kTextColorEm      Emphasized text color

    // UI elements (dialog and widgets)
    kDlgColor         Dialog background
    kWidColor         Widget background
    kWidFrameColor    Border for currently selected widget

    // Button colors
    kBtnColor         Normal button background
    kBtnColorHi       Highlighted button background
    kBtnTextColor     Normal button font color
    kBtnTextColorHi   Highlighted button font color

    // Checkbox colors
    kCheckColor       Color of 'X' in checkbox

    // Scrollbar colors
    kScrollColor      Normal scrollbar color
    kScrollColorHi    Highlighted scrollbar color

    // Debugger colors
    kDbgChangedColor      Background color for changed cells
    kDbgChangedTextColor  Text color for changed cells
    kDbgColorHi           Highlighted color in debugger data cells
*/
uInt32 FrameBuffer::ourGUIColors[kNumColors-256] = {
  0x686868, 0x000000, 0x404040, 0x000000, 0x62a108, 0x9f0000,
  0xc9af7c, 0xf0f0cf, 0xc80000,
  0xac3410, 0xd55941, 0xffffff, 0xffd652,
  0xac3410,
  0xac3410, 0xd55941,
  0xac3410, 0xd55941,
  0xc80000, 0x00ff00, 0xc8c8ff
};
