#include "FileSystem.h"
#include <Windows.h>
#include <shobjidl.h>
#include <soloud.h>
#include "soloud_wav.h"
#include <vector>
#include <string>
#include <soloud_wavstream.h>

bool FileSystem::openFiles(std::vector<std::string>& sFilePathes)
{
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (f_SysHr == S_FALSE) {
		return FALSE;
	}

	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	const COMDLG_FILTERSPEC c_rgSaveTypes[] =
	{
		{L"Music file (*.wav; *.mp3)",       L"*.wav;*.mp3"},
	};


	f_SysHr = f_FileSystem->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);

	if (!SUCCEEDED(f_SysHr)) {
		f_FileSystem->Release();
		return FALSE;
	}

	DWORD dwOptions;
	f_SysHr = f_FileSystem->GetOptions(&dwOptions);

	if (SUCCEEDED(f_SysHr)) {
		f_SysHr = f_FileSystem->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
	}

	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		return FALSE;
	}

	//  SHOW OPEN FILE DIALOG WINDOW
	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	GetResults(f_SysHr, f_FileSystem, sFilePathes);
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}

void FileSystem::getFileAtributes(const std::string& path, std::string& title, std::string& artist, std::string& format) {
	const size_t slash = path.find_last_of("/\\");
	const size_t dot = path.find_last_of(".");
	const size_t dash = path.find_last_of("-");
	if (dash > path.size()) {
		title = path.substr(slash + 1, dot - slash - 1);
		artist = "None";
	}
	else {
		title = path.substr(slash + 1, dash - slash - 1);
		artist = path.substr(dash + 1, dot - dash - 1);
	}
	format = path.substr(dot + 1);
}

std::string FileSystem::getDuration(const std::string& path) {
	SoLoud::WavStream* checkSong = new SoLoud::WavStream;
	std::string result = "";
	checkSong->load(path.c_str());
	float duration = checkSong->getLength();
	int minutes = (int)duration / 60;
	result += std::to_string(minutes) + ":" + std::to_string((int)duration - minutes * 60);
	delete checkSong;
	return result;
}

void FileSystem::GetResults(HRESULT& f_SysHr, IFileOpenDialog* f_FileSystem, std::vector<std::string>& sFilePathes) {
	IShellItemArray* psiaResults;
	std::vector<std::string> pathes;
	f_SysHr = f_FileSystem->GetResults(&psiaResults);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path = NULL;
	IShellItem* ppsi;
	DWORD pdwNumItems;
	HRESULT psiaRes = psiaResults->GetCount(&pdwNumItems);
	if (psiaRes != S_OK) {
		psiaResults->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return;
	}
	for (int i = 0; i < pdwNumItems; i++) {//iterating through ShellItemArray
		psiaResults->GetItemAt(i, &ppsi);
		f_SysHr = ppsi->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
		if (FAILED(f_SysHr)) {
			psiaResults->Release();
			f_FileSystem->Release();
			CoUninitialize();
			return;
		}
		std::wstring path(f_Path);
		std::string c(path.begin(), path.end());
		sFilePathes.push_back(c);
	}
	CoTaskMemFree(f_Path);
	psiaResults->Release();
}