#pragma once
#include <chrono>

namespace Utils
{
    class Timer
    {
    public:
        void FromMilliseconds(std::chrono::milliseconds milliseconds)
        {
            Reset();
            mActiveTimeDuration = milliseconds;
        }
        void Reset()
        {
            mActiveTimeDuration = std::chrono::milliseconds(0);
            mIsActive = false;
            mLastTimePoint = std::chrono::system_clock::time_point{};
        }
        void Start()
        {
            if (mIsActive)
            {
                return;
            }
            mIsActive = true;
            mLastTimePoint = std::chrono::system_clock::now();
        }
        void Pause() {
            if (!mIsActive)
            {
                return;
            }
            UpdateElapsedTimeDuration();
            mIsActive = false;
        }
        void UpdateElapsedTimeDuration()
        {
            if (!mIsActive)
            {
                return;
            }
            auto timeNow = std::chrono::system_clock::now();
            mActiveTimeDuration += std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - mLastTimePoint);
            mLastTimePoint = timeNow;
        }
        std::chrono::milliseconds GetElapsedTime()
        {
            UpdateElapsedTimeDuration();
            return mActiveTimeDuration;
        }
    private:
        std::chrono::milliseconds mActiveTimeDuration{};
        std::chrono::system_clock::time_point mLastTimePoint{};
        bool mIsActive{false};
    };
}