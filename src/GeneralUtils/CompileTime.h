#pragma once
#include <type_traits>

#define DECL_MEMBER_EXISTANSE_CHECKER(funcName) \
    template<typename T> \
    struct Is##funcName##Exists \
    { \
        template<typename T1> \
        static int StubFunction(...); \
        template<typename T1> \
        static char StubFunction(decltype(&T1::##funcName)); \
            \
        static constexpr bool value() \
        { \
            return sizeof(StubFunction<T>(0)) == sizeof(char); \
        } \
    };
