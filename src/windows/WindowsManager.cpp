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

#include "WindowsManager.h"

#include "AboutWindow.h"
#include "FaberDefs.h"
#include "FreqWindow.h"
#include "ResampleWindow.h"


WindowsManager*	WindowsManager::fInstance = NULL;

const char* kCopyrights[] = {
	"2002 Xentronix",
	NULL
};

const char* kAuthors[] = {
	"Dario Casalinuovo",
	NULL
};

const char* kThanks[] = {
	"Frans van Nispen",
	"Libwalter devs",
	"Davide Gessa",
	"Stephan Aßmus",
	NULL
};


WindowsManager::WindowsManager()
	:
	fSavePanel(NULL),
	fOpenPanel(NULL),
	fSettingsWindow(NULL),
	fMainWindow(NULL)
{
	fProgress = new ProgressWindow(BRect(0, 0, 300, 40));
}


WindowsManager::~WindowsManager() 
{
	if (fProgress && fProgress->Lock()) {
		fProgress->Quit();
		fProgress->Unlock();
	}

	if (fSettingsWindow && fSettingsWindow->Lock()) {
		fSettingsWindow->Quit();
		fSettingsWindow->Unlock();
	}

	if (fOpenPanel)
		delete fOpenPanel;

	if (fSavePanel)
		delete fSavePanel;
}


WindowsManager*
WindowsManager::Get()
{
	if (fInstance == NULL)
		fInstance = new WindowsManager();

	return fInstance;	
}


// TODO this should not return a pointer
BMessenger*
WindowsManager::MainWinMessenger()
{
	BMessenger* mess = new BMessenger(Get()->fMainWindow);
	return mess;
}


FaberWindow*
WindowsManager::MainWindow()
{
	if (Get()->fMainWindow == NULL) {
		BRect rect(50, 50, WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
		Get()->fMainWindow = new FaberWindow(rect);
	}
	return Get()->fMainWindow;
}


void
WindowsManager::ShowSettings()
{
	if (fSettingsWindow == NULL)
		GetSettingsWindow();

	if (fSettingsWindow->LockLooper()) {

		if (fSettingsWindow->IsHidden())
			fSettingsWindow->Show();

		fSettingsWindow->Activate();
		fSettingsWindow->UnlockLooper();
	}
}


SettingsWindow*
WindowsManager::GetSettingsWindow()
{
	if (fSettingsWindow == NULL)
		fSettingsWindow = new SettingsWindow();

	return fSettingsWindow;
}


void
WindowsManager::ShowAbout()
{
	BAboutWindow* box = new BAboutWindow("Faber", "Faber");

	box->AddDescription(FABER_DESCRIPTION);
	box->AddCopyright(2012, FABER_COPYRIGHT, kCopyrights);
	box->SetVersion(FABER_VERSION);
	box->AddAuthors(kAuthors);
	box->AddSpecialThanks(kThanks);
	box->AddExtraInfo(FABER_HELP_HOMEPAGE);

	box->Show();
	box->MoveTo(_CalculateWindowPoint());
}


OpenPanel*
WindowsManager::GetOpenPanel()
{
	if (Get()->fOpenPanel == NULL)
		Get()->fOpenPanel = new OpenPanel(be_app);

	return Get()->fOpenPanel;
}


SavePanel*
WindowsManager::GetSavePanel()
{
	if (Get()->fSavePanel == NULL)
		Get()->fSavePanel = new SavePanel(be_app);	

	return Get()->fSavePanel;
}


void
WindowsManager::ShowResampleWindow()
{
	/*BPoint point = _CalculateWindowPoint();

	(new ResampleWindow(point));*/
}


void
WindowsManager::ShowFrequencyWindow()
{
	/*BPoint point = _CalculateWindowPoint();

	(new FreqWindow(point));*/
}


void
WindowsManager::StartProgress(const char *label, int32 max)
{
	fProgress->MoveTo(_CalculateWindowPoint());
	fProgress->StartProgress(label, max);

}


void
WindowsManager::ProgressUpdate(int32 delta)
{
	fProgress->SetProgress(delta);
}


void
WindowsManager::SetProgressName(const char *name)
{
	fProgress->SetProgressName(name);
}


void
WindowsManager::HideProgress()
{
	fProgress->HideProgress();
}


bool
WindowsManager::IsWindowValid(BWindow* window)
{
	BMessenger mess(window);
	return mess.IsValid();
}


BPoint
WindowsManager::_CalculateWindowPoint()
{
	BRect frame = Get()->fMainWindow->Frame();
	return BPoint((frame.left + frame.right) / 2,
		(frame.top + frame.bottom) / 2);
}
