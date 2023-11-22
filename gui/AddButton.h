#pragma once
#include "IGUIElement.h"
#include "FileSystem.h"
#include "SongPlaylist.h"
#include "Soundmanager.h"

class AddButton : IGUIElement {
public:
	AddButton(SoundManager* soundManager, FileSystem* fileSystem);
	void Render() override;
private:
	FileSystem* _fileSystem;
};