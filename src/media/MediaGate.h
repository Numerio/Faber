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
#ifndef _MEDIA_GATE_H
#define _MEDIA_GATE_H


#include <SupportDefs.h>

#include "Track.h"


class MediaGate {
public:
	virtual status_t				Start();
	virtual status_t				Stop();

			bool					IsStarted() const;

			void					SetLoop(bool loop);
			bool					Loop() const;

			void					SetPause(bool pause);
			bool					IsPaused() const;

protected:
			bool					fStarted;
			bool					fLoop;
};

#endif
