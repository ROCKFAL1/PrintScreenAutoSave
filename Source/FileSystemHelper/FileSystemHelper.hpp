#pragma once

#include <string>
#include <filesystem>
#include <Windows.h>

class FileSystemHelper
{
public:
	static std::string GetSystemDiskName();
	static LPWSTR GetPathOfExecutable();
};
