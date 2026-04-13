#pragma once

#include <cstdint>
#include <functional>
#include <unordered_set>

#include <mutex>
#include <cstring> //memcmp
#include <ranges>

namespace BBC{
    struct Address{
        union{
            std::size_t address;
            void* pointer;
        };

        constexpr Address() : address{0} {}
        constexpr Address(std::size_t addr)
        : address{addr}
        {}
        template<typename T>
        constexpr Address(T* ptr)
        : pointer{reinterpret_cast<void*>(ptr)}
        {}

        //if memcmp is constexpr, this funciton can be constepxr
        bool operator==(Address other) const{
            //this avoids active union member problem
            return memcmp(this, &other, sizeof(std::size_t)) == 0;
        }

        template<typename T>
        T* reinterpret() {
            return reinterpret_cast<T*>(pointer);
        }
        template<typename T>
        T const* reinterpret() const {
            return reinterpret_cast<T const*>(pointer);
        }

        template<typename T>
        static T* r_cast(Address addr){
            return reinterpret_cast<T*>(addr.pointer);
        }
    };
} //namespace BBC

template <>
struct std::hash<BBC::Address> {
    std::size_t operator()(const BBC::Address& a) const noexcept {
        return std::hash<std::size_t>{}(a.address);
    }
};

namespace BBC{

    //this is buffers, command pools, and images
    template<typename T>
    struct ResourceTracker{
        std::mutex mut;
        std::unordered_set<Address> resources;

        void Add(T* res){
            mut.lock();
            resources.insert(res);
            mut.unlock();
        }
        void Remove(T* res){
            mut.lock();
            resources.erase(res);
            mut.unlock();
        }

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using value_type = T*;
            using difference_type = std::ptrdiff_t;
            using pointer = T**;
            using reference = T*&;

            std::unordered_set<Address>::iterator it;

            T* operator*() const { return static_cast<T*>(it->pointer); }
            Iterator& operator++() { ++it; return *this; }
            bool operator!=(const Iterator& other) const { return it != other.it; }
        };

        auto begin() { return Iterator{ resources.begin() }; }
        auto end()   { return Iterator{ resources.end() };   }
    };
    
} //namespace BBC