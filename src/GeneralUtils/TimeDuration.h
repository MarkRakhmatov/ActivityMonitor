#pragma once
#include <chrono>

namespace Utils
{
    struct TimeDuration
    {
        std::chrono::hours hours;
        std::chrono::minutes minutes;
        std::chrono::seconds seconds;
        std::chrono::milliseconds milliseconds;
        std::chrono::microseconds microseconds;
        std::chrono::nanoseconds nanoseconds;
    };

    template<typename DurationType>
    TimeDuration DurationToTimeDuration(DurationType duration)
    {
        TimeDuration resultDuration{};
        std::chrono::nanoseconds nanoseconds(duration);
        resultDuration.hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        nanoseconds -= resultDuration.hours;
        resultDuration.minutes = std::chrono::duration_cast<std::chrono::minutes>(nanoseconds);
        nanoseconds -= resultDuration.minutes;
        resultDuration.seconds = std::chrono::duration_cast<std::chrono::seconds>(nanoseconds);
        nanoseconds -= resultDuration.seconds;
        resultDuration.milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(nanoseconds);
        nanoseconds -= resultDuration.milliseconds;
        resultDuration.microseconds = std::chrono::duration_cast<std::chrono::microseconds>(nanoseconds);
        nanoseconds -= resultDuration.microseconds;
        resultDuration.nanoseconds = nanoseconds;
        return resultDuration;
    }
}
