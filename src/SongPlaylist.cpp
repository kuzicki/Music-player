#include "SongPlaylist.h"

void SongPlaylist::Add(std::string path, std::string artist, std::string title,std::string format, std::string duration) {
    playlist.emplace_back(path, artist, title, format, duration);
}

Song& SongPlaylist::operator[](int position) {
    return playlist[position];
}

Song* SongPlaylist::GetNextSong() {
    if (!playlist.size())
        return nullptr;
    if (currentSongPosition + 1 < playlist.size())
        return &playlist[++currentSongPosition];
    if (currentSongPosition + 1 == playlist.size())
        return &playlist[currentSongPosition];
}

bool SongPlaylist::IsNextSong() {
    if (!playlist.size())
        return false;
    if (currentSongPosition < playlist.size() - 1)
        return true;
    return false;
}

Song* SongPlaylist::GetPrevSong() {
    if (!playlist.size())
        return nullptr;
    if (currentSongPosition >= 1)
        return &playlist[--currentSongPosition];
    if (currentSongPosition == 0)
        return &playlist[currentSongPosition];
}

Song* SongPlaylist::GetCurrentSong() {
    if (playlist.size() != 0)
        return &playlist[currentSongPosition];
    return nullptr;
}

std::string SongPlaylist::GetCurrentSongName() {
    if (!playlist.size())
        return "";
    return playlist[currentSongPosition].Title();
}

int SongPlaylist::GetCount() {
    return playlist.size();
}