#include "wx/setup.h"
#include "wx/wx.h"
#include <wx/grid.h>
#include "MainFrame.h"

#include "ActivityMonitorLib/StatisticsHandler.h"
#include "GeneralUtils/CompileTime.h"
#include <future>


struct ApplicationUIData
{
    std::string applicationName;
    std::chrono::milliseconds time;
};

struct UiTable
{
    void ActiveTimeChanged(const std::string& appName, std::chrono::milliseconds newTime)
    {
        // find app position

        auto newPosition = FindNewPosition(newTime);
        newPosition = mUiData.insert(newPosition, ApplicationUIData{ appName, newTime });

        auto oldPosition = FindOldPosition(newPosition, appName);
        if (oldPosition == mUiData.end())
        {
            return;
        }
        mUiData.erase(oldPosition);
    }
    std::list<ApplicationUIData>::iterator FindOldPosition(std::list<ApplicationUIData>::iterator& newPosition, const std::string& appName)
    {
        for (auto it = ++newPosition; it != mUiData.end(); ++it)
        {
            if (it->applicationName == appName)
            {
                return it;
            }
        }
        return mUiData.end();
    }
    std::list<ApplicationUIData>::iterator FindNewPosition(std::chrono::milliseconds newTime)
    {
        auto newPosition = mUiData.end();

        while (newPosition != mUiData.begin())
        {
            --newPosition;
            if (newPosition->time > newTime)
            {
                ++newPosition;
                break;
            }
        }
        return newPosition;
    }
    std::list<ApplicationUIData> mUiData;
};

class ActivityMonitorApp : public wxApp
{
public:
    virtual bool OnInit()override;
    int OnExit() override;
    bool ProcessIdle() override;
    void UpdateGrid();
private:
    MainFrame* mMainFrame;
    wxGrid* mGrid;
    int mGridSize;
    UiTable mAppTimesTable;
    AccessControl::StatisticsHandler mStatisticHandler;
    std::atomic_bool mDone = false;
    std::future<void> mGridUpdateFuture;
};
