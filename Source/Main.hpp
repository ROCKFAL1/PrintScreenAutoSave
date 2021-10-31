#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <Windows.h>

#include "KeyBoardHook/KeyBoardHook.hpp"
#include "FileSystemHelper/FileSystemHelper.hpp"
#include "AutoSave/AutoSave.hpp"
#include "Registry/RegSettings.hpp"
#include "Registry/RegAutorun.hpp"

#pragma comment (lib, "tray.lib")
#include "tray.hpp"

const int   PRTSCR_KEY = 44;

HINSTANCE hInstance;
HWND hwnd = nullptr;
WNDCLASSEX clipboardHandlerWin;

bool printScreenPressedFlag = false;


void PrintScreenHandler(int keyCode);
LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);