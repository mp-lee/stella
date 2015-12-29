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
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef GUI_OBJECT_HXX
#define GUI_OBJECT_HXX

class Dialog;
class DialogContainer;
class Widget;

#include "Command.hxx"
#include "OSystem.hxx"
#include "Vec.hxx"

using WidgetArray = vector<Widget*>;

// The commands generated by various widgets
enum {
  kOKCmd           = 'OK  ',
  kCloseCmd        = 'CLOS',
  kNextCmd         = 'NEXT',
  kPrevCmd         = 'PREV',
  kEditCmd         = 'EDIT',
  kDefaultsCmd     = 'DEFA',
  kSetPositionCmd  = 'SETP',
  kTabChangedCmd   = 'TBCH',
  kCheckActionCmd  = 'CBAC',
  kRefreshAllCmd   = 'REFA'
};

/**
  This is the base class for all GUI objects/widgets.
  
  @author  Stephen Anthony
  @version $Id$
*/
class GuiObject : public CommandReceiver
{
  friend class Widget;
  friend class DialogContainer;

  public:
    GuiObject(OSystem& osystem, DialogContainer& parent, Dialog& dialog,
              int x, int y, int w, int h)
      : myOSystem(osystem),
        myParent(parent),
        myDialog(dialog),
        _x(x), _y(y), _w(w), _h(h),
        _dirty(false),
        _firstWidget(0) { }

    virtual ~GuiObject() = default;

    OSystem& instance() const       { return myOSystem; }
    DialogContainer& parent() const { return myParent;  }
    Dialog& dialog() const          { return myDialog;  }

    void setDirty() { _dirty = true; }

    virtual int getAbsX() const     { return _x; }
    virtual int getAbsY() const     { return _y; }
    virtual int getChildX() const   { return getAbsX(); }
    virtual int getChildY() const   { return getAbsY(); }
    virtual int getWidth() const    { return _w; }
    virtual int getHeight() const   { return _h; }

    virtual void setWidth(int w)    { _w = w; }
    virtual void setHeight(int h)   { _h = h; }

    virtual bool isVisible() const = 0;

    /** Add given widget(s) to the focus list */
    virtual void addFocusWidget(Widget* w) = 0;
    virtual void addToFocusList(WidgetArray& list) = 0;

    /** Return focus list for this object */
    WidgetArray& getFocusList() { return _focusList; }

    /** Redraw the focus list */
    virtual void redrawFocus() { }

  protected:
    virtual void releaseFocus() = 0;
    virtual void draw() = 0;

  private:
    OSystem&         myOSystem;
    DialogContainer& myParent;
    Dialog&          myDialog;

  protected:
    int _x, _y, _w, _h;
    bool _dirty;

    Widget* _firstWidget;
    WidgetArray _focusList;

  private:
    // Following constructors and assignment operators not supported
    GuiObject() = delete;
    GuiObject(const GuiObject&) = delete;
    GuiObject(GuiObject&&) = delete;
    GuiObject& operator=(const GuiObject&) = delete;
    GuiObject& operator=(GuiObject&&) = delete;
};

#endif
