#include "Application.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <thread>
#include "Interface.h"

Device* Application::_device;
HWND Application::window;
POINTS Application::position;

void Start() {
	Application* app = new Application();
	delete app;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

Application::Application() {
	window = nullptr;
	position = { };
	LPDIRECT3DDEVICE9 device =  Device::GetDevice();
	device = nullptr;
	CreateHWindow("GUI", "GUI class");
	_device = new Device(window);
	soundManager = new SoundManager();
	gui = new Interface(_device, soundManager, window, WIDTH, HEIGHT, &exit);
	while (exit) {
		gui->BeginRender();
		gui->Render();
		gui->EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

Application::~Application() {
	void DestroyHWindow();
}

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (Application::_device->GetDevice() && wideParameter != SIZE_MINIMIZED) {
			D3DPRESENT_PARAMETERS& presentParameters = Application::_device->GetPresentParameters();
			presentParameters.BackBufferWidth = LOWORD(longParameter);
			presentParameters.BackBufferHeight = HIWORD(longParameter);
			Application::_device->ResetDevice();
		}

	} return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) //Disable ALT application menuk
			return 0;
	} break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	} return 0;


	case WM_LBUTTONDOWN: {
		Application::position = MAKEPOINTS(longParameter);
	} return 0;
	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON) {
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(window, &rect);

			rect.left += points.x - Application::position.x;
			rect.top += points.y - Application::position.y;

			if (Application::position.x >= 0 &&
				Application::position.x <= Application::WIDTH &&
				Application::position.y >= 0 && Application::position.y <= 19)
				SetWindowPos(
					window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}
	} return 0;

	}

	return DefWindowProcW(window, message, wideParameter, longParameter);
}

void Application::CreateHWindow(
	const char* windowName,
	const char* className) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA(
		className,
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void Application::DestroyHWindow() noexcept {
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}