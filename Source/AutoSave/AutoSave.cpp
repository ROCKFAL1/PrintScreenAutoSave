#include "AutoSave.hpp"

std::string screenshotFolder;

HDC hDC, compDC;

void AutoSave::InitAutoSave(std::string folder)
{
	screenshotFolder = folder;
	hDC = GetDC(NULL);
	compDC = CreateCompatibleDC(hDC);
}

//TODO
void AutoSave::UpdateScreenFolder(std::string newFolder)
{
	screenshotFolder = newFolder;
}

void AutoSave::SaveScreen()
{
	if (!OpenClipboard(NULL))
	{
		return;
	}

	BITMAP bmp;	
	HBITMAP hBitmap;
	SYSTEMTIME sysTm;
	PBITMAPINFO pbmi;
	BITMAPFILEHEADER bmFileHeader;
	WORD clrBits;
	DWORD dwTmp;
	LPBYTE lpBits;
	HANDLE hFile;

	hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	
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
	std::string screenshotPath = screenshotFolder
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

	hFile = CreateFile(&screenshotPath[0], GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	bmFileHeader.bfType = 0x4d42;
	bmFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + pbmi->bmiHeader.biSize + pbmi->bmiHeader.biClrUsed * sizeof(RGBQUAD) + pbmi->bmiHeader.biSizeImage;
	bmFileHeader.bfReserved1 = 0;
	bmFileHeader.bfReserved2 = 0;
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + pbmi->bmiHeader.biSize + pbmi->bmiHeader.biClrUsed * sizeof(RGBQUAD);

	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, 0);
	WriteFile(hFile, pbmi, sizeof(BITMAPINFOHEADER) + pbmi->bmiHeader.biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, 0);
	WriteFile(hFile, lpBits, pbmi->bmiHeader.biSizeImage, &dwTmp, 0);

	CloseHandle(hFile);
	CloseClipboard();
	DeleteObject(hBitmap);
	VirtualFree(lpBits, 0, MEM_RELEASE);
	delete[sizeof(BITMAPINFOHEADER)] pbmi;
}
