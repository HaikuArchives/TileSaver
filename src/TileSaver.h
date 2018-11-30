//		Copyright © 1999	Jens Kilian
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

#ifndef TILESAVER_H
#define TILESAVER_H

#include <image.h>

#include <List.h>
#include <Message.h>
#include <Point.h>
#include <ScreenSaver.h>
#include <View.h>

class TileSaver : public BScreenSaver
{
	typedef BScreenSaver inherited;
public:
						TileSaver(BMessage* pMsg, image_id id);
	virtual 			~TileSaver(void);

	// Hook functions.
	virtual void 		Draw(BView* pView, int32 frame);
	virtual void 		StartConfig(BView* pConfigView);
	virtual status_t	StartSaver(BView* pView, bool preview);

private:
						TileSaver(const TileSaver &orig);
						TileSaver &operator =(const TileSaver &orig);

	void				DisposeRectList(void);

	// Data members.
	int					fLimit;
	BList				fRects;
};

#endif // TILESAVER_H
