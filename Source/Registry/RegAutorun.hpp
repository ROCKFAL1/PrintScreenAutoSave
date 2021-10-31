#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <windows.h>

#include "../FileSystemHelper/FileSystemHelper.hpp"
#include "../StringUtilities/StringUtilities.hpp"

#define PROG_KEY_NAME L"PrintScreenAutoSave"
#define AUTORUN_PATH L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"

class RegAutorun
{
public:
	static bool PutOrUpdateInAutorun();
	static bool DeleteFromAutorun();
	static bool IsInAutorun();
};
