#include "ActivityMonitorApp.h"
#include "GeneralUtils/TimeDuration.h"
#include <future>

std::string MillisecondsToString(std::chrono::milliseconds milliseconds)
{
    Utils::TimeDuration timeDuration = Utils::DurationToTimeDuration(milliseconds);
    std::stringstream ss;
    ss << std::to_string(timeDuration.hours.count()) << ":"
        << std::to_string(timeDuration.minutes.count()) << ":"
        << std::to_string(timeDuration.seconds.count());
    return ss.str();
}

bool ActivityMonitorApp::OnInit()
{
    auto onExit = [this]()
    {
        mStatisticHandler.Stop();
        mDone = true;
        mGridUpdateFuture.get();
    };
    mMainFrame = new MainFrame(onExit);

    mGrid = new wxGrid(mMainFrame, wxID_ANY);
    int rowsCount = 3;
    mGridSize = rowsCount;
    int columnsCount = 2;
    mGrid->CreateGrid(rowsCount, columnsCount);
    mGridUpdateFuture = std::async([this]() {
        while (!mDone)
        {
            UpdateGrid();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    mMainFrame->Show(true);
    mStatisticHandler.Start();

    return true;
}

int ActivityMonitorApp::OnExit()
{
    mStatisticHandler.Stop();
    return wxAppBase::OnExit();
}

bool ActivityMonitorApp::ProcessIdle()
{
    return wxAppBase::ProcessIdle();
}

void ActivityMonitorApp::UpdateGrid()
{
    auto& activeProcInfo = mStatisticHandler.GetActiveProcessInfo();
    mAppTimesTable.ActiveTimeChanged(activeProcInfo.GetTrackingElement().name, activeProcInfo.GetActiveTime());
    if (mAppTimesTable.mUiData.size() > mGridSize)
    {
        mGrid->AppendRows();
        ++mGridSize;
    }
    int currentRow = 0;
    for (auto& appInfo : mAppTimesTable.mUiData)
    {
        mGrid->SetCellValue(currentRow, 0, appInfo.applicationName.c_str());
        mGrid->SetCellValue(currentRow, 1, MillisecondsToString(appInfo.time).c_str());
        ++currentRow;
    }
}
