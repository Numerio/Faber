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

#ifndef WINDOWS_MANAGER
#define WINDOWS_MANAGER

#include <FilePanel.h>
#include <Messenger.h>

#include "ExportWindow.h"
#include "FaberEffect.h"
#include "FaberWindow.h"
#include "ParameterWindow.h"
#include "ProgressWindow.h"
#include "SettingsWindow.h"


class WindowsManager {
public:
	static FaberWindow*		MainWindow();

	static void				PostMessage(BMessage* message,
								BHandler* handler = NULL);

	static void				ShowSettings();

	static void				ShowAbout();

	// App panels
	static BFilePanel*		GetOpenPanel();
	static BFilePanel*		GetSavePanel();
	static ExportWindow*	GetExportPanel();

	static ParameterWindow*	GetFaberMixer();
	static ParameterWindow*	GetSystemMixer();

	// Progress window methods
	static void				ProgressUpdate(float percent, const char* text);

	static void				StartProgress(const char *label, int32 max = 100);
	static void				HideProgress();

	// Util method which you can use to see if 
	// a window is already opened / valid or not.
	static bool				IsWindowValid(BWindow* window);

	// This display just a minimal advice alert
	static void				SimpleAlert(const char* text);
	static void				SimpleError(const char* text);

private:
	static BPoint			_CalculateWindowPoint();

	static FaberWindow*		fMainWindow;
	static SettingsWindow*	fSettingsWindow;
	static ProgressWindow*	fProgress;

	static BFilePanel*		fOpenPanel;
	static BFilePanel*		fSavePanel;
	static ExportWindow*	fExportWindow;

	static ParameterWindow*	fFaberMixer;
	static ParameterWindow*	fSystemMixer;
};

#endif
