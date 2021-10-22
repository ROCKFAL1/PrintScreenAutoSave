#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>

#include "KeyBoardHook/KeyBoardHook.hpp"


std::string SCREENSHOT_FOLDER;

const int   PRTSCR_KEY = 44;

int width, heigth;

void OnPrntScrPressed(int);
void SaveScreen(HBITMAP);
wchar_t* СonvertCharArrayToLPCWSTR(const char*);

int main()
{
	width = GetSystemMetrics(SM_CXSCREEN);
	heigth = GetSystemMetrics(SM_CYSCREEN);

#if NDEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	// Looking for the system disk
	char systemDirectory[MAX_PATH];
	GetSystemDirectoryA(systemDirectory, MAX_PATH);
	std::string systemDisk;
	systemDisk.push_back(systemDirectory[0]);

#if !NDEBUG
	std::cout << "System dir: " << systemDirectory << std::endl;
	std::cout << "System disk: " << systemDisk << std::endl;
#endif

	// Getting final path
	SCREENSHOT_FOLDER = systemDisk + ":\\Users\\Public\\Pictures\\PrintScreenAutoSave\\";
	std::filesystem::create_directory(SCREENSHOT_FOLDER);

#if !NDEBUG
	std::cout << SCREENSHOT_FOLDER << std::endl;
#endif

	HookKeyboard(OnPrntScrPressed);
	MSG mes;
	while (true) GetMessage(&mes, NULL, 0, 0);
}

void OnPrntScrPressed(int keyCode)
{
	if (keyCode != PRTSCR_KEY)
	{
		return;
	}

	if (!OpenClipboard(NULL))
	{
		std::cout << "Open ERROR!" << std::endl;
		return;
	}

	BITMAP bm;
    HBITMAP hBm = (HBITMAP)GetClipboardData(CF_BITMAP);

	SaveScreen(hBm);

	CloseClipboard();
}

void SaveScreen(HBITMAP hBitmap)
{
	BITMAP bmp;
	HDC hDC, compDC;
	SYSTEMTIME sysTm;
	PBITMAPINFO pbmi; 
	BITMAPFILEHEADER bmFileHeader;
	WORD clrBits;
	DWORD dwTmp;
	LPBYTE lpBits;
	HANDLE hFile;
	
	hDC = GetDC(NULL);
	compDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, width, heigth);
	SelectObject(compDC, hBitmap);
	BitBlt(compDC, 0, 0, width, heigth, hDC, 0, 0, SRCCOPY);
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);

	clrBits = (WORD)bmp.bmPlanes * bmp.bmBitsPixel;
	if (clrBits <= 16)
	{
		clrBits = 16;
	}
	else
	{
		if (clrBits <= 24)
		{
			clrBits = 24;
		}
		else
		{
			clrBits = 32;
		}
	}

	pbmi = (PBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER)];
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (clrBits < 24)
	{
		pbmi->bmiHeader.biClrUsed = 2 ^ clrBits;
	}
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth) * pbmi->bmiHeader.biHeight * (clrBits / 8);
	pbmi->bmiHeader.biClrImportant = 0;
	lpBits = (UCHAR*)VirtualAlloc(NULL, pbmi->bmiHeader.biSizeImage, MEM_TOP_DOWN | MEM_COMMIT, PAGE_READWRITE);
	GetDIBits(compDC, hBitmap, 0, pbmi->bmiHeader.biHeight, lpBits, pbmi, DIB_RGB_COLORS);

	GetLocalTime(&sysTm);
	std::string screenshotPath = SCREENSHOT_FOLDER
		+ "screenshot"
		+ std::to_string(sysTm.wYear)
		+ "_" + std::to_string(sysTm.wMonth)
		+ "_" + std::to_string(sysTm.wDay)
		+ "_" + std::to_string(sysTm.wHour)
		+ "_" + std::to_string(sysTm.wMinute);

	int counter = 1;

	if (std::filesystem::exists(screenshotPath + ".png"))
	{
		while (std::filesystem::exists(screenshotPath + "(" + std::to_string(counter) + ")" + ".png"))
		{
			counter++;
		}
		screenshotPath = screenshotPath + "(" + std::to_string(counter) + ")";
	}

	screenshotPath = screenshotPath + ".png";

	hFile = CreateFile(СonvertCharArrayToLPCWSTR(&screenshotPath[0]), GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	bmFileHeader.bfType = 0x4d42;
	bmFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + pbmi->bmiHeader.biSize + pbmi->bmiHeader.biClrUsed * sizeof(RGBQUAD) + pbmi->bmiHeader.biSizeImage;
	bmFileHeader.bfReserved1 = 0;
	bmFileHeader.bfReserved2 = 0;
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + pbmi->bmiHeader.biSize + pbmi->bmiHeader.biClrUsed * sizeof(RGBQUAD);

	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, 0);
	WriteFile(hFile, pbmi, sizeof(BITMAPINFOHEADER) + pbmi->bmiHeader.biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, 0);
	WriteFile(hFile, lpBits, pbmi->bmiHeader.biSizeImage, &dwTmp, 0);

	CloseHandle(hFile);
	DeleteDC(compDC);
	DeleteObject(hBitmap);
	VirtualFree(lpBits, 0, MEM_RELEASE);
	delete pbmi;
}

wchar_t* СonvertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, MAX_PATH);
	return wString;
}