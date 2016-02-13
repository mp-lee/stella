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

#ifndef CARTRIDGESB_WIDGET_HXX
#define CARTRIDGESB_WIDGET_HXX

class CartridgeSB;
class PopUpWidget;

#include "CartDebugWidget.hxx"

class CartridgeSBWidget : public CartDebugWidget
{
  public:
    CartridgeSBWidget(GuiObject* boss, const GUI::Font& lfont,
                      const GUI::Font& nfont,
                      int x, int y, int w, int h,
                      CartridgeSB& cart);
    virtual ~CartridgeSBWidget() = default;

  private:
    CartridgeSB& myCart;
    PopUpWidget* myBank;

    enum { kBankChanged = 'bkCH' };

  private:
    void loadConfig() override;
    void handleCommand(CommandSender* sender, int cmd, int data, int id) override;

    string bankState() override;

    // Following constructors and assignment operators not supported
    CartridgeSBWidget() = delete;
    CartridgeSBWidget(const CartridgeSBWidget&) = delete;
    CartridgeSBWidget(CartridgeSBWidget&&) = delete;
    CartridgeSBWidget& operator=(const CartridgeSBWidget&) = delete;
    CartridgeSBWidget& operator=(CartridgeSBWidget&&) = delete;
};

#endif