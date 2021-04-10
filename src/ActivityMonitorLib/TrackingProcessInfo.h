#include <string>
#include <filesystem>
#include <chrono>

namespace AccessControl
{
    struct TrackingProcessInfo
    {
        long int processId;
        std::string name;
        std::filesystem::path binaryPath;
        std::chrono::milliseconds activeTime;
    };
}