#include "KeyBoardHook.hpp"

HHOOK           _hook;
KBDLLHOOKSTRUCT _kbHookStruct;
void            (*_callbackFn)(int);

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

void HookKeyboard(void (*callbackFn)(int))
{
	_callbackFn = callbackFn;

	_hook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0);	
}


