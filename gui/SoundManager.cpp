#include "SoundManager.h"
#include "../soloud/soloud_wav.h"

void SoundManager::InitializeAudio() {
	gSoloud = new SoLoud::Soloud();
	gSoloud->init();
	gSoloud->setGlobalVolume(volume);
}

void SoundManager::DeinitializeAudio() {
	gSoloud->deinit();
	delete gSoloud;
}

SoundManager::SoundManager() {
	currentSong = nullptr;
	_playlist = new SongPlaylist();
	InitializeAudio();
}

SoundManager::~SoundManager() {
	DeinitializeAudio();
	delete _playlist;
}

void SoundManager::PlaySong() {
	if (currentSong == nullptr) {
		Song* song = _playlist->GetCurrentSong();
		if (song == nullptr)
			return;
		time = 0.f;
		currentSong = new SoLoud::Wav();
		currentSong->load(song->Path());
		songTime = static_cast<float>(currentSong->getLength());
		soloudHandle = gSoloud->play(*currentSong);
		SetLoop();
	}
	else {
		bool isPaused = gSoloud->getPause(soloudHandle);
		gSoloud->setPause(soloudHandle, !isPaused);
	}
}

void SoundManager::UpdatePlaytime() {
	time = static_cast<float>(gSoloud->getStreamPosition(soloudHandle));
}

const std::string SoundManager::GetDisplayString() {
	int minutes = (int)time / 60;
	if (!(int)time) {
		return "0%d:0%d";
	}
	else if (minutes < 10 && (int)time % 60 < 10) {
		return "0%d:0%d";
	}
	else if ((int)time % 60 < 10) {
		return "%d:0%d";
	}
	else if (minutes < 10) {
		return "0%d:%d";
	}
	else
		return "%d:%d";
}

void SoundManager::PlayPrev() {
	if (currentSong != nullptr)
		delete currentSong;
	Song* nextSong = _playlist->GetPrevSong();
	if (nextSong == nullptr)
		return;
	currentSong = new SoLoud::Wav();
	currentSong->load(nextSong->Path());
	time = 0.f;
	songTime = static_cast<float>(currentSong->getLength());
	soloudHandle = gSoloud->play(*currentSong);
	SetLoop();
}

void SoundManager::PlayNext() {
	if (currentSong != nullptr)
		delete currentSong;
	Song* nextSong = _playlist->GetNextSong();
	if (nextSong == nullptr)
		return;
	currentSong = new SoLoud::Wav();
	currentSong->load(nextSong->Path());
	time = 0.f;
	songTime = static_cast<float>(currentSong->getLength());
	soloudHandle = gSoloud->play(*currentSong);
	SetLoop();
}

void SoundManager::ResetSong() {
	if (gSoloud->getVoiceCount() < 1 && currentSong != nullptr) {
		delete currentSong;
		PlaySong();
	}
	else {
		gSoloud->seek(soloudHandle, 0.f);
	}
}

std::string SoundManager::GetSongName() {
	return _playlist->GetCurrentSongName();
}

int SoundManager::GetSongsSize() {
	return _playlist->GetCount();
}

void SoundManager::CheckSoundManagerState() {
	if (currentSong != nullptr && _playlist->IsNextSong() && !gSoloud->getVoiceCount()) {
		PlayNext();
	}
}