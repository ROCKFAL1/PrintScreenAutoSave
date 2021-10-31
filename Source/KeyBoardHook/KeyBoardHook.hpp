#pragma once

#include <functional>
#include <Windows.h>

void HookKeyboard(std::function<void(int)> callbackFn);
