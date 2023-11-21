#pragma once

#include <vector>
#include <string>
#include <shobjidl.h>

class FileSystem {
public:
	bool openFiles(std::vector<std::string>& sFilePathes);
	void getFileAtributes(const std::string& path, std::string& title, std::string& artist, std::string& format);
	std::string getArtist(const std::string& path);
	std::string getFormat(const std::string& path);
	std::string getDuration(const std::string& path);
private:
	void GetResults(HRESULT& f_SysHr, IFileOpenDialog* f_FileSystem, std::vector<std::string>& sFilePathes);
};