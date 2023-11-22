#pragma once
#include <d3d9.h>
#include "Device.h"
#include "SongTable.h"
#include "SoundManager.h"
#include "FileSystem.h"
#include "AddButton.h"

class Interface {
public:
	Interface(Device* device, SoundManager* soundManager, HWND window, int width, int height, bool* exit);
	~Interface();
	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
private:
	void CreateImGui(HWND window) noexcept;
	void DestroyImGui() noexcept;
	FileSystem* _fileSystem = new FileSystem();
	LPDIRECT3DDEVICE9 _devicePointer;
	Device* _device;
	SoundManager* _soundManager;
	SongTable _songTable;
	AddButton _addButton;
	bool* _exit;
	int HEIGHT, WIDTH;
	float _time;
	float _newTimeValue;
	bool isLooping;
};