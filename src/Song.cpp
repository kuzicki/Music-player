#include "Song.h"

const char* Song::Path() const {
    return _filePath.c_str();
}

const char* Song::Duration() const {
    return _duration.c_str();
}

const char* Song::FileFormat() const {
    return _fileFormat.c_str();
}

const char* Song::Title() const {
    return _title.c_str();
}

const char* Song::Artist() const {
    return _artist.c_str();
}