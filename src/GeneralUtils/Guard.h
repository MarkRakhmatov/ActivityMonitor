#pragma once
#include <type_traits>

namespace Utils
{
    template<typename T, typename TDeleter, TDeleter VDeleter>
    class LightGuard
    {
    public:
        LightGuard() = default;
        explicit LightGuard(T data)
            : mData(std::forward<T>(data))
            , mState(EState::Initialized)
        {
        }

        LightGuard(LightGuard&& other) noexcept
            : mState(other.mState)
            , mData(other.Release())
        {
        }
        LightGuard& operator=(LightGuard&& other) noexcept
        {
            mState = other.mState;
            mData = other.Release();
            return *this;
        }
        ~LightGuard()
        {
            if (mState == EState::Initialized)
            {
                VDeleter(mData);
            }
        }
        T& Get()
        {
            return mData;
        }
        const T& Get() const
        {
            return mData;
        }
        T Release()
        {
            mState = EState::Released;
            return mData;
        }
        operator bool()
        {
            return mState == EState::Initialized;
        }
    private:
        enum class EState 
        {
            Initialized,
            NotInitialized, 
            Released
        };
        EState mState{ EState::NotInitialized };
        T mData;
    };
}