//
// Created by dnbyyyy on 24.05.2022.
//

#ifndef LAB5_CUSTOMALLOCATOR_HPP
#define LAB5_CUSTOMALLOCATOR_HPP

#include <stdexcept>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <cstdint>
#include <cstdlib>

    template<typename T>
    class customAllocator {
    private:
        std::shared_ptr<void> heap;
        std::shared_ptr<void**> freeList;
        std::size_t sizeOfChunk;

    public:

        using value_type = T;
        using pointer = T*;
        
        template<typename U>
        struct rebind {
            using other = customAllocator<U>;
        };

        explicit customAllocator(std::size_t size = 64, std::size_t count = 1024)
                : heap(std::shared_ptr<void>(std::malloc(size * count), [](void* ptr) { std::free(ptr); })),
                  freeList(std::make_shared<void**>(reinterpret_cast<void**>(heap.get()))),
                  sizeOfChunk(size) {

            if (size < sizeof(std::uintptr_t)) {
                throw std::invalid_argument("Chunk size can't be less than pointer size.");
            }

            auto endAddress = reinterpret_cast<char*>(heap.get()) + size * count;
            for (std::size_t i = 0; i < count; ++i) {
                auto current = reinterpret_cast<char*>(heap.get()) + i * size;
                auto next = current + size;
                *reinterpret_cast<void**>(current) = next < endAddress ? next : nullptr;
            }
        }

        bool operator==(const customAllocator& rhs) {
            return heap == rhs.heap;
        }

        const std::shared_ptr<void>& get_heap() const {
            return heap;
        }

        const std::shared_ptr<void**>& get_free_list() const {
            return freeList;
        }

        std::size_t get_block_size() const {
            return sizeOfChunk;
        }

        template<typename U>
        explicit customAllocator(const customAllocator<U>& other) :
                heap(other.get_heap()),
                freeList(other.get_free_list()),
                sizeOfChunk(other.get_block_size()) {
        }

        [[nodiscard]] T* allocate(std::size_t n) {

            if (n * sizeof(T) > sizeOfChunk) {
                using namespace std::string_literals;
                throw std::invalid_argument("Can't allocate more than "s + std::to_string(n) + " elements of size " + std::to_string(sizeof(T)));
            }
            if (*freeList == nullptr) {
                throw std::bad_alloc();
            }

            auto allocating = reinterpret_cast<T*>(*freeList);
            *freeList = reinterpret_cast<void**>(**freeList);

            return allocating;

        }

        void deallocate(T* p, std::size_t) {
            if (p == nullptr) {
                return;
            }

            if (*freeList == nullptr) {
                *freeList = reinterpret_cast<void**>(p);
                **freeList = nullptr;
            }
            else {
                void** temp = *freeList;
                *freeList = reinterpret_cast<void**>(p);
                **freeList = reinterpret_cast<void*>(temp);
            }

        }

    };

#endif //LAB5_CUSTOMALLOCATOR_HPP
