#include "KeyBoardHook.hpp"

HHOOK					 _hook;
KBDLLHOOKSTRUCT			 _kbHookStruct;
std::function<void(int)> _callbackFn;

LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(_hook, nCode, wParam, lParam);
	}

	if (wParam == WM_KEYDOWN)
	{
		_kbHookStruct = *((KBDLLHOOKSTRUCT*)lParam);
		_callbackFn(_kbHookStruct.vkCode);
	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void HookKeyboard(std::function<void(int)> callbackFn)
{
	_callbackFn = callbackFn;

	_hook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0);	
}


