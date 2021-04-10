#pragma once
#include "GeneralUtils/Timer.h"
#include "GeneralUtils/CompileTime.h"

namespace AccessControl
{
    template<typename TrackingElementType>
    class StatisticUnit
    {
    public:
        StatisticUnit() = default;
        StatisticUnit(TrackingElementType&& element)
            : mTrackingElement(std::forward<TrackingElementType>(element))
        {
        }

        StatisticUnit(const TrackingElementType& element)
            : mTrackingElement(element)
        {
        }
        template<typename ... ArgsTypes>
        void Activate(ArgsTypes ... args)
        {
            mTimer.Start();
            if constexpr (IsActivateExists<TrackingElementType>::value())
            {
                mTrackingElement.Activate(std::forward<ArgsTypes>(args)...);
            }
        }
        template<typename ... ArgsTypes>
        void Deactivate(ArgsTypes ... args)
        {
            mTimer.Pause();
            if constexpr (IsDeactivateExists<TrackingElementType>::value())
            {
                mTrackingElement.Deactivate(std::forward<ArgsTypes>(args)...);
            }
        }
        std::chrono::milliseconds GetActiveTime()
        {
            return mTimer.GetElapsedTime();
        }
        void SetTrackingElement(TrackingElementType&& updatedElement)
        {
            mTrackingElement = std::move(updatedElement);
        }
        const TrackingElementType& GetTrackingElement() const
        {
            return mTrackingElement;
        }
    private:
        DECL_MEMBER_EXISTANSE_CHECKER(Deactivate);
        DECL_MEMBER_EXISTANSE_CHECKER(Activate);
        TrackingElementType mTrackingElement;
        Utils::Timer mTimer;
    };
}  