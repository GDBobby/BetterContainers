#pragma once

#include "StackBlock.h"

#include <bitset>
#include <cstdint>
#include <concepts>
#include <optional>

//somewhat specialized
namespace BBC{
	template<typename T, std::size_t _Size>
	struct RingBuffer{
		static constexpr std::size_t Size = _Size;
		
		StackBlock<T, Size> data;
		std::bitset<Size> usage;
		
		std::size_t starting_index = 0;
		
		template<typename... Args>
		requires(std::is_constructible_v<T, Args...>)
		[[nodiscard]] RingBuffer(Args&&... args) {
			data.ConstructAll(std::forward<Args>(args)...);
		}

		[[nodiscard]] RingBuffer()
			requires(std::is_trivially_constructible_v<T>)
		{}

		T* GetNext(){
			std::size_t current_index = starting_index;
			while(usage[current_index]){
				current_index = (current_index + 1) % Size;
				if(current_index == starting_index){
					throw std::runtime_error("BBC::RingBuffer - out of space");
				}
			}
			starting_index = (current_index + 1) % Size;
			usage[current_index] = true;
			return &data[current_index];
		}
		void Return(T* obj){
			for(std::size_t i = 0; i < Size; i++){
				if(obj == &data[i]){
					assert(usage[i]);
					if(!usage[i]){
						throw std::runtime_error("BBC::RingBuffer - returning an object not in use");
					}
					usage[i] = false;
					return;
				}
			}
			std::unreachable();
		}

		auto begin() {return data.begin();}
		auto end() {return data.end();}
	private:
		
		void UpdateIndex(){
			starting_index = (starting_index + 1) % Size;
		}
	};
}