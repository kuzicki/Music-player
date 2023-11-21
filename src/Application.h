#pragma once
#pragma once
#include "FileSystem.h"
#include <memory>
#include "SoundManager.h"
#include "Interface.h"
#include "SongPlaylist.h"
#include "Device.h"

void Start();

class Application {
public:
	Application();
	~Application();
private:
	void CreateHWindow(const char* windowName,
		const char* className) noexcept;
	void DestroyHWindow() noexcept;
	
	static constexpr int WIDTH = 500;
	static constexpr int HEIGHT = 300;
	static HWND window;
	WNDCLASSEXA windowClass = { };
	bool exit = true;
	static POINTS position;

	SoundManager* soundManager;
	Interface* gui;
	FileSystem* fileSystem;
	SongPlaylist* playlist;
	static Device* _device;
	friend long __stdcall WindowProcess(
		HWND window,
		UINT message,
		WPARAM wideParameter,
		LPARAM longParameter);
};