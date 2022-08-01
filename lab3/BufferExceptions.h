//
// Created by dnbyyyy on 11.06.2022.
//

#ifndef LAB3_BUFFEREXCEPTIONS_H
#define LAB3_BUFFEREXCEPTIONS_H

#pragma once

#include <exception>

class buffer_underflow_exception : public std::exception {
    const char* what() const _NOEXCEPT override {
            return "Buffer underflow";
    }
};

class index_out_of_bounds_exception : public std::exception {
    const char* what() const _NOEXCEPT override {
            return "Index out of bounds";
    }
};

#endif //LAB3_BUFFEREXCEPTIONS_H
