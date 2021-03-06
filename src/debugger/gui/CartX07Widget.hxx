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

#ifndef CARTRIDGEX07_WIDGET_HXX
#define CARTRIDGEX07_WIDGET_HXX

class CartridgeX07;
class PopUpWidget;

#include "CartDebugWidget.hxx"

class CartridgeX07Widget : public CartDebugWidget
{
  public:
    CartridgeX07Widget(GuiObject* boss, const GUI::Font& lfont,
                       const GUI::Font& nfont,
                       int x, int y, int w, int h,
                       CartridgeX07& cart);
    virtual ~CartridgeX07Widget() = default;

  private:
    CartridgeX07& myCart;
    PopUpWidget* myBank;

    enum { kBankChanged = 'bkCH' };

  private:
    void loadConfig() override;
    void handleCommand(CommandSender* sender, int cmd, int data, int id) override;

    string bankState() override;

    // Following constructors and assignment operators not supported
    CartridgeX07Widget() = delete;
    CartridgeX07Widget(const CartridgeX07Widget&) = delete;
    CartridgeX07Widget(CartridgeX07Widget&&) = delete;
    CartridgeX07Widget& operator=(const CartridgeX07Widget&) = delete;
    CartridgeX07Widget& operator=(CartridgeX07Widget&&) = delete;
};

#endif
