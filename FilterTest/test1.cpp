#include <memory>
#include <cmath>
#include <cstring>
#include <emmintrin.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>

template <typename T>
size_t filterMethod(T * res, const uint8_t * filter, size_t size, const T * data)
{
    T * begin_res = res;
    const uint8_t * filt_pos = filter;
    const uint8_t * filt_end = filt_pos + size;
    const T * data_pos = data;

    while (filt_pos < filt_end)
    {
        if (*filt_pos)
        {
            *res = *data_pos;
            res++;
        }

        ++filt_pos;
        ++data_pos;
    }
    return res - begin_res;
}

template <typename T>
size_t filterSSE(T * res, const uint8_t * filter, size_t size, const T * data)
{
    const T * begin_res = res;
    const uint8_t * filt_pos = filter;
    const T * data_pos = data;

    static constexpr size_t SIMD_BYTES = 16;
    const __m128i zero16 = _mm_setzero_si128();
    const uint8_t * filt_end_sse = filt_pos + size / SIMD_BYTES * SIMD_BYTES;

    while (filt_pos < filt_end_sse)
    {
        uint16_t mask = _mm_movemask_epi8(_mm_cmpeq_epi8(_mm_loadu_si128(reinterpret_cast<const __m128i *>(filt_pos)), zero16));
        mask = ~mask;

        if (0 == mask)
        {
        }
        else if (0xFFFF == mask)
        {
            memcpy(res, data_pos, 16 * sizeof(T));
            res += 16;
        }
        else
        {
            for (size_t i = 0; i < SIMD_BYTES; ++i)
                if (filt_pos[i])
                {
                    *res = data_pos[i];
                    res++;
                }
        }

        filt_pos += SIMD_BYTES;
        data_pos += SIMD_BYTES;
    }
    return res - begin_res;
}



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

//int main(int argc, char ** argv) {
//    if (argc < 3)
//    {
//        std::cout << "error" << std::endl;
//        return 0;
//    }
//
//    int use_sse = atoi(argv[1]);
//    size_t size = atoi(argv[2]);
//
//    uint32_t * res = new uint32_t[size];
//    uint8_t * filt = new uint8_t[size];
//    uint32_t * data = new uint32_t[size];
//    for (size_t i = 0; i < size; ++i)
//    {
//        if (i % 32 < 16)
//            filt[i] = 1;
//        else
//            filt[i] = gen_random() % 2;
//        data[i] = gen_random();
//    }
//
//    const auto start = std::chrono::high_resolution_clock::now();
//    size_t res_size;
//    if (use_sse)
//        res_size = filterSSE(res, filt, size, data);
//    else
//        res_size = filter(res, filt, size, data);
//    const std::chrono::duration<double> diff =
//            std::chrono::high_resolution_clock::now() - start;
//    std::cout << "Time: " << std::fixed << std::setprecision(6) << diff.count()
//              << " sec " << std::endl;
//
//    std::cout << "res_size: " << res_size << std::endl;
//
//    delete[] res;
//    delete[] filt;
//    delete[] data;
//
//}

typedef struct Row
{
    union
    {
        int32_t a[2]; /// a, b
        uint64_t c;
    };
} Row;
// ((b >= 10 && b < 50) && * (a == 1000 || a == 2000 || a == 3000))


Row target1;
Row target2;
Row target3;
Row target1_end;
Row target2_end;
Row target3_end;

void initTarget()
{
    target1.a[0] = 1000;
    target1.a[1] = 10;

    target1_end.a[0] = 1000;
    target1_end.a[1] = 50;

    target2.a[0] = 2000;
    target2.a[1] = 10;

    target2_end.a[0] = 2000;
    target2_end.a[1] = 50;

    target3.a[0] = 3000;
    target3.a[1] = 10;

    target3_end.a[0] = 3000;
    target3_end.a[1] = 50;
}



uint8_t * test1(uint8_t * filter, const Row *rows, int nrows)
{
    size_t size = nrows;
    size_t i = 0;
    while (i < size)
    {
        if ((rows[i].c >= target1.c && rows[i].c < target1_end.c) || (rows[i].c >= target2.c && rows[i].c < target2_end.c) || (rows[i].c >= target3.c && rows[i].c < target3_end.c))
            filter[i] = 1;
        else
            filter[i] = 0;
        ++i;
    }

    return filter;
}

size_t test11(Row * res, const Row *rows, int nrows)
{
    size_t size = nrows;
    size_t i = 0;
    while (i < size)
    {
        if ((rows[i].c >= target1.c && rows[i].c < target1_end.c) || (rows[i].c >= target2.c && rows[i].c < target2_end.c) || (rows[i].c >= target3.c && rows[i].c < target3_end.c))
            res[i] = rows[i];

        ++i;
    }

    return i;
}

int main(int argc, char ** argv) {
    if (argc < 4)
    {
        std::cout << "param size error" << std::endl;
        return 0;
    }
    initTarget();

    int use_sse = atoi(argv[1]);
    size_t size = atoi(argv[2]);
    size_t target_data_ratio = atoi(argv[3]);

    std::cout << gen_random1(target_data_ratio) << "------------" << gen_random1(target_data_ratio) << std::endl;
    std::cout << gen_random1(target_data_ratio) << "------------" << gen_random1(target_data_ratio) << std::endl;
    std::cout << gen_random1(target_data_ratio) << "------------" << gen_random1(target_data_ratio) << std::endl;
    std::cout << gen_random() << "------------" << gen_random() << std::endl;
    std::cout << gen_random() << "------------" << gen_random() << std::endl;
    std::cout << gen_random() << "------------" << gen_random() << std::endl;

    Row * data = new Row[size];
    for (size_t i = 0; i < size; ++i)
    {
        if (gen_random1(target_data_ratio) == 1)
        {
            data[i].c = target1.c;
        }
        else
        {
            data[i].c = gen_random();
        }
    }

    auto * filter = new uint8_t[size];
    Row * res = new Row[size];
    size_t res_size;
    const auto start = std::chrono::high_resolution_clock::now();
    if (use_sse == 2)
    {
        res_size = test11(res, data, size);
    }
    else
    {
        filter = test1(filter, data, size);

        if (use_sse)
            res_size = filterSSE(res, filter, size, data);
        else
            res_size = filterMethod(res, filter, size, data);
    }

    const std::chrono::duration<double> diff =
            std::chrono::high_resolution_clock::now() - start;
    std::cout << "Time: " << std::fixed << std::setprecision(6) << diff.count()
              << " sec " << std::endl;

    std::cout << "res_size: " << res_size << std::endl;

    delete[] res;
    delete[] filter;
    delete[] data;

}

