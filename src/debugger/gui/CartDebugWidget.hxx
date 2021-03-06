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

#ifndef CART_DEBUG_WIDGET_HXX
#define CART_DEBUG_WIDGET_HXX

class GuiObject;
class ButtonWidget;

#include "Base.hxx"
#include "Font.hxx"
#include "Command.hxx"
#include "Debugger.hxx"
#include "RomWidget.hxx"
#include "Widget.hxx"
#include "EditTextWidget.hxx"
#include "StringListWidget.hxx"
#include "StringParser.hxx"

class CartDebugWidget : public Widget, public CommandSender
{
  public:
    CartDebugWidget(GuiObject* boss, const GUI::Font& lfont,
                    const GUI::Font& nfont,
                    int x, int y, int w, int h)
      : Widget(boss, lfont, x, y, w, h),
        CommandSender(boss),
        _nfont(nfont),
        myFontWidth(lfont.getMaxCharWidth()),
        myFontHeight(lfont.getFontHeight()),
        myLineHeight(lfont.getLineHeight()),
        myButtonHeight(myLineHeight + 4),
        myDesc(nullptr)   { }

    virtual ~CartDebugWidget() = default;

  public:
    int addBaseInformation(int bytes, const string& manufacturer,
        const string& desc, const uInt16 maxlines = 10)
    {
      const int lwidth = _font.getStringWidth("Manufacturer: "),
                fwidth = _w - lwidth - 20;
      EditTextWidget* w = nullptr;
      ostringstream buf;

      int x = 10, y = 10;

      // Add ROM size, manufacturer and bankswitch info
      new StaticTextWidget(_boss, _font, x, y, lwidth,
            myFontHeight, "ROM Size: ", kTextAlignLeft);
      buf << bytes << " bytes";
      if(bytes >= 1024)
        buf << " / " << (bytes/1024) << "KB";

      w = new EditTextWidget(_boss, _nfont, x+lwidth, y,
            fwidth, myLineHeight, buf.str());
      w->setEditable(false);
      y += myLineHeight + 4;

      new StaticTextWidget(_boss, _font, x, y, lwidth,
            myFontHeight, "Manufacturer: ", kTextAlignLeft);
      w = new EditTextWidget(_boss, _nfont, x+lwidth, y,
            fwidth, myLineHeight, manufacturer);
      w->setEditable(false);
      y += myLineHeight + 4;

      StringParser bs(desc, (fwidth - kScrollBarWidth) / myFontWidth - 4);
      const StringList& sl = bs.stringList();
      uInt32 lines = uInt32(sl.size());
      if(lines < 3) lines = 3;
      if(lines > maxlines) lines = maxlines;

      new StaticTextWidget(_boss, _font, x, y, lwidth,
            myFontHeight, "Description: ", kTextAlignLeft);
      myDesc = new StringListWidget(_boss, _nfont, x+lwidth, y,
                   fwidth, lines * myLineHeight, false);
      myDesc->setEditable(false);
      myDesc->setList(sl);
      addFocusWidget(myDesc);
      y += myDesc->getHeight() + 4;

      return y;
    }

    // Inform the ROM Widget that the underlying cart has somehow changed
    void invalidate()
    {
      sendCommand(RomWidget::kInvalidateListing, -1, -1);
    }

    // Some carts need to save old state in the debugger, so that we can
    // implement change tracking; most carts probably won't do anything here
    virtual void saveOldState() { }

    virtual void loadConfig() override { myDesc->setSelected(0); }
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id) override { }

    // Query internal state of the cart (usually just bankswitching info)
    virtual string bankState() { return "0 (non-bankswitched)"; }

    // To make the Cartridge RAM show up in the debugger, implement
    // the following 8 functions for cartridges with internal RAM
    virtual uInt32 internalRamSize() { return 0; }
    virtual uInt32 internalRamRPort(int start) { return 0; }
    virtual string internalRamDescription() { return EmptyString; }
    virtual const ByteArray& internalRamOld(int start, int count) { return myRamOld; }
    virtual const ByteArray& internalRamCurrent(int start, int count) { return myRamCurrent; }
    virtual void internalRamSetValue(int addr, uInt8 value) { }
    virtual uInt8 internalRamGetValue(int addr) { return 0; }
    virtual string internalRamLabel(int addr) { return "Not available/applicable"; }

  protected:
    // Arrays used to hold current and previous internal RAM values
    ByteArray myRamOld, myRamCurrent;

    // Font used for 'normal' text; _font is for 'label' text
    const GUI::Font& _nfont;

    // These will be needed by most of the child classes;
    // we may as well make them protected variables
    int myFontWidth, myFontHeight, myLineHeight, myButtonHeight;

    ostringstream& buffer() { myBuffer.str(""); return myBuffer; }

  private:
    StringListWidget* myDesc;
    ostringstream myBuffer;

  private:
    // Following constructors and assignment operators not supported
    CartDebugWidget() = delete;
    CartDebugWidget(const CartDebugWidget&) = delete;
    CartDebugWidget(CartDebugWidget&&) = delete;
    CartDebugWidget& operator=(const CartDebugWidget&) = delete;
    CartDebugWidget& operator=(CartDebugWidget&&) = delete;
};

#endif
