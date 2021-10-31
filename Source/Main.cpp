#include "Main.hpp"

int main()
{
#if NDEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	RegSettings regSettings(L"Software", L"PrintScreenAutoSave");

	if ((bool)regSettings.GetParameterValueInt(L"IsInAutorun"))
	{
		RegAutorun::PutOrUpdateInAutorun();
	}
	else
	{
		RegAutorun::DeleteFromAutorun();
	}

	std::string screenshotFolder = FileSystemHelper::GetSystemDiskName() + ":\\Users\\Public\\Pictures\\PrintScreenAutoSave\\";
	std::filesystem::create_directory(screenshotFolder);

	AutoSave::InitAutoSave(screenshotFolder);

	hInstance = GetModuleHandle(NULL);

	SecureZeroMemory(&clipboardHandlerWin, sizeof(WNDCLASSEX));
	clipboardHandlerWin.cbSize = sizeof(WNDCLASSEX);
	clipboardHandlerWin.lpfnWndProc = wndProc;
	clipboardHandlerWin.hInstance = hInstance;
	clipboardHandlerWin.lpszClassName = "ClipboardHandlerWin";

	RegisterClassEx(&clipboardHandlerWin);
	CreateWindowA("ClipboardHandlerWin", "ClipboardHandlerWin", 0, 0, 0, 0, 0,
		NULL, NULL, clipboardHandlerWin.hInstance, NULL);

	//TODO ADD ICON
	Tray::Tray tray("PrintScreenAutoSave", LoadIcon(NULL, IDI_APPLICATION));

	//TODO
	tray.addEntry(Tray::Button("Change screens folder(SOON!)"))->setDisabled(true);

	tray.addEntry(Tray::Toggle(std::string("Put in autorun"),
		(bool)regSettings.GetParameterValueInt(L"IsInAutorun"),
		[&](bool state) { regSettings.SetParameterValueInt(L"IsInAutorun", (int)state);
						  state ? RegAutorun::PutOrUpdateInAutorun() : RegAutorun::DeleteFromAutorun(); }));

	tray.addEntry(Tray::Separator());
	tray.addEntry(Tray::Button("Close", [&] { tray.exit(); }));

	HookKeyboard(PrintScreenHandler);

	MSG msg;
	while (GetMessage(&msg, hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	tray.~Tray();
}

void PrintScreenHandler(int keyCode)
{
	if (keyCode != PRTSCR_KEY)
	{
		return;
	}

	printScreenPressedFlag = true;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			AddClipboardFormatListener(hWnd);
			break;
		}
		case WM_DESTROY:
		{
			RemoveClipboardFormatListener(hWnd);
			break;
		}
		case WM_CLIPBOARDUPDATE:
		{
			if (printScreenPressedFlag)
			{
				AutoSave::SaveScreen();			
			}
			printScreenPressedFlag = false;
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
