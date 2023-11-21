#pragma once
#include <d3d9.h>

class Device {
public:
	Device(HWND window);
	~Device();
	void ResetDevice() noexcept;
	static LPDIRECT3DDEVICE9 GetDevice();
	D3DPRESENT_PARAMETERS& GetPresentParameters();
private:
	PDIRECT3D9 d3d = nullptr;
	HWND _window;
	static LPDIRECT3DDEVICE9 device;
	D3DPRESENT_PARAMETERS presentParameters = { };
	bool CreateDevice() noexcept;
	void DestroyDevice() noexcept;
};