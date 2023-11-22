#include "Interface.h"
#include "SongPlaylist.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#define WIDTH 500
#define HEIGHT 300

#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning (disable: 26451)    // [Static Analyzer] Arithmetic overflow : Using operator 'xxx' on a 4 byte value and then casting the result to an 8 byte value. Cast the value to the wider type before calling operator 'xxx' to avoid overflow(io.2).
#endif


Interface::Interface(Device* device, SoundManager* soundManager, HWND window, int width, int height, bool* exit) : _soundManager(soundManager), _exit(exit), _songTable(soundManager), _addButton(soundManager, _fileSystem) {
	_devicePointer = device->GetDevice();
	CreateImGui(window);
}

Interface::~Interface() {
	DestroyImGui();
	delete _fileSystem;
}

void Interface::CreateImGui(HWND window) noexcept {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(_devicePointer);
}

void Interface::DestroyImGui() noexcept {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Interface::BeginRender() noexcept {
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Interface::EndRender() noexcept {
	ImGui::EndFrame();

	_devicePointer->SetRenderState(D3DRS_ZENABLE, FALSE);
	_devicePointer->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	_devicePointer->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	_devicePointer->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.f, 0);

	if (_devicePointer->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		_devicePointer->EndScene();
	}

	const auto result = _devicePointer->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && _devicePointer->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		_device->ResetDevice();
}

void Interface::Render() noexcept {
	_soundManager->UpdatePlaytime();
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin("Music player", _exit,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);
	std::string heading = _soundManager->GetSongName();
	if (heading == "") {
		heading = "Menu";
	}
	ImGui::SeparatorText(heading.c_str());
	
	_songTable.Render();
	
	ImGui::SameLine();
	ImGui::PushItemWidth(-100.f);
	if (ImGui::VSliderFloat("##ff15", ImVec2(18, 160), _soundManager->GetVolume(), 0.0f, 1.f, "")) {
		_soundManager->SetVolume();
	}
	ImGui::PopItemWidth();

	_addButton.Render();
	
	_time = _soundManager->GetSongPlaytime();
	
	int minutes = (int)_time / 60;
	std::string displayText = _soundManager->GetDisplayString();
	ImGui::Text(displayText.c_str(), minutes, (int)_time - minutes * 60);
	ImGui::SameLine();
	if (ImGui::SliderFloat("##idk", &_time, 0.0f, static_cast<float>(_soundManager->GetSongTime()), "")) {
		_newTimeValue = _time;
	}
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		_soundManager->Seek(_newTimeValue);
	}

	if (ImGui::Button("prev")) {
		_soundManager->PlayPrev();
	}
	ImGui::SameLine();

	if (ImGui::Button("play/stop")) {
		_soundManager->PlaySong();
	}
	ImGui::SameLine();

	if (ImGui::Button("next")) {
		_soundManager->PlayNext();
	}
	ImGui::SameLine();

	if (ImGui::Button("reset")) {
		_soundManager->ResetSong();
	}
	ImGui::SameLine();
	
	if (ImGui::Checkbox("Loop", &isLooping)) {
		_soundManager->ChangeLoop(isLooping);
	}

	_soundManager->CheckSoundManagerState();

	ImGui::End();
}