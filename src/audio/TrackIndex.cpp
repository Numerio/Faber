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


#include "TrackIndex.h"

#include "AudioProtocolDefs.h"
#include "FaberDefs.h"


TrackIndex::TrackIndex()
	:
	BArchivable()
{
	fInitErr = B_OK;
	fChannels = new BObjectList<MediaBlockMap>(true);
}


TrackIndex::TrackIndex(BMessage* from)
	:
	BArchivable(from)
{
	BMessage msg;
	for (int i = 0; from->FindMessage(SAVE_TRACK_CHANNEL_NAME, i, &msg)
		== B_OK; i++) {

		MediaBlockMap* channel =
			(MediaBlockMap*)MediaBlockMap::Instantiate(&msg);

		fChannels->AddItem(channel);
	}
}


TrackIndex::~TrackIndex()
{
	delete fChannels;
}


status_t
TrackIndex::Archive(BMessage* into, bool deep) const
{
	BArchivable::Archive(into, deep);

	for (int i = 0; i < fChannels->CountItems(); i++) {
		MediaBlockMap* channel = fChannels->ItemAt(i);

		BMessage* msg = MessageBuilder(SAVE_TRACK_CHANNEL, channel);
		into->AddMessage(SAVE_TRACK_CHANNEL_NAME, msg);
	}

	return B_OK;
}


BArchivable*
TrackIndex::Instantiate(BMessage* archive)
{
	if (!validate_instantiation(archive, "TrackIndex"))
		return NULL;

	return new TrackIndex(archive);
}


status_t
TrackIndex::InitCheck() const
{
	return fInitErr;
}


uint32
TrackIndex::CountChannels() const
{
	return fChannels->CountItems();
}


void
TrackIndex::AddChannel(MediaBlockMap* tree)
{
	fChannels->AddItem(tree);
}


MediaBlockMap*
TrackIndex::GetChannel(int32 index) const
{
	return fChannels->ItemAt(index);
}


BObjectList<MediaBlockMap>&
TrackIndex::GetChannels() const
{
	return *fChannels;
}


MediaBlockMap*
TrackIndex::ExtractChannel(int32 index)
{
	// Unimplemented right now.
	return NULL;
}
