#include "RegSettings.hpp"

RegSettings::RegSettings(std::wstring subKey, std::wstring key)
{
	HKEY hKeys;

	if (ERROR_SUCCESS != RegOpenKeyW(HKEY_CURRENT_USER, &subKey[0], &hKeys))
	{
		return;
	}

	if (ERROR_SUCCESS != RegCreateKeyW(hKeys, &key[0], &settingRootKey))
	{
		return;
	}

	RegCloseKey(hKeys);
}

RegSettings::~RegSettings()
{
	RegCloseKey(settingRootKey);
}

bool RegSettings::SetParameterValue(std::wstring parameterName, std::wstring value)
{

	if (ERROR_SUCCESS != RegSetValueExW(settingRootKey, &parameterName[0], 0,
		REG_SZ, reinterpret_cast<LPBYTE>(&value[0]), sizeof(wchar_t) * value.size()))
	{
		return false;
	}

	return true;
}

bool RegSettings::SetParameterValueInt(std::wstring parameterName, int value)
{
	if (ERROR_SUCCESS != RegSetValueExW(settingRootKey, &parameterName[0], 0,
		REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(int)))
	{
		return false;
	}

	return true;
}

std::wstring RegSettings::GetParameterValueStr(std::wstring parameterName, unsigned long valueLength)
{
	DWORD type = REG_SZ;
	DWORD bufferSize = valueLength * sizeof(wchar_t);
	wchar_t* buffer = new wchar_t[valueLength];
	
	if (ERROR_SUCCESS != RegQueryValueExW(settingRootKey, &parameterName[0], 0,
		&type, reinterpret_cast<LPBYTE>(&buffer[0]), &bufferSize))
	{
		return L"-1";
	}

	std::wstring result;
	result.resize(valueLength);
	for (int i = 0; i < valueLength; i++)
	{
		result[i] = buffer[i];
	}

	return result;
}

int RegSettings::GetParameterValueInt(std::wstring parameterName)
{
	DWORD type = REG_DWORD;
	int result = 0;
	DWORD resultSize = sizeof(result);

	if (ERROR_SUCCESS != RegQueryValueExW(settingRootKey, &parameterName[0], 0,
		&type, reinterpret_cast<LPBYTE>(&result), &resultSize))
	{
		return -1;
	}

	return result;
}