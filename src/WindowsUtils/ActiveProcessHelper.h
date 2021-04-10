#pragma once
#include <string>
#include "HandleGuard.h"
#include "ProcessWrapper.h"

namespace Utils::WinApi
{
    struct ActiveProcessInfo
    {
        ProcessWrapper process;
        HWND window;
    };
    class ActiveProcessHelper
    {
    public:
        static WindowHandleGuard GetActiveWindow()
        {
            return WindowHandleGuard(::GetForegroundWindow());
        }
        static DWORD GetActiveWindowPID()
        {
            HWND activeWnd(::GetForegroundWindow());
            DWORD procId{};
            if (!::GetWindowThreadProcessId(activeWnd, &procId))
            {
                return {};
            }
            return procId;
        }
        static std::string GetActiveWindowName()
        {
            auto procId = GetActiveWindowPID();

            ProcessWrapper proc;
            if (!proc.Initialize(::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, procId)))
            {
                return "";
            }
            return proc.GetProcessName();
        };
        static ActiveProcessInfo GetActiveWindowProcessInfo()
        {
            HWND activeWnd(::GetForegroundWindow());
            DWORD procId{};
            if (!::GetWindowThreadProcessId(activeWnd, &procId))
            {
                return {};
            }

            Utils::WinApi::ProcessWrapper proc;
            if (!proc.Initialize(::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, procId)))
            {
                return {};
            }
            return { std::move(proc),  std::move(activeWnd) };
        };
    };
}