#pragma once
#include <array>

namespace Utils
{
	template<size_t Size>
	struct StackBuffer
	{
		template<typename T>
		T* As()
		{
			static_assert(sizeof(T) <= Size);
			return reinterpret_cast<T*>(buffer.data());
		}
		std::array<unsigned char, Size> buffer{};
	};
}