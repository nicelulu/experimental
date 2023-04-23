#include <memory>
#include <cmath>
#include <cstring>
#include <emmintrin.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>


size_t filter(uint32_t * res, const uint8_t * filter, size_t size, const uint32_t * data)
{
    uint32_t * begin_res = res;
    const uint8_t * filt_pos = filter;
    const uint8_t * filt_end = filt_pos + size;
    const uint32_t * data_pos = data;

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

size_t filterSSE(uint32_t * res, const uint8_t * filter, size_t size, const uint32_t * data)
{
    const uint32_t * begin_res = res;
    const uint8_t * filt_pos = filter;
    const uint32_t * data_pos = data;

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
            memcpy(res, data_pos, 16 * 4);
            res += 16;
        }
        else
        {
            for (size_t i = 0; i < SIMD_BYTES; ++i)
                if (filt_pos[i])
                {
                    *res = *data_pos;
                    res++;
                }
        }

        filt_pos += SIMD_BYTES;
        data_pos += SIMD_BYTES;
    }
    return res - begin_res;
}



uint32_t gen_random() noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0, 1);
    return std::ceil(dis(gen) * UINT32_MAX);
}

int main(int argc, char ** argv) {
    if (argc < 3)
    {
        std::cout << "error" << std::endl;
        return 0;
    }

    int use_sse = atoi(argv[1]);
    size_t size = atoi(argv[2]);

    uint32_t * res = new uint32_t[size];
    uint8_t * filt = new uint8_t[size];
    uint32_t * data = new uint32_t[size];
    for (size_t i = 0; i < size; ++i)
    {
        if (i % 32 < 16)
            filt[i] = 1;
        else
            filt[i] = gen_random() % 2;
        data[i] = gen_random();
    }

    const auto start = std::chrono::high_resolution_clock::now();
    size_t res_size;
    if (use_sse)
        res_size = filterSSE(res, filt, size, data);
    else
        res_size = filter(res, filt, size, data);
    const std::chrono::duration<double> diff =
            std::chrono::high_resolution_clock::now() - start;
    std::cout << "Time: " << std::fixed << std::setprecision(6) << diff.count()
              << " sec " << std::endl;

    std::cout << "res_size: " << res_size << std::endl;

    delete[] res;
    delete[] filt;
    delete[] data;

}
