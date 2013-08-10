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
// Copyright (c) 1995-2013 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id$
//============================================================================

#include "Base.hxx"
#include "OSystem.hxx"
#include "FrameBuffer.hxx"
#include "Debugger.hxx"
#include "TIADebug.hxx"
#include "Widget.hxx"
#include "EditTextWidget.hxx"
#include "GuiObject.hxx"

#include "TiaInfoWidget.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TiaInfoWidget::TiaInfoWidget(GuiObject* boss, const GUI::Font& font,
                             int x, int y)
  : Widget(boss, font, x, y, 16, 16),
    CommandSender(boss)
{
  x += 5;
  const int lineHeight = font.getLineHeight();
  int xpos = x, ypos = y, lwidth = font.getStringWidth("F. Cyc:");

  // Add frame info
  xpos = x;  ypos = y + 10;
  new StaticTextWidget(boss, font, xpos, ypos, lwidth, lineHeight,
                       "Frame:", kTextAlignLeft);
  xpos += lwidth;
  myFrameCount = new EditTextWidget(boss, font, xpos, ypos-1, 45, lineHeight, "");
  myFrameCount->setEditable(false);

  xpos = x;  ypos += lineHeight + 5;
  new StaticTextWidget(boss, font, xpos, ypos, lwidth, lineHeight,
                       "F. Cyc:", kTextAlignLeft);
  xpos += lwidth;
  myFrameCycles = new EditTextWidget(boss, font, xpos, ypos-1, 45, lineHeight, "");
  myFrameCycles->setEditable(false);

  xpos = x + 10;  ypos += lineHeight + 8;
  myVSync = new CheckboxWidget(boss, font, xpos, ypos-3, "VSync", 0);
  myVSync->setEditable(false);

  xpos = x + 10;  ypos += lineHeight + 5;
  myVBlank = new CheckboxWidget(boss, font, xpos, ypos-3, "VBlank", 0);
  myVBlank->setEditable(false);

  xpos = x + lwidth + myFrameCycles->getWidth() + 5;  ypos = y + 10;
  lwidth = font.getStringWidth("Pixel Pos:");
  new StaticTextWidget(boss, font, xpos, ypos, lwidth, lineHeight,
                       "Scanline:", kTextAlignLeft);

  myScanlineCount = new EditTextWidget(boss, font, xpos+lwidth, ypos-1, 30,
                                       lineHeight, "");
  myScanlineCount->setEditable(false);

  ypos += lineHeight + 5;
  new StaticTextWidget(boss, font, xpos, ypos, lwidth, lineHeight,
                       "S. Cyc:", kTextAlignLeft);

  myScanlineCycles = new EditTextWidget(boss, font, xpos+lwidth, ypos-1, 30,
                                        lineHeight, "");
  myScanlineCycles->setEditable(false);

  ypos += lineHeight + 5;
  new StaticTextWidget(boss, font, xpos, ypos, lwidth, lineHeight,
                       "Pixel Pos:", kTextAlignLeft);

  myPixelPosition = new EditTextWidget(boss, font, xpos+lwidth, ypos-1, 30,
                                       lineHeight, "");
  myPixelPosition->setEditable(false);

  ypos += lineHeight + 5;
  new StaticTextWidget(boss, font, xpos, ypos, lwidth, lineHeight,
                       "Color Clk:", kTextAlignLeft);

  myColorClocks = new EditTextWidget(boss, font, xpos+lwidth, ypos-1, 30,
                                     lineHeight, "");
  myColorClocks->setEditable(false);

  // Calculate actual dimensions
  _w = 100 + 30 + lwidth;
  _h = ypos + lineHeight;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TiaInfoWidget::~TiaInfoWidget()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TiaInfoWidget::handleMouseDown(int x, int y, int button, int clickCount)
{
//cerr << "TiaInfoWidget button press: x = " << x << ", y = " << y << endl;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TiaInfoWidget::handleCommand(CommandSender* sender, int cmd, int data, int id)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TiaInfoWidget::loadConfig()
{
  Debugger& dbg = instance().debugger();
  TIADebug& tia = dbg.tiaDebug();

  myFrameCount->setText(Common::Base::toString(tia.frameCount(), Common::Base::F_10));
  myFrameCycles->setText(Common::Base::toString(dbg.cycles(), Common::Base::F_10));

  myVSync->setState(tia.vsync());
  myVBlank->setState(tia.vblank());

  int clk = tia.clocksThisLine();
  myScanlineCount->setText(Common::Base::toString(tia.scanlines(), Common::Base::F_10));
  myScanlineCycles->setText(Common::Base::toString(clk/3, Common::Base::F_10));
  myPixelPosition->setText(Common::Base::toString(clk-68, Common::Base::F_10));
  myColorClocks->setText(Common::Base::toString(clk, Common::Base::F_10));
}
