#pragma once

namespace Utils
{
    template<typename TOnExit>
    struct ScopeExit
    {
        explicit ScopeExit(TOnExit&& onExit)
            : mOnExit(std::forward<TOnExit>(onExit))
        {
        }
        ~ScopeExit()
        {
            mOnExit();
        }
    private:
        TOnExit mOnExit;
    };
}