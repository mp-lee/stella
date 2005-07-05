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
// $Id: CheatWidget.hxx,v 1.6 2005-07-05 15:25:44 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef CHEAT_WIDGET_HXX
#define CHEAT_WIDGET_HXX

class GuiObject;
class ButtonWidget;
class StaticTextWidget;
class EditNumWidget;
class AddrValueWidget;

#include "Array.hxx"
#include "Widget.hxx"
#include "Command.hxx"


class CheatWidget : public Widget, public CommandSender
{
  private:
    struct AddrValue {
      uInt16 addr;
      uInt8 value;
    };

    typedef GUI::Array<AddrValue> AddrValueList;

    AddrValueList mySearchArray;
    AddrValueList myCompareArray;

  public:
    CheatWidget(GuiObject *boss, int x, int y, int w, int h);
    virtual ~CheatWidget();

    Widget* activeWidget() { return myActiveWidget; }

    void handleCommand(CommandSender* sender, int cmd, int data, int id);

  private:
    void doSearch();
    void doCompare();
    void doRestart();

    void fillResultsList();

  private:
    Widget* myActiveWidget;

    EditNumWidget* myEditBox;
    StaticTextWidget* myResult;

    ButtonWidget* mySearchButton;
    ButtonWidget* myCompareButton;
    ButtonWidget* myRestartButton;

    AddrValueWidget* myResultsList;
};

#endif
