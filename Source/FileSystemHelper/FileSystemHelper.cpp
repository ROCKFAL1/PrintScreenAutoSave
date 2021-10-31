#include "FileSystemHelper.hpp"

std::string FileSystemHelper::GetSystemDiskName()
{
	char systemDirectory[MAX_PATH];
	std::string systemDisk;
	GetSystemDirectoryA(systemDirectory, MAX_PATH);
	systemDisk.push_back(systemDirectory[0]);
	return systemDisk;
}

LPWSTR FileSystemHelper::GetPathOfExecutable()
{
#pragma warning ( disable: 6387)
	LPWSTR autorun = new wchar_t[MAX_PATH];
	
	GetModuleFileNameW(NULL, autorun, MAX_PATH);
	return autorun;
}