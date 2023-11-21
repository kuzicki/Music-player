#include "Application.h"
#define _CRT_SECURE_NO_WARNINGS
#include <thread>

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow) 
{
	Start();
	return EXIT_SUCCESS;
}