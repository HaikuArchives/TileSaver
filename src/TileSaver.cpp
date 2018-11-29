//		Copyright © 1999–2000  Jens Kilian
//
//		This program is free software; you can redistribute it and/or modify
//		it under the terms of the GNU General Public License as published by
//		the Free Software Foundation; either version 1, or (at your option)
//		any later version.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
//		GNU General Public License for more details.
//
//		You should have received a copy of the GNU General Public License
//		along with this program; if not, write to the Free Software
//		Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//		Screen saver class.

#include <cstdlib>
#include <cstring>
#include <image.h>

#include <GraphicsDefs.h>
#include <List.h>
#include <Message.h>
#include <OS.h>
#include <Point.h>
#include <Rect.h>
#include <Screen.h>
#include <ScreenSaver.h>
#include <TextView.h>
#include <View.h>

#include "TileSaver.h"
#include "RandomColor.h"
#include "RandomNumbers.h"

const rgb_color kBlack = { 0, 0, 0, 0 };

// Screen saver interface.
BScreenSaver *
instantiate_screen_saver(BMessage *pMsg, image_id id)
{
	return new TileSaver(pMsg, id);
}

TileSaver::TileSaver(BMessage *pMsg, image_id id)
:	inherited(pMsg, id),
	fLimit(0),
	fRects()
{
	srand((long)system_time());
}

TileSaver::~TileSaver(void)
{
	DisposeRectList();
}

void
TileSaver::Draw(BView *pView, int32 frame)
{
	const BRect bounds = pView->Bounds();
	if (frame % fLimit == 0) {
		DisposeRectList();
		fRects.AddItem(new BRect(bounds));
	}

	BRect *pRect = (BRect *)fRects.RemoveItem((int32)0);
	pView->SetHighColor(RandomColor(0.2, 1.0, 0.2, 1.0));
	pView->FillRect(*pRect);
	pView->StrokeRect(*pRect, B_SOLID_LOW);

	const float split = randomFloat(0.3, 0.7);
	BRect rect1, rect2;
	if (pRect->Width() > pRect->Height()) {
		const float xSplit = pRect->left * split + pRect->right * (1.0 - split);
		rect1 = BRect(pRect->left, pRect->top, xSplit, pRect->bottom);
		rect2 = BRect(xSplit, pRect->top, pRect->right, pRect->bottom);
	} else {
		const float ySplit = pRect->top * split + pRect->bottom * (1.0 - split);
		rect1 = BRect(pRect->left, pRect->top, pRect->right, ySplit);
		rect2 = BRect(pRect->left, ySplit, pRect->right, pRect->bottom);
	}
	if (randomFloat() < 0.5) {
		fRects.AddItem(new BRect(rect1));
		fRects.AddItem(new BRect(rect2));
	} else {
		fRects.AddItem(new BRect(rect2));
		fRects.AddItem(new BRect(rect1));
	}
	
	delete pRect;
	
	pView->Sync();
}

void
TileSaver::StartConfig(BView *pConfigView)
{
	/*const*/ BRect frame = pConfigView->Bounds().InsetBySelf(10.0, 10.0);
	BTextView *pCaption =
		new BTextView(frame, "TileSaver", frame.OffsetToCopy(B_ORIGIN),
							B_FOLLOW_ALL, B_WILL_DRAW);
	pCaption->SetText("TileSaver\n"
							"\n"
							"\t© 1999  Jens Kilian <jjk@acm.org>\n"
							"\n"
							"TileSaver comes with ABSOLUTELY NO WARRANTY; "
							"it is free software, and you are welcome "
							"to redistribute it under certain conditions.  "
							"See the GNU General Public License for details.");
	pConfigView->AddChild(pCaption);
	pCaption->SetViewColor(pConfigView->ViewColor());
	pCaption->SetStylable(true);
	pCaption->SetFontAndColor(be_plain_font);
	const char *pText = pCaption->Text();
	pCaption->SetFontAndColor(0, strchr(pText, '\n') - pText, be_bold_font);
	pCaption->MakeEditable(FALSE);
	pCaption->MakeSelectable(FALSE);
}

status_t
TileSaver::StartSaver(BView * /*pView*/, bool preview)
{
	SetTickSize(preview ? 100000 : 10000);	// 0.1/0.01 seconds
	fLimit = preview ? 256 : 2048;
	return B_OK;
}

void
TileSaver::DisposeRectList(void)
{
	while (!fRects.IsEmpty()) {
		delete fRects.RemoveItem(fRects.CountItems() - 1);
	}
}
