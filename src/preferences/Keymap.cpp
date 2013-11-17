/*
   	Copyright (c) 2003, Xentronix
	Author: Frans van Nispen (frans@xentronix.com)
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:
	
	Redistributions of source code must retain the above copyright notice, this list
	of conditions and the following disclaimer. Redistributions in binary form must
	reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution. 
	
	Neither the name of Xentronix nor the names of its contributors may be used
	to endorse or promote products derived from this software without specific prior
	written permission. 
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * Copyright 2013 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <LayoutBuilder.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <Path.h>
#include <TranslationKit.h>
#include <TranslationUtils.h>

#include "FaberDefs.h"
#include "Keymap.h"
#include "FStringItem.h"

#define SELECT		'selK'

FaberShortcut gKeyBind = *FaberShortcut::Get();

static const char* _KeyLabel(char in)
{
	const char* key;

	switch(in)
	{
			case ' ':	key = "   ";	break;
			case 1:		key = "Home";	break;
			case 4:		key = "End";	break;
			case 5:		key = "Ins";	break;
			case 8:		key = "BS ";	break;
			case 9:		key = "Tab";	break;
			case 10:	key = "Return";	break;
			case 11:	key = "PgUp";	break;
			case 12:	key = "PgDown";	break;
			case 14:	key = "F1";		break;
			case 15:	key = "F2";		break;
			case 16:	key = "F3";		break;
			case 17:	key = "F4";		break;
			case 18:	key = "F5";		break;
			case 19:	key = "F6";		break;
			case 20:	key = "F7";		break;
			case 21:	key = "F8";		break;
			case 22:	key = "F9";		break;
			case 23:	key = "F10";	break;
			case 24:	key = "F11";	break;
			case 25:	key = "F12";	break;
			case 27:	key = "Esc";	break;
			case 28:	key = "Left";	break;
			case 29:	key = "Right";	break;
			case 30:	key = "Up";		break;
			case 31:	key = "Down";	break;
			case 127:	key = "Del";	break;
	
			default:
				key = "???";
				break;
		}
		return key;
}


// Item

class KeyItem : public StringItem
{
public:
							KeyItem(const char *ID, char key,
								int32 mod, char key2, int32 mod2, int32 id);

							~KeyItem();
	
	virtual void			DrawItem(BView *owner, BRect rect, bool all);
			int32			GetCode();
	
			void			SetKey(char key);
			void			SetKeyAlt(char key);
			void			SetMod(int32 mod);
			void			SetModAlt(int32 mod);

private:
		  	void			DrawMods(BView *view, BRect r, int32 mod);
		  	void			DrawKey(BView *view, BRect r, const char *c);

			char			m_key, m_key2;
			int32			m_mod, m_mod2, m_id;
};


KeyItem::KeyItem(const char *ID, char key, int32 mod,
	char key2, int32 mod2, int32 id)
	:
	StringItem(ID),
	m_key(key),
	m_key2(key2),
	m_mod(mod),
	m_mod2(mod2),
	m_id(id)
{
}


KeyItem::~KeyItem()
{
}


int32
KeyItem::GetCode()
{
	return m_id;
}


void
KeyItem::SetKey(char key)
{
	m_key = key;
}


void
KeyItem::SetKeyAlt(char key)
{
	m_key2 = key;
}


void
KeyItem::SetMod(int32 mod)
{
	m_mod = mod;
}


void
KeyItem::SetModAlt(int32 mod)
{
	m_mod2 = mod;
}


void
KeyItem::DrawItem(BView *view, BRect rect, bool all)
{
	BFont font;
	view->GetFont(&font);

	if (m_id == -1) {				// Draw the Outline
		if (IsSelected())
			view->SetLowColor(150,190,230);
		else
			view->SetLowColor(255,255,255);

		view->FillRect(rect, B_SOLID_LOW);
		view->SetHighColor(0,0,0);
		view->SetFont(be_bold_font);
		view->DrawString( Label(), BPoint( rect.left +5, rect.top +font.Size() ));
		view->SetFont(&font);
		return;
	}

	const char *key;

	StringItem::DrawItem(view, rect, all);

	view->SetHighColor(240,240,240);
	view->StrokeLine( BPoint(rect.left, rect.bottom), BPoint(rect.right, rect.bottom));
	view->SetHighColor(0,0,0);

	float x = rect.left + rect.Width()/2 +(font.Size()+4)*3;	// max 3 combinations

	DrawMods(view, BRect( x - font.Size() - 6, rect.top+1, x -4, rect.bottom-1), m_mod);

	if (m_key >' ' && m_key < 'a') {
		view->DrawChar( m_key, BPoint( x, rect.top +font.Size() ));
	} else if (m_key) {
		key = _KeyLabel(m_key);
		DrawKey(view, BRect(x-2, rect.top+1, x+font.StringWidth(key)+2, rect.bottom-1), key);
	}

	x = rect.right - 32;

	DrawMods(view, BRect( x - font.Size() - 6, rect.top+1, x -4, rect.bottom-1), m_mod2);

	if (m_key2 >' ' && m_key2 < 'a') {
		view->DrawChar( m_key2, BPoint( x, rect.top +font.Size() ));
	} else if (m_key2) {
		key = _KeyLabel(m_key2);
		DrawKey(view, BRect(x-2, rect.top+1, x+font.StringWidth(key)+2, rect.bottom-1), key);
	}
}

void
KeyItem::DrawMods(BView *view, BRect r, int32 mod)
{
	float w = -(r.Width()+2);

	if (mod & B_SHIFT_KEY) {
		DrawKey(view, r, "S");
		r.OffsetBy(w,0);
	}

	if (mod & B_CONTROL_KEY) {
		DrawKey(view, r, "C");
		r.OffsetBy(w,0);
	}

	if (mod & B_OPTION_KEY) {
		DrawKey(view, r, "O");
		r.OffsetBy(w,0);
	}

	if ((mod & B_COMMAND_KEY)) {
		DrawKey(view, r, "A");
		r.OffsetBy(w,0);
	}
}

void
KeyItem::DrawKey(BView *view, BRect r, const char *c)
{
	BFont font;
	view->GetFont(&font);

	view->SetHighColor(240,240,240);
	view->StrokeLine( BPoint( r.left, r.top ), BPoint( r.right-1, r.top ));
	view->StrokeLine( BPoint( r.left, r.top+1 ), BPoint( r.left, r.bottom ));
	view->SetHighColor(128,128,128);
	view->StrokeLine( BPoint( r.left+1, r.bottom ), BPoint( r.right, r.bottom ));
	view->StrokeLine( BPoint( r.right, r.bottom-1 ), BPoint( r.right, r.top ));
	view->SetHighColor(192,192,192);
	r.InsetBy(1,1);
	view->FillRect(r);
	view->SetHighColor(0,0,0);
	view->SetLowColor(192,192,192);
	view->DrawString(c, BPoint( (r.left+r.right)/2.0 - font.StringWidth(c)/2.0 +1, r.top+font.Size()-2) );

	if (IsSelected())
		view->SetLowColor(150,190,230);
	else
		view->SetLowColor(255,255,255);
}

// Dialog

class KeyControl : public BControl
{
public:
	KeyControl(BRect rect, const char *label, char key, int32 mod,
		bool menu, uint32 r = B_FOLLOW_ALL, uint32 m = B_WILL_DRAW | B_NAVIGABLE);

	~KeyControl();

	virtual void Draw(BRect);
	virtual void MouseDown(BPoint);
	virtual void KeyDown(const char *bytes, int32 numBytes);

	void SetDivider(float x);
	float Divider();
	
	char GetKey();
	int32 GetMod();
	void SetBinding(char c, int32 m);

private:

  	float DrawMods(BRect r, int32 mod);
  	void DrawKey(BRect r, const char *c);

	char m_key;
	int32 m_mod;
	bool m_menu;
	float m_div;
};


class SetKeyWindow : public BWindow
{
public:
						SetKeyWindow(BPoint p, int32 index, BView *v);
	virtual void		MessageReceived(BMessage*);
	virtual bool		QuitRequested();
   
private:
			int32		index;
			BView*		parent;
			char		key, key2;
			int32		mod, mod2;
			uint32		message;
			bool		menu;
			KeyControl*	control1, *control2;
};


KeyControl::KeyControl(BRect r, const char *label, char key,
	int32 mod, bool menu, uint32 rs, uint32 m)
	:
	BControl(r, NULL, label, NULL, rs, m),
	m_key(key),
	m_mod(mod),
	m_menu(menu)
{
}


KeyControl::~KeyControl()
{
}


void
KeyControl::KeyDown(const char *bytes, int32 numBytes)
{
	int32 key, mod, raw_key;
	if (numBytes == 1) {
		BMessage *msg = Window()->CurrentMessage();
		msg->FindInt32("key", &raw_key);
		msg->FindInt32("modifiers", &mod);
		msg->FindInt32("raw_char", &key);

		// now do some conversions for combinations
		if (key == B_FUNCTION_KEY) {
			key = 12+raw_key;
		} else if (key>='a' && key<='z')
			key -= ('a'-'A');
		
		mod = mod & (B_SHIFT_KEY | B_CONTROL_KEY | B_COMMAND_KEY | B_OPTION_KEY);		// mask left / right stuff
		if (m_menu)	mod = mod | B_COMMAND_KEY;
		
		if (!m_menu || (m_menu && key>=' ' && key<'z')) {
			m_mod = mod ;
			m_key = key;
			Invalidate();
		}
	}
}


void
KeyControl::MouseDown(BPoint p)
{
	MakeFocus(true);
}


void
KeyControl::SetDivider(float x)
{
	m_div = x;
}


float
KeyControl::Divider()
{
	return m_div;
}


int32
KeyControl::GetMod()
{
	return m_mod;
}


char
KeyControl::GetKey()
{
	return m_key;
}


void
KeyControl::SetBinding(char key, int32 mod)
{
	m_mod = mod;
	if (m_menu)	m_mod = m_mod | B_COMMAND_KEY;
	m_key = key;
	Invalidate();
}


void
KeyControl::Draw(BRect r)
{
	BFont font;
	GetFont(&font);
	font_height fh;
	font.GetHeight(&fh);
	float y = Bounds().bottom - ceil(fh.descent);

	const char *key;

	SetHighColor(0,0,0);
	DrawString( Label(), BPoint(2.0, y));

	r = Bounds();
	r.left = Divider();
	if (IsFocus())
		SetHighColor(ui_color(B_KEYBOARD_NAVIGATION_COLOR));
	else
		SetHighColor(0,0,0);

	StrokeRect(r);
	r.InsetBy(1,1);
	SetHighColor( 255,255,255 );
	FillRect(r);
	SetHighColor(0,0,0);
	r.InsetBy(1,1);

	float x = DrawMods(BRect( r.left, r.top+1, r.left +font.Size()+6, r.bottom-1), m_mod);
	if (m_key >' ' && m_key < 'a') {
		DrawChar( m_key, BPoint( x, r.bottom - ceil(fh.descent)));
	} else if (m_key) {
		key = _KeyLabel(m_key);
		DrawKey(BRect(x-2, r.top+1, x+font.StringWidth(key)+2, r.bottom-1), key);
	}
}

float
KeyControl::DrawMods(BRect r, int32 mod)
{
	float w = (r.Width()+2);
	if ((mod & B_COMMAND_KEY)) {
		DrawKey(r, "A");
		r.OffsetBy(w,0);
	}
	if (mod & B_OPTION_KEY) {
		DrawKey(r, "O");
		r.OffsetBy(w,0);
	}
	if (mod & B_CONTROL_KEY) {
		DrawKey(r, "C");
		r.OffsetBy(w,0);
	}
	if (mod & B_SHIFT_KEY) {
		DrawKey(r, "S");
		r.OffsetBy(w,0);
	}
	return r.left+4;
}


void
KeyControl::DrawKey(BRect r, const char *c)
{
	BFont font;
	GetFont(&font);

	SetHighColor(240,240,240);
	StrokeLine( BPoint( r.left, r.top ), BPoint( r.right-1, r.top ));
	StrokeLine( BPoint( r.left, r.top+1 ), BPoint( r.left, r.bottom ));
	SetHighColor(128,128,128);
	StrokeLine( BPoint( r.left+1, r.bottom ), BPoint( r.right, r.bottom ));
	StrokeLine( BPoint( r.right, r.bottom-1 ), BPoint( r.right, r.top ));
	SetHighColor(192,192,192);
	r.InsetBy(1,1);
	FillRect(r);
	SetHighColor(0,0,0);
	SetLowColor(192,192,192);
	DrawString(c, BPoint( (r.left+r.right)/2.0 - font.StringWidth(c)/2.0 +1, r.top+font.Size()) );
	SetLowColor(255,255,255);
}

#define SET				'setF'
#define CLEAR1			'clr1'
#define CLEAR2			'clr2'


SetKeyWindow::SetKeyWindow(BPoint p, int32 i, BView *v)
	:
	BWindow(BRect(p.x,p.y,p.x,p.y),
		B_TRANSLATE("Change Key Binding"), B_TITLED_WINDOW,
		B_NOT_RESIZABLE|B_NOT_ZOOMABLE), index(i), parent(v)
{
	BRect r(0,0,300,150);
	ResizeTo(r.Width(), r.Height());
	MoveBy(-r.Width()/2, -r.Height()/2);

	BView *view = new BView(r, NULL, B_FOLLOW_ALL, B_WILL_DRAW);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	r.InsetBy(8,8);
	r.bottom = r.top+19;
	BStringView *st;
	r.right = r.left + be_bold_font->StringWidth(B_TRANSLATE("Enter new bindings for:"));
	view->AddChild(st = new BStringView(r, NULL, B_TRANSLATE("Enter new bindings for:")));
	st->SetFont(be_bold_font);
	r.left = r.right+8;
	r.right = Bounds().right-8;

	uint32 code = gKeyBind.GetCode(index);
	view->AddChild(new BStringView(r, NULL, B_TRANSLATE(gKeyBind.GetLabel(code))));
	
	// request the installed message
	key = gKeyBind.GetKey(code);
	key2 = gKeyBind.GetKeyAlt(code);
	mod = gKeyBind.GetMod(code);
	mod2 = gKeyBind.GetModAlt(code);
	message = code;
	menu = gKeyBind.IsMenuItem(code);

	r.OffsetBy(0,30);
	r.left = 8;
	float x = 8 + MAX( be_plain_font->StringWidth(B_TRANSLATE("Primary")),
		be_plain_font->StringWidth(B_TRANSLATE("ALTERNATE")));
	
	r.right = Bounds().right -96;

	view->AddChild(control1 = new KeyControl(r, B_TRANSLATE("Primary"),
		key, mod, menu));

	control1->SetDivider(x);
	view->AddChild(new BButton(BRect(r.right+8, r.top, Bounds().right-8, r.bottom),
		NULL, B_TRANSLATE("CLEAR"), new BMessage(CLEAR1)) );

	r.OffsetBy(0,30);

	view->AddChild(control2 = new KeyControl(r, B_TRANSLATE("Alternate"), key2, mod2, false));

	control2->SetDivider(x);

	view->AddChild(new BButton(BRect(r.right+8, r.top, Bounds().right-8, r.bottom),
		NULL, B_TRANSLATE("CLEAR"), new BMessage(CLEAR2)) );

	r = Bounds();
	r.InsetBy(8,8);

	r.top = r.bottom - 23;
	r.left = r.right - 80;

	view->AddChild(new BButton(r, NULL, B_TRANSLATE("Apply"), new BMessage(SET)) );
	r.OffsetBy(-(r.Width()+8), 0);
	view->AddChild(new BButton(r, NULL, B_TRANSLATE("Cancel"), new BMessage(B_QUIT_REQUESTED)) );

	AddChild(view);
	Run();
	Show();
}


bool
SetKeyWindow::QuitRequested()
{
	return true;
}


void
SetKeyWindow::MessageReceived(BMessage* msg)
{
	switch(msg->what) {
		case SET:
		{/*
			KeyBind* key = new KeyBind();
	
			key->key = control1->GetKey();
			key->mod = control1->GetMod();
			key->altKey = control2->GetKey();
			key->altMod = control2->GetMod();
			key->label = gKeyBind.GetLabel(index);
			key->message->code = message;
			key->isMenuItem = menu;
	
			gKeyBind.AddKeyBind(key);
	
			if (parent->LockLooper()) {
				parent->Pulse();
				parent->UnlockLooper();
			}
			Quit();*/
			break;
		}

		case CLEAR1:
			mod = key = 0;
			control1->SetBinding(key,mod);
			break;
			
		case CLEAR2:
			mod2 = key2 = 0;
			control2->SetBinding(key2,mod2);
			break;
			
		default:
			BWindow::MessageReceived(msg);
	}
}


/*******************************************************
*   Setup the main view. Add in all the niffty components
*   we have made and get things rolling
*******************************************************/
KeymapView::KeymapView()
	:
	BView(BRect(0,0,490,380), "Prefs keys", B_FOLLOW_ALL, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// add the prefs list at the left
	list = new BOutlineListView("key list");

	BScrollView *sv = new BScrollView("scroll", list,
		B_FOLLOW_ALL_SIDES, false, true, B_PLAIN_BORDER);

	sv->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	sv->MakeFocus(false);

	BListItem *item = NULL;

	for (int32 i=0; i<gKeyBind.CountKeys(); i++) {
		uint32 code = gKeyBind.GetCode(i);
		if (code == FABER_SPLITTER || code == FABER_ITEM_END)
			continue;

		if (code == FABER_ITEM_START) {
			if (item)
				list->Collapse(item);

			list->AddItem(
				item = new KeyItem(gKeyBind.GetLabel(i), 0, 0, 0, 0, -1));
		} else {
			list->AddUnder(new KeyItem(gKeyBind.GetLabel(i),
				gKeyBind.GetKey(code),
				gKeyBind.GetMod(code), gKeyBind.GetKeyAlt(code),
				gKeyBind.GetModAlt(code), i), item);
		}
	}

	if (item)
		list->Collapse(item);

	m_index = -1;

	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(sv, 0)
	.End();
}


KeymapView::~KeymapView()
{
}


void
KeymapView::AttachedToWindow()
{
	list->SetTarget(this);
//	list->SetSelectionMessage(new BMessage(SELECT));
	list->SetInvocationMessage(new BMessage(SELECT));
}


void
KeymapView::Pulse()
{
	BListItem *item = NULL;
	if (m_index<0) {
		while (item = list->RemoveItem((int32)0))
			delete item;

		for (int32 i=0; i<gKeyBind.CountKeys(); i++) {
			uint32 code = gKeyBind.GetCode(i);

			if (code == FABER_ITEM_START) {
				if (item)
					list->Collapse(item);

				list->AddItem(item = new KeyItem(
					gKeyBind.GetLabel(code), 0, 0, 0, 0, -1));
			} else {
				list->AddUnder(new KeyItem(gKeyBind.GetLabel(code),
				gKeyBind.GetKey(code),
					gKeyBind.GetMod(code), gKeyBind.GetKeyAlt(code),
					gKeyBind.GetModAlt(code), i), item);
			}
		}
		if (item)
			list->Collapse(item);
	} else {
		item = list->FullListItemAt(m_index);
		if (item) {
			// needed to convert the outline numbers, they are inverted !
			uint32 code = ((KeyItem*)item)->GetCode();			
			if (code > 0) {
				((KeyItem*)item)->SetKey( gKeyBind.GetKey(code));
				((KeyItem*)item)->SetKeyAlt( gKeyBind.GetKeyAlt(code));
				((KeyItem*)item)->SetMod( gKeyBind.GetMod(code));
				((KeyItem*)item)->SetModAlt( gKeyBind.GetModAlt(code));
			}
		}
	}

	m_index = -1;
	list->Invalidate();
}


void
KeymapView::MessageReceived(BMessage *msg)
{
	char s[255];
	int32 code, index;
	BScreen screen;
	BPoint p;
	BListItem *it = NULL;

	switch(msg->what)
	{
		case SELECT:
		{
			index = list->FullListCurrentSelection();
			if (index < 0)
				// nothing selected
				break;

			it = list->FullListItemAt(index);
			if (it) {
				// needed to convert the outline numbers, they are inverted !
				code = ((KeyItem*)it)->GetCode();
				if (code > 0) {
					sprintf(s, "item: %s", gKeyBind.GetLabel(code));
					p.x =  (screen.Frame().left+screen.Frame().right)/2;
					p.y =  (screen.Frame().top+screen.Frame().bottom)/2;
					m_index = index;
					(new SetKeyWindow(p, code, this));
				}
			}
			break;
		}

		default:
			BView::MessageReceived(msg);
			break;   
	}
}
