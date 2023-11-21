#pragma once
#include "SongPlaylist.h"
#include <soloud.h>
#include <soloud_wav.h>

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	void PlaySong();
	void UpdatePlaytime();
	const std::string GetDisplayString();
	void PlayPrev();
	void PlayNext();
	void ResetSong();
	std::string GetSongName();
	int GetSongsSize();
	inline void Seek(double time) {
		if (currentSong != nullptr) {
			gSoloud->seek(soloudHandle, 0.f);
			gSoloud->seek(soloudHandle, time);
		}
	}
	inline double GetSongTime() const {
		return songTime;
	}
	inline SongPlaylist*& getPlaylist() {
		return _playlist;
	}
	inline float* GetVolume() {
		return &volume;
	}
	inline void SetVolume() {
		gSoloud->setGlobalVolume(volume);
	}

	inline void ChangeLoop(bool isLooping) {
		_isLooping = isLooping;
		SetLoop();
	}

	inline void SetLoop() {
		gSoloud->setLooping(soloudHandle, _isLooping);
	}
	inline float GetSongPlaytime() {
		return time;
	}
	void CheckSoundManagerState();
private:
	void InitializeAudio();
	void DeinitializeAudio();
	SoLoud::Soloud* gSoloud;
	SoLoud::Wav* currentSong;
	SongPlaylist* _playlist;
	float volume = .15f;
	int soloudHandle = 0;
	float time;
	float songTime = 0.f;
	bool _isLooping = false;
};