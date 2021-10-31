#pragma once

#include <string>
#include <Windows.h>

class StringUtilities
{
public:
	static std::wstring StrToWstr(std::string& string);
	static bool PtrToWchIsEqual(WCHAR* w1, WCHAR* w2, size_t size);
};
