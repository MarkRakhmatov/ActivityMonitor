#pragma once
#include <filesystem>
#include <windows.h>
#include <Psapi.h>
#include "WindowsUtils/HandleGuard.h"
#include "WindowsUtils/TokenWrapper.h"
#include "GeneralUtils/Timer.h"

namespace Utils::WinApi
{
    class ProcessWrapper
    {
    public:
        ProcessWrapper() = default;
        ProcessWrapper(ProcessWrapper&&) = default;
        ProcessWrapper& operator=(ProcessWrapper&&) = default;
        [[nodiscard]] bool Initialize(HANDLE procHandle)
        {
            mProcHandle = Utils::WinApi::HandleGuard(procHandle);
            const size_t size = 512;
            std::string path(size, '\0');
            DWORD result = ::GetProcessImageFileNameA(procHandle, path.data(), static_cast<DWORD>(size));
            if (!result)
            {
                return false;
            }
            mPath = path.c_str();
            mName = mPath.filename().string(); 
            return true;
        }
    public:
        const std::string& GetProcessName() const
        {
            return mName;
        }
        long int GetProcessId() const
        {
            return static_cast<long int>(::GetProcessId(mProcHandle.Get()));
        }
        const std::filesystem::path& GetBinaryPath() const
        {
            return mPath;
        }
        HANDLE GetHandle() const
        {
            return mProcHandle.Get();
        }
        HANDLE ReleaseHandle()
        {
            return mProcHandle.Release();
        }
        bool IsAlive() const
        {
            DWORD exitCode;
            if (::GetExitCodeProcess(mProcHandle.Get(), &exitCode) == TRUE)
            {
                return false;
            }
            return ::GetLastError() == STILL_ACTIVE;
        }
    private:
        std::string mName;
        std::filesystem::path mPath;
        Utils::Timer mTimer;
        Utils::WinApi::HandleGuard mProcHandle{ NULL };
    };

    static bool operator < (const ProcessWrapper& lhs, const ProcessWrapper& rhs)
    {
        return lhs.GetHandle() < rhs.GetHandle();
    }
}