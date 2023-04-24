#include <memory>
#include <cmath>
#include <cstring>
#include <emmintrin.h>
#include <immintrin.h>
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
//        std::cout << mask << std::endl;
        if (0 == mask)
        {
        }
        else if (0xFFFF == mask)
        {
            memcpy(res, data_pos, 16);
            res += SIMD_BYTES/sizeof(T);
        }
        else
        {
            for (size_t i = 0; i < SIMD_BYTES; ++i)
            {
                if (filt_pos[i])
                {
                    *res = data_pos[i];
                    res++;
                }
            }
        }

        filt_pos += SIMD_BYTES;
        data_pos += SIMD_BYTES/sizeof(T);
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
    target1.a[1] = 9;

    target1_end.a[0] = 1000;
    target1_end.a[1] = 49;

    target2.a[0] = 2000;
    target2.a[1] = 9;

    target2_end.a[0] = 2000;
    target2_end.a[1] = 49;

    target3.a[0] = 3000;
    target3.a[1] = 9;

    target3_end.a[0] = 3000;
    target3_end.a[1] = 49;
}



void genFilter(uint8_t * filter, const Row *rows, int nrows)
{
    size_t size = nrows;
    size_t i = 0;
    while (i < size)
    {
        if ((rows[i].c > target1.c && rows[i].c <= target1_end.c) || (rows[i].c > target2.c && rows[i].c <= target2_end.c) || (rows[i].c > target3.c && rows[i].c <= target3_end.c))
            filter[i] = 1;
        else
            filter[i] = 0;
        ++i;
    }
}

size_t testDirectly(Row * res, const Row *rows, int nrows)
{
    Row * res_pos = res;
    size_t size = nrows;
    size_t i = 0;
    while (i < size)
    {
        if ((rows[i].c > target1.c && rows[i].c <= target1_end.c) || (rows[i].c > target2.c && rows[i].c <= target2_end.c) || (rows[i].c > target3.c && rows[i].c <= target3_end.c))
        {
            *(res_pos) = rows[i];
            res_pos++;
        }
        ++i;
    }

    return res_pos - res;
}

void print(__m256i var)
{
    auto * val = reinterpret_cast<uint64_t *>(&var);

    std::cout << "print _mm256i: " << std::hex;
    for (size_t i = 0; i < 4; ++i)
    {
        std::cout << "" << val[i] << " ";
    }

    std::cout << std::dec << std::endl;
}

size_t testAVX(Row * res, const Row * rows, int nrows)
{
    Row * res_pos = res;
    size_t size = nrows;
    size_t i = 0;
    for (; i + 3 < size; i += 4)
    {
        const __m256i ones = _mm256_set1_epi64x(-1);
//        print(ones);
        const __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(rows + i));
//        print(data);
        const __m256i range1_l = _mm256_set1_epi64x(target1.c);
//        print(range1_l);
        const __m256i range1_r = _mm256_set1_epi64x(target1_end.c);
//        print(range1_r);
        const __m256i range2_l = _mm256_set1_epi64x(target2.c);
//        print(range2_l);
        const __m256i range2_r = _mm256_set1_epi64x(target2_end.c);
//        print(range2_r);
        const __m256i range3_l = _mm256_set1_epi64x(target3.c);
//        print(range3_l);
        const __m256i range3_r = _mm256_set1_epi64x(target3_end.c);
//        print(range3_r);

        const __m256i range1_l_res = _mm256_cmpgt_epi64(data, range1_l);
//        print(range1_l_res);
        const __m256i range1_r_res = _mm256_xor_si256(_mm256_cmpgt_epi64(data, range1_r), ones);
//        print(range1_r_res);
        const __m256i range1_res = _mm256_and_si256(range1_l_res, range1_r_res);
//        print(range1_res);

        const __m256i range2_l_res = _mm256_cmpgt_epi64(data, range2_l);
//        print(range2_l_res);
        const __m256i range2_r_res = _mm256_xor_si256(_mm256_cmpgt_epi64(data, range2_r), ones);
//        print(range2_r_res);
        const __m256i range2_res = _mm256_and_si256(range2_l_res, range2_r_res);
//        print(range2_res);

        const __m256i range3_l_res = _mm256_cmpgt_epi64(data, range3_l);
//        print(range3_l_res);
        const __m256i range3_r_res = _mm256_xor_si256(_mm256_cmpgt_epi64(data, range3_r), ones);
//        print(range3_r_res);
        const __m256i range3_res = _mm256_and_si256(range3_l_res, range3_r_res);
//        print(range3_res);

        const __m256i range_res = _mm256_or_si256(range1_res, _mm256_or_si256(range2_res, range3_res));
//        print(range_res);

        uint32_t range_res_mask = _mm256_movemask_epi8(range_res);
//        std::cout << "range_res_mask: " << range_res_mask << std::endl;
        if (0 == range_res_mask)
        {

        }
        else if (0xFFFFFFFF == range_res_mask)
        {
            memcpy(res_pos, rows + i, 4 * sizeof(Row));
            res_pos += 4;
        }
        else
        {
            for (size_t j = 0; j < 4; ++j)
            {
                auto * filter = reinterpret_cast<uint8_t *>(&range_res_mask);
                if (filter[j])
                {
                    *(res_pos) = *(rows + i + j);
                    res_pos++;
                }
            }
        }
    }

    for (; i < size; i++)
    {
        if ((rows[i].c > target1.c && rows[i].c <= target1_end.c) || (rows[i].c > target2.c && rows[i].c <= target2_end.c) || (rows[i].c > target3.c && rows[i].c <= target3_end.c))
        {
            *(res_pos) = rows[i];
            res_pos++;
        }
    }

    return res_pos - res;
}

int main(int argc, char ** argv) {
    if (argc < 3)
    {
        std::cout << "param size error. First param is data size, second param is target data ratio." << std::endl;
        return 0;
    }
    initTarget();

    size_t size = atoi(argv[1]);
    size_t target_data_ratio = atoi(argv[2]);

    std::vector<Row> data(size);
    for (size_t i = 0; i < size; ++i)
    {
        if (gen_random1(target_data_ratio) == 1)
        {
            data[i].c = target1_end.c;
        }
        else
        {
            data[i].c = gen_random();
        }
    }

    /// testDirectly
    {
        std::vector<Row> res(size);

        const auto start = std::chrono::high_resolution_clock::now();
        size_t res_size = testDirectly(&res[0], &data[0], size);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "testDirectly Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;
    }

    /// testAVX
    {
        std::vector<Row> res(size);

        const auto start = std::chrono::high_resolution_clock::now();
        size_t res_size = testAVX(&res[0], &data[0], size);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "testAVX Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;
    }

    /// filterSSE
    {
        std::vector<Row> res(size);
        std::vector<uint8_t> filter(size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilter(&filter[0], &data[0], size);
        size_t res_size = filterSSE(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "filterSSE Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;
    }

    /// filterMethod
    {
        std::vector<Row> res(size);
        std::vector<uint8_t> filter(size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilter(&filter[0], &data[0], size);
        size_t res_size = filterMethod(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "filterMethod Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;
    }

}
