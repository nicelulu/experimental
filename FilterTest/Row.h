#pragma once

#include <chrono>
#include <random>

#define IS_LITTLE_ENDIAN (*(uint16_t *)"\0\xff" > 0x100)

#ifdef IS_LITTLE_ENDIAN
typedef struct __attribute__((packed)) Row
{
    int32_t b;
    int32_t a;
} Row;
#endif

#ifndef IS_LITTLE_ENDIAN
typedef struct Row
{
    int32_t a;
    int32_t b;
} Row;
#endif

struct RowWapper
{
    union
    {
        Row row;
        uint64_t c;
    };
};

RowWapper target1;
RowWapper target2;
RowWapper target3;
RowWapper target1_end;
RowWapper target2_end;
RowWapper target3_end;

uint64_t gen_random() noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0, 1);
    return std::ceil(dis(gen) * UINT64_MAX);
}

uint64_t gen_random1(size_t range) noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0, 1);
    return std::ceil(dis(gen) * range);
}
