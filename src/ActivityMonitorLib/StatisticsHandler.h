#pragma once
#include <map>
#include "StatisticUnit.h"
#include "ActiveProcessChangeNotifierWin.h"
#include <thread>
#include <functional>
#include <mutex>

namespace AccessControl
{
    enum class EStatisticsHandlerStatus
    {
        Success,
        Fail
    };

    using StatUnit = StatisticUnit<TrackingProcessInfo>;
    using StatUnitsMap = std::map<std::string, StatUnit>;
    using OnActiveProcessTimeChangeFunc = std::function<void(StatUnitsMap&, const std::string&)>;

    class StatisticsHandler
    {
    public:
        void Start()
        {
            auto OnActiveWindowChangedFunc = [this](const std::string& prevProcessName, TrackingProcessInfo&& currentProcess) {
                OnActiveWindowChanged(prevProcessName, std::move(currentProcess));
            };
            mNotifier.Start(std::move(OnActiveWindowChangedFunc));
        }
        void Stop()
        {
            mNotifier.Stop();
        }
        auto GetStatistics()
        {
            std::unique_lock lock(mStatisticMutex);
            return mStatisticUnits;
        }
        StatUnit GetActiveProcessInfo()
        {
            std::unique_lock lock(mStatisticMutex);
            return mStatisticUnits[mActiveProcessName];
        }
        void OnActiveWindowChanged(const std::string& prevProcessName, TrackingProcessInfo&& currentProcInfo)
        {
            std::unique_lock lock(mStatisticMutex);
            mActiveProcessName = currentProcInfo.name;
            mStatisticUnits[prevProcessName].Deactivate();
            auto& activeProcess = mStatisticUnits[mActiveProcessName];
            activeProcess.SetTrackingElement(std::move(currentProcInfo));
            activeProcess.Activate();
        }
    private:
        OnActiveProcessTimeChangeFunc mOnActiveProcessTimeChange;
        std::mutex mStatisticMutex;
        std::map<std::string, StatUnit> mStatisticUnits{};
        std::string mActiveProcessName;
        // TODO replace with interface
        ActiveProcessChangeNotifierWin mNotifier;
    };
}