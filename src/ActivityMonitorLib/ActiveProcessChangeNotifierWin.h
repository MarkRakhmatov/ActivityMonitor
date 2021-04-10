#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <filesystem>
#include <functional>
#include <set>
#include <thread>
#include "WindowsUtils/ProcessWrapper.h"
#include "WindowsUtils/HandleGuard.h"
#include "WindowsUtils/TokenWrapper.h"
#include "WindowsUtils/ActiveProcessHelper.h"
#include "TrackingProcessInfo.h"

namespace AccessControl
{
    using ActiveProcessChangeCallbackType = std::function<void(const std::string&, TrackingProcessInfo&&)>;

    class ActiveProcessChangeNotifierWin
    {
    public:
        void Start(ActiveProcessChangeCallbackType&& onProcessChanged)
        {
            mOnProcessChanged = std::move(onProcessChanged);
            mWorkerThread = std::thread([this]() {
                ProcessChangeChecker(); 
            });
            
        }
        void Stop()
        {
            mDone = true;
            if (mWorkerThread.joinable())
            {
                mWorkerThread.join();
            }
        }
        ~ActiveProcessChangeNotifierWin()
        {
            Stop();
        }
    private:
        void ProcessChangeChecker()
        {
            std::string lastProcessName;
            while (!mDone)
            {
                auto sleepTime = std::chrono::milliseconds(200);
                std::this_thread::sleep_for(sleepTime);
                auto activeProcessInfo = Utils::WinApi::ActiveProcessHelper::GetActiveWindowProcessInfo();
                std::string processName = activeProcessInfo.process.GetProcessName();
                if (lastProcessName == processName)
                {
                    continue;
                }
                
                mOnProcessChanged(lastProcessName, TrackingProcessInfo{
                    activeProcessInfo.process.GetProcessId(),
                    processName,
                    activeProcessInfo.process.GetBinaryPath()
                });
                lastProcessName = processName;
            }
            lastProcessName = lastProcessName;
        }
    private:
        ActiveProcessChangeCallbackType mOnProcessChanged;
        std::atomic_bool mDone = false;
        std::thread mWorkerThread;
    };
}