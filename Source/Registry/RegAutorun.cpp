#include "RegAutorun.hpp"

bool RegAutorun::PutOrUpdateInAutorun()
{
	HKEY hKeys;

	if (ERROR_SUCCESS != RegOpenKeyW(HKEY_CURRENT_USER, AUTORUN_PATH, &hKeys))
	{
		return false;
	}
	if (ERROR_SUCCESS != RegSetValueExW(hKeys, PROG_KEY_NAME, 0,
		REG_SZ, reinterpret_cast<LPBYTE>(FileSystemHelper::GetPathOfExecutable()), sizeof(wchar_t) * MAX_PATH))
	{
		return false;
	}	
	
	RegCloseKey(hKeys);
	return true;
}

bool RegAutorun::DeleteFromAutorun()
{
	HKEY hKeys;

	if (ERROR_SUCCESS != RegOpenKeyW(HKEY_CURRENT_USER, AUTORUN_PATH, &hKeys))
	{
		return false;
	}

	if (ERROR_SUCCESS != RegDeleteValueW(hKeys, PROG_KEY_NAME))
	{
		return false;
	}

	RegCloseKey(hKeys);
	return true;
}

bool RegAutorun::IsInAutorun()
{
	HKEY hKeys;

	WCHAR buffer[MAX_PATH];
	DWORD bufferSize = sizeof(buffer);
	DWORD bufferType = REG_SZ;

	if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_CURRENT_USER, AUTORUN_PATH, 0, KEY_QUERY_VALUE, &hKeys))
	{
		return false;
	}

	if (ERROR_SUCCESS != RegQueryValueExW(hKeys, PROG_KEY_NAME, NULL,
		&bufferType, reinterpret_cast<LPBYTE>(&buffer), &bufferSize))
	{
		return false;
	}

	LPWSTR executebleFilePath = FileSystemHelper::GetPathOfExecutable();
	RegCloseKey(hKeys);
	return StringUtilities::PtrToWchIsEqual(executebleFilePath, buffer, MAX_PATH);
}
