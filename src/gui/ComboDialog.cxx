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

#include <sstream>

#include "bspf.hxx"

#include "Control.hxx"
#include "Dialog.hxx"
#include "DialogContainer.hxx"
#include "EventHandler.hxx"
#include "OSystem.hxx"
#include "EditTextWidget.hxx"
#include "PopUpWidget.hxx"
#include "Widget.hxx"

#include "ComboDialog.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ComboDialog::ComboDialog(GuiObject* boss, const GUI::Font& font,
                         const VariantList& combolist)
  : Dialog(boss->instance(), boss->parent()),
    myComboEvent(Event::NoType)
{
  const int lineHeight   = font.getLineHeight(),
            fontWidth    = font.getMaxCharWidth(),
            fontHeight   = font.getFontHeight(),
            buttonWidth  = font.getStringWidth("Defaults") + 20,
            buttonHeight = font.getLineHeight() + 4;
  int xpos, ypos;
  WidgetArray wid;

  // Set real dimensions
  _w = 35 * fontWidth + 10;
  _h = 11 * (lineHeight + 4) + 10;
  xpos = ypos = 5;

  // Get maximum width of popupwidget
  int pwidth = 0;
  for(const auto& s: combolist)
    pwidth = BSPF_max(font.getStringWidth(s.first), pwidth);

  // Label for dialog, indicating which combo is being changed
  myComboName = new StaticTextWidget(this, font, xpos, ypos, _w - xpos - 10,
                                     fontHeight, "", kTextAlignCenter);
  ypos += (lineHeight + 4) + 5;

  // Add event popup for 8 events
  auto ADD_EVENT_POPUP = [&](int idx, const string& label)
  {
    myEvents[idx] = new PopUpWidget(this, font, xpos, ypos,
                        pwidth, lineHeight, combolist, label);
    wid.push_back(myEvents[idx]);
    ypos += lineHeight + 4;
  };

  xpos = 10;
  myEvents[0] = nullptr;  ADD_EVENT_POPUP(0, "Event 1: ");
  myEvents[1] = nullptr;  ADD_EVENT_POPUP(1, "Event 2: ");
  myEvents[2] = nullptr;  ADD_EVENT_POPUP(2, "Event 3: ");
  myEvents[3] = nullptr;  ADD_EVENT_POPUP(3, "Event 4: ");
  myEvents[4] = nullptr;  ADD_EVENT_POPUP(4, "Event 5: ");
  myEvents[5] = nullptr;  ADD_EVENT_POPUP(5, "Event 6: ");
  myEvents[6] = nullptr;  ADD_EVENT_POPUP(6, "Event 7: ");
  myEvents[7] = nullptr;  ADD_EVENT_POPUP(7, "Event 8: ");

  // Add Defaults, OK and Cancel buttons
  ButtonWidget* b;
  b = new ButtonWidget(this, font, 10, _h - buttonHeight - 10,
                       buttonWidth, buttonHeight, "Defaults", kDefaultsCmd);
  wid.push_back(b);
  addOKCancelBGroup(wid, font);

  addToFocusList(wid);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ComboDialog::show(Event::Type event, const string& name)
{
  // Make sure the event is allowed
  if(event >= Event::Combo1 && event <= Event::Combo16)
  {
    myComboEvent = event;
    myComboName->setLabel("Add events for " + name);
    open();
  }
  else
    myComboEvent = Event::NoType;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ComboDialog::loadConfig()
{
  StringList events = instance().eventHandler().getComboListForEvent(myComboEvent);

  uInt32 size = BSPF_min(uInt32(events.size()), 8u);
  for(uInt32 i = 0; i < size; ++i)
    myEvents[i]->setSelected("", events[i]);

  // Fill any remaining items to 'None'
  if(size < 8)
    for(uInt32 i = size; i < 8; ++i)
      myEvents[i]->setSelected("None", "-1");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ComboDialog::saveConfig()
{
  StringList events;
  for(int i = 0; i < 8; ++i)
    events.push_back(myEvents[i]->getSelectedTag().toString());

  instance().eventHandler().setComboListForEvent(myComboEvent, events);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ComboDialog::setDefaults()
{
  for(int i = 0; i < 8; ++i)
    myEvents[i]->setSelected("None", "-1");

  _dirty = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ComboDialog::handleCommand(CommandSender* sender, int cmd,
                                int data, int id)
{
  switch(cmd)
  {
    case kOKCmd:
      saveConfig();
      close();
      break;

    case kDefaultsCmd:
      setDefaults();
      break;

    default:
      Dialog::handleCommand(sender, cmd, data, 0);
      break;
  }
}
