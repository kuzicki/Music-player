#pragma once
#include <string>

class Song {
public:
	Song(std::string filePath, std::string artist, std::string title, std::string fileFormat, std::string duration) :
		_filePath(filePath), _artist(artist), _title(title), _fileFormat(fileFormat), _duration(duration) { }
	const char* Artist() const;
	const char* Title() const;
	const char* FileFormat() const;
	const char* Duration() const;
	const char* Path() const;
private:
	std::string _filePath;
	std::string _artist;
	std::string _title;
	std::string _fileFormat;
	std::string _duration;
};