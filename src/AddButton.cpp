#include "AddButton.h"


AddButton::AddButton(SoundManager* soundManager, FileSystem* fileSystem) : IGUIElement(soundManager), _fileSystem(fileSystem) { }

void AddButton::Render() {
	if (ImGui::Button("+")) {
		std::vector<std::string> newPathes;
		SongPlaylist*& playlist = _soundManager->getPlaylist();
		_fileSystem->openFiles(newPathes);
		std::string title, artist, format;
		for (auto& path : newPathes) {
			_fileSystem->getFileAtributes(path, title, artist, format);
			playlist->Add(path, artist, title, format, _fileSystem->getDuration(path));
		}
	}
}