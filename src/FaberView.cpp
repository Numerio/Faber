/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FaberView.h"

#include <LayoutBuilder.h>
#include <StringView.h>

#include "FaberDefs.h"


FaberView::FaberView()
	:
	BGroupView(B_VERTICAL, 0)
{
	fToolBar = new ToolBar();
	//fToolBar->SetTool(Prefs.tool_mode);

	fTracksContainer = new TracksContainer();

	fInfoToolBar = new InfoToolBar();

	rgb_color backgroundColor = {120,120,120};

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(fToolBar)
		.AddStrut(0.0f)
			.AddGroup(B_VERTICAL, 0)
				.Add(fTracksContainer)
			.End()
		.AddStrut(10.0f)
		.Add(fInfoToolBar)
		.AddStrut(0.5f)
	.End();

	fTracksContainer->SetViewColor(backgroundColor);
}


FaberView::~FaberView()
{
}


void
FaberView::MessageReceived(BMessage* message)
{

	/*switch (message->what)
	{
		case FABER_ZOOM_IN:
			fTracksContainer->ZoomIn();
			break;
	
		case FABER_ZOOM_OUT:
			fTracksContainer->ZoomOut();
			break;
	
		case FABER_ZOOM_FULL:
			fTracksContainer->ZoomFull();
			break;

		case FABER_ZOOM_SELECTION:
			fTracksContainer->ZoomSelection();
			break;

		default:
			BGroupView::MessageReceived(message);
	}*/
}


TracksContainer*
FaberView::Container() const
{
	return fTracksContainer;
}


bool
FaberView::IsEmpty() const
{
	return fTracksContainer->CountTracks() == 0;
}

/*
bool
FaberView::IsSelected()
{
	
}*/


status_t
FaberView::AddTrack(Track* track)
{
	return fTracksContainer->AddTrack(track);
}


bool
FaberView::HasChanged()
{
	return fTracksContainer->HasChanged();
}
