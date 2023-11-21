#pragma once
#include "Song.h"
#include <vector>

class SongPlaylist {
public:
	SongPlaylist() : currentSongPosition(0) { }
	void Add(std::string path, std::string artist, std::string title, std::string format, std::string duration);
	Song& operator[](int position);
	Song* GetNextSong();
	Song* GetPrevSong();
	Song* GetCurrentSong();
	std::string GetCurrentSongName();
	int GetCount();
	bool IsNextSong();
private:
	std::vector<Song> playlist;
	int currentSongPosition;
};