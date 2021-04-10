#pragma once
#include <Windows.h>
#include "GeneralUtils/Guard.h"

namespace Utils::WinApi
{
    static void CloseHandleChecked(HANDLE handle)
    {
        if (handle != NULL && handle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(handle);
        }
    }
    static void CloseWindowHandleChecked(HWND handle)
    {
        if (handle != NULL)
        {
            ::CloseWindow(handle);
        }
    }

    using HandleGuard = Utils::LightGuard<HANDLE, decltype(CloseHandleChecked)*, &CloseHandleChecked>;
    using WindowHandleGuard = Utils::LightGuard<HWND, decltype(CloseWindowHandleChecked)*, &CloseWindowHandleChecked>;
}