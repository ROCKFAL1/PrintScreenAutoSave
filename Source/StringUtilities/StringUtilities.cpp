#include "StringUtilities.hpp"

std::wstring StringUtilities::StrToWstr(std::string& string)
{
	const size_t cSize = string.size() + 1;

	std::wstring wc;
	wc.resize(cSize);

	size_t cSize1;
	mbstowcs_s(&cSize1, (wchar_t*)&wc[0], cSize, string.c_str(), cSize);

	wc.pop_back();

	return wc;
}

bool StringUtilities::PtrToWchIsEqual(WCHAR* w1, WCHAR* w2, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		if (w1[i] != w2[i])
		{
			return false;
		}
	}

	return true;
}