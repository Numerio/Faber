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


#include "ProjectManager.h"

#include "AutoDeleter.h"
#include "CommandBuilder.h"
#include "CommandServer.h"
#include "FaberDefs.h"
#include "MediaAgent.h"
#include "MessageBuilder.h"
#include "StorageUtils.h"
#include "TrackIO.h"

BString	ProjectManager::fName;
bool ProjectManager::fWasSaved = false;
BPath ProjectManager::fProjectPath;
BFile* ProjectManager::fProjectFile;


void
ProjectManager::Init()
{
	fProjectPath = StorageUtils::TemporaryDirRequested();
	fProjectFile = StorageUtils::GetProjectFile(fProjectPath, true);
}


void
ProjectManager::Quit()
{
	// Delete the temporary dir of this project
	StorageUtils::DeleteDirectory(fProjectPath);
}


bool
ProjectManager::HasChanged()
{
	//return SnanpshotManager::HasChanged();

	return true;
}


bool
ProjectManager::HasRedo()
{
	//return SnapshotManager::HasRedo();

	return false;
}


bool
ProjectManager::HasUndo()
{
	//return SnapshotManager::HasUndo();

	return false;
}


void
ProjectManager::Undo()
{
	//return SnapshotManager::Undo();
}


void
ProjectManager::Redo()
{
	//return SnapshotManager::Redo();
}


bool
ProjectManager::WasSaved()
{
	return fWasSaved;
}


status_t
ProjectManager::SaveProject()
{
	BMessage* msg = MediaAgent::ArchiveTracks();

	// todo seek at 0
	if (msg->Flatten(fProjectFile) != B_OK)
		return B_ERROR;

	delete msg;

	if (fWasSaved == false)
		fWasSaved = true;

	return B_OK;
}


status_t
ProjectManager::LoadProject(entry_ref ref)
{
	WindowsManager::ProgressUpdate(1.0f, "Loading project");

	BPath path;
	BEntry entry(&ref);

	if (entry.GetPath(&path) != B_OK)
		return B_ERROR;

	fProjectPath.SetTo(path.Path());
	fProjectFile = new BFile(&ref, B_READ_WRITE);

	BMessage* msg = new BMessage();

	// todo seek
	if (msg->Unflatten(fProjectFile) == B_OK
		&& MediaAgent::UnarchiveTracks(msg) != B_OK) {
		fWasSaved = true;

		WindowsManager::PostMessage(MessageBuilder(FABER_UPDATE_MENU));

		return B_OK;
	}

	WindowsManager::ProgressUpdate(2.0f, "Error loading project!");

	return B_ERROR;
}


status_t
ProjectManager::LoadFile(entry_ref ref)
{
	WindowsManager::StartProgress("Loading File");

	// TODO maybe use B_MEDIA_FILE_BIG_BUFFERS !?
	BMediaFile* mediaFile = new BMediaFile(&ref);
	ObjectDeleter<BMediaFile> deleter(mediaFile);
	status_t ret = B_ERROR;

	if (mediaFile->InitCheck() == B_OK) {
		ret = LoadMediaFile(mediaFile, ref.name);
		WindowsManager::HideProgress();
		return ret;
	}

	ret = LoadProject(ref);

	WindowsManager::ProgressUpdate(100.0f, "Operation complete!\n");

	WindowsManager::HideProgress();

	return ret;
}


status_t
ProjectManager::LoadMediaFile(BMediaFile* mediaFile, const char* name)
{
	BString str("Loading media file ");
	str << name;
	WindowsManager::ProgressUpdate(1.0f, str);

	AudioTrack* track = TrackIO::ImportAudio(mediaFile, name);

	if (track == NULL)
		return B_ERROR;

	status_t ret = track->InitCheck();

	if (ret != B_OK) {
		BString err("Error loading track: ");
		err << strerror(ret);
		WindowsManager::ProgressUpdate(0.0f, err);

		delete track;
		return ret;
	}

  	_NotifyTracksContainer(track);

	return B_OK;
}


BPath
ProjectManager::GetProjectPath()
{
	return fProjectPath;
}


status_t
ProjectManager::RegisterTrack(Track* track)
{
	if (track->IsAudio())
		return MediaAgent::RegisterTrack(track);

	return B_ERROR;
}


status_t
ProjectManager::UnregisterTrack(Track* track)
{
	if (track->IsAudio())
		return MediaAgent::UnregisterTrack(track);

	return B_ERROR;
}


BString
ProjectManager::Name()
{
	return fName;
}


void
ProjectManager::SetName(const char* name)
{
	fName = BString(name);
	BString title("Faber - ");
	title.Append(fName);

	FaberWindow* win = WindowsManager::MainWindow();
	if (win->Lock()) {
		win->SetTitle(title);
		win->Unlock();
	}
}


void
ProjectManager::_NotifyTracksContainer(Track* track)
{
	BMessage* msg = CommandBuilder(FABER_ADD_TRACK);
	msg->AddPointer("faber:track_pointer", track);
	CommandServer::SendCommand(msg);
}
