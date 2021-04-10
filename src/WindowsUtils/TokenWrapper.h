#pragma once
#include <string>
#include "HandleGuard.h"
#include "GeneralUtils/StackBuffer.h"

namespace Utils::WinApi
{

    class TokenWrapper
    {
    public:
        explicit TokenWrapper(HANDLE token)
            : mToken(token)
        {
        }
        Utils::StackBuffer<512> GetTokenPrivileges()
        {
            Utils::StackBuffer<512> priviligesBuffer{};
            DWORD retDataLength{};
            void* priviligesBufferPtr = priviligesBuffer.As<void*>();
            if (!::GetTokenInformation(
                mToken.Get(),
                TOKEN_INFORMATION_CLASS::TokenPrivileges,
                priviligesBufferPtr,
                static_cast<DWORD>(priviligesBuffer.buffer.size()),
                &retDataLength
            ))
            {
                return {};
            }
            return priviligesBuffer;
        }
        template<typename TCallback>
        bool EnumPrivileges(TCallback&& enumerator)
        {
            auto priviligesBuffer = GetTokenPrivileges();
            const TOKEN_PRIVILEGES* priviliges = priviligesBuffer.As<TOKEN_PRIVILEGES>();
            for (DWORD i = 0; i < priviliges->PrivilegeCount; ++i)
            {
                if (!enumerator(priviliges->Privileges[i]))
                {
                    break;
                }
            }
            return true;
        }
        LUID GetPrivilegeLuid(std::string_view privilegeName)
        {
            LUID luid{};

            if (!::LookupPrivilegeValueA(
                NULL,            // lookup privilege on local system
                privilegeName.data(),   // privilege to lookup 
                &luid))        // receives LUID of privilege
            {
                return {};
            }
            return luid;
        }
        BOOL SetPrivilege(
            std::string_view privilegeName,  // name of privilege to enable/disable
            BOOL bEnablePrivilege   // to enable or disable privilege
        )
        {
            HANDLE hToken = mToken.Get();
            TOKEN_PRIVILEGES tp;
            const LUID luid = GetPrivilegeLuid(privilegeName);
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            if (bEnablePrivilege)
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            else
                tp.Privileges[0].Attributes = 0;
            if (!::AdjustTokenPrivileges(
                hToken,
                FALSE,
                &tp,
                sizeof(TOKEN_PRIVILEGES),
                nullptr,
                nullptr))
            {
                return FALSE;
            }

            if (::GetLastError() == ERROR_NOT_ALL_ASSIGNED)
            {
                return FALSE;
            }

            return TRUE;
        }
    private:
        Utils::WinApi::HandleGuard mToken{ NULL };
    };
}