#pragma once

#include <string>
#include <filesystem>
#include <Windows.h>

namespace AutoSave
{
	void InitAutoSave(std::string folder);

	//TODO
	void UpdateScreenFolder(std::string newFolder);

	void SaveScreen();
}