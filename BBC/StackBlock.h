#pragma once

#include <cstdint>
#include <concepts>
#include <bitset>
#include <memory>

#include "MemoryHelpers.h"

namespace BBC{
	template<typename T, std::size_t Size>
	struct StackBlock : MemoryHelper_Construction<StackBlock<T, Size>, T> {
        static constexpr std::size_t size = Size;
        alignas(T) uint8_t memory[size * sizeof(T)];
    };
} //namespace BBC