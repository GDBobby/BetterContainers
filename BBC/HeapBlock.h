#pragma once

#include <cstdint>
#include <concepts>
#include <bitset>
#include <memory>

#include "MemoryHelpers.h"

#include <cstring> //memcpy

#include "ExplicitCopyMove.h"

//UNFINISHED

namespace BBC{
	
	
    template<typename T>
    struct HeapBlock : public MemoryHelper_Construction<HeapBlock<T>, T> {
	private:
        std::allocator<T> allocator{};
	public:
        T* memory;
		

		//DO NOT CHANGE EXTERNALLY
		std::size_t size; //DO NOT CHANGE EXTERNALLY
	public:
	
		[[nodiscard]] HeapBlock() 
		: memory{nullptr}, size{0}
		{}

        [[nodiscard]] explicit HeapBlock(std::size_t _size) 
		: size{_size}
		{
			if(size == 0){
				memory = nullptr;	
			}
			else{
            	memory = allocator.allocate(size);
			}
        }
		~HeapBlock(){
			if(size != 0){
				allocator.deallocate(memory, size);
			}
		}

		void Clear(){
			if(size != 0){
				allocator.deallocate(memory, size);
			}
			size = 0;
			memory = nullptr;
		}
		
		void Resize(std::size_t count){
			if(count != size){
				if(size != 0){
					allocator.deallocate(memory, count);
				}
				memory = allocator.allocate(count);
				size = count;
			}
		}

		//from here on is explicit move and assignment
	private:
		//the memory should not be allocated from outside, this is not a view
		[[nodiscard]] explicit HeapBlock(T* _memory, std::size_t _size) noexcept
		: memory{_memory},
			size{_size}
		{}
	public:
    };


	template<typename T>
	struct CopyConstruct<HeapBlock<T>>{
		static HeapBlock<T>& Ex(HeapBlock<T>* construction_addr, HeapBlock<T> const& copySrc){
			T* ret_memory;
			//HeapBlock ret = //if i do this, I need an internal move constructor
			ret_memory = copySrc.allocator.allocate(copySrc.size);
			memcpy(ret_memory, copySrc.memory, copySrc.SizeInBytes());
			std::construct_at(construction_addr, ret_memory, copySrc.size);
			return *construction_addr;
		}
	};
    template<typename T>
	struct MoveConstruct<HeapBlock<T>>{
		static HeapBlock<T>& Ex(HeapBlock<T>* construction_addr, HeapBlock<T>&& moveSrc) noexcept{
			T* ret_memory = moveSrc.memory;
			std::size_t ret_size = moveSrc.size;
			moveSrc.memory = nullptr;
			moveSrc.size = 0;
			std::construct_at(construction_addr, ret_memory, ret_size);
			return *construction_addr;
		}
	};
	template<typename T>
	struct CopyAssign<HeapBlock<T>>{
		static HeapBlock<T>& Ex(HeapBlock<T>& lh, HeapBlock<T> const& rh){
			if(rh.size == 0){
				lh.Clear();
				return lh;
			}

			lh.Resize(rh.size);
			
			lh.size = rh.size;
			memcpy(lh.memory, rh.memory, lh.SizeInBytes());
			return lh;
		}
	};
    template<typename T>
	struct MoveAssign<HeapBlock<T>>{
		static HeapBlock<T>& Ex(HeapBlock<T>& lh, HeapBlock<T>&& rh) noexcept {
			if(lh.size != 0){
				lh.allocator.deallocate(lh.memory, lh.size);
			}
			lh.memory = rh.memory;
			lh.size = rh.size;
			rh.memory = nullptr;
			rh.size = 0;

			return lh;
		}
	};
	
	//this wont control construction or deconstruction, just the memory itself
}