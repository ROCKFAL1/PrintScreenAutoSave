#pragma once

#include <iostream>
#include <string>

#include <Windows.h>

class RegSettings
{
public:
	RegSettings(std::wstring subKey, std::wstring key);
	~RegSettings();

	std::wstring GetParameterValueStr(std::wstring parameterName, unsigned long valueLength = 100);
	int GetParameterValueInt(std::wstring parameterName);

	bool SetParameterValue(std::wstring parameterName, std::wstring value);
	bool SetParameterValueInt(std::wstring parameterName, int value);

private:
	HKEY settingRootKey = HKEY_CURRENT_USER;
};
