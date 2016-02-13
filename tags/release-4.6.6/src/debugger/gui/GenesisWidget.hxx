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
// Copyright (c) 1995-2015 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#ifndef GENESIS_WIDGET_HXX
#define GENESIS_WIDGET_HXX

#include "Control.hxx"
#include "Event.hxx"
#include "ControllerWidget.hxx"

class GenesisWidget : public ControllerWidget
{
  public:
    GenesisWidget(GuiObject* boss, const GUI::Font& font, int x, int y,
                  Controller& controller);
    virtual ~GenesisWidget();

  private:
    enum { kJUp = 0, kJDown, kJLeft, kJRight, kJBbtn, kJCbtn };

    CheckboxWidget* myPins[6];
    static Controller::DigitalPin ourPinNo[5];

  private:
    void loadConfig() override;
    void handleCommand(CommandSender* sender, int cmd, int data, int id) override;

    // Following constructors and assignment operators not supported
    GenesisWidget() = delete;
    GenesisWidget(const GenesisWidget&) = delete;
    GenesisWidget(GenesisWidget&&) = delete;
    GenesisWidget& operator=(const GenesisWidget&) = delete;
    GenesisWidget& operator=(GenesisWidget&&) = delete;
};

#endif