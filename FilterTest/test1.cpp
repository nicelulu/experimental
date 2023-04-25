#include <memory>
#include <cmath>
#include <cstring>
#include <emmintrin.h>
#include <immintrin.h>
#include <iomanip>
#include <iostream>
#include "Row.h"

/*
 * set result to res by filter array
 */
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

/*
 * set result to res by filter array
 */
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
            memcpy(res, data_pos, 16);
            res += SIMD_BYTES;
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
        data_pos += SIMD_BYTES;
    }

    const uint8_t * filter_end = filter + size;
    while (filt_pos < filter_end)
    {
        if (*filt_pos)
        {
            *res = *data_pos;
            res++;
        }
        filt_pos++;
        data_pos++;
    }
    return res - begin_res;
}

/*
 * set result to res by filter array
 */
template <typename T>
size_t filterAVX(T * res, const uint8_t * filter, size_t size, const T * data)
{
    const T * begin_res = res;
    const uint8_t * filt_pos = filter;
    const T * data_pos = data;

    static constexpr size_t SIMD_BYTES = 32;
    const __m256i zero32 = _mm256_setzero_si256();
    const uint8_t * filt_end_sse = filt_pos + size / SIMD_BYTES * SIMD_BYTES;

    while (filt_pos < filt_end_sse)
    {
        uint32_t mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256(reinterpret_cast<const __m256i *>(filt_pos)), zero32));
        mask = ~mask;
        if (0 == mask)
        {
        }
        else if (0xFFFFFFFF == mask)
        {
            memcpy(res, data_pos, 32);
            res += SIMD_BYTES;
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
        data_pos += SIMD_BYTES;
    }

    const uint8_t * filter_end = filter + size;
    while (filt_pos < filter_end)
    {
        if (*filt_pos)
        {
            *res = *data_pos;
            res++;
        }
        filt_pos++;
        data_pos++;
    }
    return res - begin_res;
}

void initTarget()
{
    target1.row.a = 1000;
    target1.row.b = 9;

    target1_end.row.a = 1000;
    target1_end.row.b = 49;

    target2.row.a = 2000;
    target2.row.b = 9;

    target2_end.row.a = 2000;
    target2_end.row.b = 49;

    target3.row.a = 3000;
    target3.row.b = 9;

    target3_end.row.a = 3000;
    target3_end.row.b = 49;
}

/*
 *  Generate filter array by condition
 */
void genFilter(uint8_t * filter, const RowWapper * row_wappers, int row_size)
{
    size_t size = row_size;
    size_t i = 0;
    while (i < size)
    {
        if ((row_wappers[i].c > target1.c && row_wappers[i].c <= target1_end.c) || (row_wappers[i].c > target2.c && row_wappers[i].c <= target2_end.c) || (row_wappers[i].c > target3.c && row_wappers[i].c <= target3_end.c))
            filter[i] = 1;
        else
            filter[i] = 0;
        ++i;
    }
}

void genFilterSSE(uint8_t * filter, const RowWapper * row_wappers, int row_size)
{
    uint8_t * filter_pos = filter;
    size_t size = row_size;
    size_t i = 0;

    const __m128i ones = _mm_set1_epi64x(-1);
    const __m128i range1_l = _mm_set1_epi64x(target1.c);
    const __m128i range1_r = _mm_set1_epi64x(target1_end.c);
    const __m128i range2_l = _mm_set1_epi64x(target2.c);
    const __m128i range2_r = _mm_set1_epi64x(target2_end.c);
    const __m128i range3_l = _mm_set1_epi64x(target3.c);
    const __m128i range3_r = _mm_set1_epi64x(target3_end.c);

    for (; i + 1 < size; i += 2)
    {
        const __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i *>(row_wappers + i));

        const __m128i range1_l_res = _mm_cmpgt_epi64(data, range1_l);

        const __m128i range1_r_res = _mm_xor_si128(_mm_cmpgt_epi64(data, range1_r), ones);

        const __m128i range1_res = _mm_and_si128(range1_l_res, range1_r_res);

        const __m128i range2_l_res = _mm_cmpgt_epi64(data, range2_l);

        const __m128i range2_r_res = _mm_xor_si128(_mm_cmpgt_epi64(data, range2_r), ones);

        const __m128i range2_res = _mm_and_si128(range2_l_res, range2_r_res);

        const __m128i range3_l_res = _mm_cmpgt_epi64(data, range3_l);

        const __m128i range3_r_res = _mm_xor_si128(_mm_cmpgt_epi64(data, range3_r), ones);

        const __m128i range3_res = _mm_and_si128(range3_l_res, range3_r_res);

        const __m128i range_res = _mm_or_si128(range1_res, _mm_or_si128(range2_res, range3_res));

        uint16_t range_res_mask = _mm_movemask_epi8(range_res);

        if (0 == range_res_mask)
        {
            *reinterpret_cast<uint16_t *>(filter_pos) = 0;
        }
        else if (0xFFFF == range_res_mask)
        {
            union value
            {
                uint8_t a[2] = {1,1};
                uint16_t b;
            };
            *reinterpret_cast<uint16_t *>(filter_pos) = value().b;
        }
        else
        {
            for (size_t j = 0; j < 2; ++j)
            {
                auto * mask = reinterpret_cast<uint8_t *>(&range_res_mask);
                if (0xFF == mask[j])
                {
                    *(filter_pos + j) = 1;
                }
                else
                {
                    *(filter_pos + j) = 0;
                }
            }
        }

        filter_pos += 2;
    }

    for (; i < size; i++)
    {
        if ((row_wappers[i].c > target1.c && row_wappers[i].c <= target1_end.c) || (row_wappers[i].c > target2.c && row_wappers[i].c <= target2_end.c) || (row_wappers[i].c > target3.c && row_wappers[i].c <= target3_end.c))
            *filter_pos = 1;
        else
            *filter_pos = 0;

        filter_pos++;
    }
}

size_t testDirectly(RowWapper * res, const RowWapper * row_wappers, int row_size)
{
    RowWapper * res_pos = res;
    size_t size = row_size;
    size_t i = 0;
    while (i < size)
    {
        if ((row_wappers[i].c > target1.c && row_wappers[i].c <= target1_end.c) || (row_wappers[i].c > target2.c && row_wappers[i].c <= target2_end.c) || (row_wappers[i].c > target3.c && row_wappers[i].c <= target3_end.c))
        {
            *(res_pos) = row_wappers[i];
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

/*
 *  set result to res by condition
 */
size_t testAVX(RowWapper * res, const RowWapper * row_wappers, int row_size)
{
    RowWapper * res_pos = res;
    size_t size = row_size;
    size_t i = 0;

    const __m256i ones = _mm256_set1_epi64x(-1);
    const __m256i range1_l = _mm256_set1_epi64x(target1.c);
    const __m256i range1_r = _mm256_set1_epi64x(target1_end.c);
    const __m256i range2_l = _mm256_set1_epi64x(target2.c);
    const __m256i range2_r = _mm256_set1_epi64x(target2_end.c);
    const __m256i range3_l = _mm256_set1_epi64x(target3.c);
    const __m256i range3_r = _mm256_set1_epi64x(target3_end.c);

    for (; i + 3 < size; i += 4)
    {
        const __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(row_wappers + i));

        const __m256i range1_l_res = _mm256_cmpgt_epi64(data, range1_l);

        const __m256i range1_r_res = _mm256_xor_si256(_mm256_cmpgt_epi64(data, range1_r), ones);

        const __m256i range1_res = _mm256_and_si256(range1_l_res, range1_r_res);

        const __m256i range2_l_res = _mm256_cmpgt_epi64(data, range2_l);

        const __m256i range2_r_res = _mm256_xor_si256(_mm256_cmpgt_epi64(data, range2_r), ones);

        const __m256i range2_res = _mm256_and_si256(range2_l_res, range2_r_res);

        const __m256i range3_l_res = _mm256_cmpgt_epi64(data, range3_l);

        const __m256i range3_r_res = _mm256_xor_si256(_mm256_cmpgt_epi64(data, range3_r), ones);

        const __m256i range3_res = _mm256_and_si256(range3_l_res, range3_r_res);

        const __m256i range_res = _mm256_or_si256(range1_res, _mm256_or_si256(range2_res, range3_res));

        uint32_t range_res_mask = _mm256_movemask_epi8(range_res);

        if (0 == range_res_mask)
        {

        }
        else if (0xFFFFFFFF == range_res_mask)
        {
            memcpy(res_pos, row_wappers + i, 4 * sizeof(RowWapper));
            res_pos += 4;
        }
        else
        {
            for (size_t j = 0; j < 4; ++j)
            {
                auto * filter = reinterpret_cast<uint8_t *>(&range_res_mask);
                if (filter[j])
                {
                    *(res_pos) = *(row_wappers + i + j);
                    res_pos++;
                }
            }
        }
    }

    for (; i < size; i++)
    {
        if ((row_wappers[i].c > target1.c && row_wappers[i].c <= target1_end.c) || (row_wappers[i].c > target2.c && row_wappers[i].c <= target2_end.c) || (row_wappers[i].c > target3.c && row_wappers[i].c <= target3_end.c))
        {
            *(res_pos) = row_wappers[i];
            res_pos++;
        }
    }

    return res_pos - res;
}

/*
 *  set result to res by condition
 */
size_t testSSE(RowWapper * res, const RowWapper * row_wappers, int row_size)
{
    RowWapper * res_pos = res;
    size_t size = row_size;
    size_t i = 0;

    const __m128i ones = _mm_set1_epi64x(-1);
    const __m128i range1_l = _mm_set1_epi64x(target1.c);
    const __m128i range1_r = _mm_set1_epi64x(target1_end.c);
    const __m128i range2_l = _mm_set1_epi64x(target2.c);
    const __m128i range2_r = _mm_set1_epi64x(target2_end.c);
    const __m128i range3_l = _mm_set1_epi64x(target3.c);
    const __m128i range3_r = _mm_set1_epi64x(target3_end.c);

    for (; i + 1 < size; i += 2)
    {
        const __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i *>(row_wappers + i));

        const __m128i range1_l_res = _mm_cmpgt_epi64(data, range1_l);

        const __m128i range1_r_res = _mm_xor_si128(_mm_cmpgt_epi64(data, range1_r), ones);

        const __m128i range1_res = _mm_and_si128(range1_l_res, range1_r_res);

        const __m128i range2_l_res = _mm_cmpgt_epi64(data, range2_l);

        const __m128i range2_r_res = _mm_xor_si128(_mm_cmpgt_epi64(data, range2_r), ones);

        const __m128i range2_res = _mm_and_si128(range2_l_res, range2_r_res);

        const __m128i range3_l_res = _mm_cmpgt_epi64(data, range3_l);

        const __m128i range3_r_res = _mm_xor_si128(_mm_cmpgt_epi64(data, range3_r), ones);

        const __m128i range3_res = _mm_and_si128(range3_l_res, range3_r_res);

        const __m128i range_res = _mm_or_si128(range1_res, _mm_or_si128(range2_res, range3_res));

        uint16_t range_res_mask = _mm_movemask_epi8(range_res);

        if (0 == range_res_mask)
        {

        }
        else if (0xFFFF == range_res_mask)
        {
            memcpy(res_pos, row_wappers + i, 2 * sizeof(RowWapper));
            res_pos += 2;
        }
        else
        {
            for (size_t j = 0; j < 2; ++j)
            {
                auto * filter = reinterpret_cast<uint8_t *>(&range_res_mask);
                if (filter[j])
                {
                    *(res_pos) = *(row_wappers + i + j);
                    res_pos++;
                }
            }
        }
    }

    for (; i < size; i++)
    {
        if ((row_wappers[i].c > target1.c && row_wappers[i].c <= target1_end.c) || (row_wappers[i].c > target2.c && row_wappers[i].c <= target2_end.c) || (row_wappers[i].c > target3.c && row_wappers[i].c <= target3_end.c))
        {
            *(res_pos) = row_wappers[i];
            res_pos++;
        }
    }

    return res_pos - res;
}

/*
 * test result:
 *
 * root$ ./test1 10000000 1 32
 * testDirectly Time: 0.015622 sec res_size: 10000000
 * testAVX Time: 0.014737 sec res_size: 10000000
 * testSSE Time: 0.016422 sec res_size: 10000000
 * filterAVX Time: 0.017115 sec res_size: 10000000
 * filterSSE Time: 0.021388 sec res_size: 10000000
 * filterMethod Time: 0.023430 sec res_size: 10000000
 *
 * root$ ./test1 10000000 2 32
 * testDirectly Time: 0.118058 sec res_size: 5000009
 * testAVX Time: 0.062876 sec res_size: 5000009
 * testSSE Time: 0.064128 sec res_size: 5000009
 * filterAVX Time: 0.051724 sec res_size: 5000009
 * filterSSE Time: 0.051664 sec res_size: 5000009
 * filterMethod Time: 0.055166 sec res_size: 5000009
 *
 * root$ ./test1 10000000 4 32
 * testDirectly Time: 0.053950 sec res_size: 2500372
 * testAVX Time: 0.052327 sec res_size: 2500372
 * testSSE Time: 0.051610 sec res_size: 2500372
 * filterAVX Time: 0.037639 sec res_size: 2500372
 * filterSSE Time: 0.037248 sec res_size: 2500372
 * filterMethod Time: 0.041703 sec res_size: 2500372
 *
 * root$ ./test1 10000000 8 32
 * testDirectly Time: 0.035610 sec res_size: 1249591
 * testAVX Time: 0.038011 sec res_size: 1249591
 * testSSE Time: 0.032496 sec res_size: 1249591
 * filterAVX Time: 0.026949 sec res_size: 1249591
 * filterSSE Time: 0.027121 sec res_size: 1249591
 * filterMethod Time: 0.030936 sec res_size: 1249591
 *
 * root$ ./test1 10000000 10 32
 * testDirectly Time: 0.028327 sec res_size: 999717
 * testAVX Time: 0.032776 sec res_size: 999717
 * testSSE Time: 0.028572 sec res_size: 999717
 * filterAVX Time: 0.025032 sec res_size: 999717
 * filterSSE Time: 0.025067 sec res_size: 999717
 * filterMethod Time: 0.028059 sec res_size: 999717
 *
 * root$ ./test1 10000000 15 32
 * testDirectly Time: 0.096796 sec res_size: 665687
 * testAVX Time: 0.025170 sec res_size: 665687
 * testSSE Time: 0.023091 sec res_size: 665687
 * filterAVX Time: 0.023990 sec res_size: 665687
 * filterSSE Time: 0.023742 sec res_size: 665687
 * filterMethod Time: 0.026712 sec res_size: 665687
 *
 * root$ ./test1 10000000 18 32
 * testDirectly Time: 0.058699 sec res_size: 555783
 * testAVX Time: 0.022826 sec res_size: 555783
 * testSSE Time: 0.022162 sec res_size: 555783
 * filterAVX Time: 0.023304 sec res_size: 555783
 * filterSSE Time: 0.022978 sec res_size: 555783
 * filterMethod Time: 0.026885 sec res_size: 555783
 *
 * root$ ./test1 10000000 20 32
 * testDirectly Time: 0.027541 sec res_size: 499292
 * testAVX Time: 0.021488 sec res_size: 499292
 * testSSE Time: 0.020572 sec res_size: 499292
 * filterAVX Time: 0.022963 sec res_size: 499292
 * filterSSE Time: 0.022361 sec res_size: 499292
 * filterMethod Time: 0.027231 sec res_size: 499292
 */

int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        std::cout << "Param size error. First param is data size, second param is target data ratio, third parame is memory align size." << std::endl;
        return 0;
    }
    initTarget();

    size_t size = atoi(argv[1]);
    size_t target_data_ratio = atoi(argv[2]);
    size_t align = atoi(argv[3]);

    RowWapper * aligned_data = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));

    /// gen data
    std::vector<RowWapper> data(aligned_data, aligned_data + size);
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
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        const auto start = std::chrono::high_resolution_clock::now();
        size_t res_size = testDirectly(&res[0], &data[0], size);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "testDirectly Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_res);
    }

    /// testAVX
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        const auto start = std::chrono::high_resolution_clock::now();
        size_t res_size = testAVX(&res[0], &data[0], size);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "testAVX Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_res);
    }

    /// testSSE
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        const auto start = std::chrono::high_resolution_clock::now();
        size_t res_size = testSSE(&res[0], &data[0], size);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "testSSE Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_res);
    }

    /// filterAVX
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        uint8_t * aligned_filter = static_cast<uint8_t *>(std::aligned_alloc(align, size * sizeof(uint8_t)));
        std::vector<uint8_t> filter(aligned_filter, aligned_filter + size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilter(&filter[0], &data[0], size);
        size_t res_size = filterAVX(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "filterAVX Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_filter);
        std::free(aligned_res);
    }

    /// filterSSE
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        uint8_t * aligned_filter = static_cast<uint8_t *>(std::aligned_alloc(align, size * sizeof(uint8_t)));
        std::vector<uint8_t> filter(aligned_filter, aligned_filter + size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilter(&filter[0], &data[0], size);
        size_t res_size = filterSSE(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "filterSSE Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_filter);
        std::free(aligned_res);
    }

    /// filterMethod
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);
        std::vector<uint8_t> filter(size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilter(&filter[0], &data[0], size);
        size_t res_size = filterMethod(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "filterMethod Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_res);
    }


    /// genFilterSSE filterAVX
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        uint8_t * aligned_filter = static_cast<uint8_t *>(std::aligned_alloc(align, size * sizeof(uint8_t)));
        std::vector<uint8_t> filter(aligned_filter, aligned_filter + size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilterSSE(&filter[0], &data[0], size);
        size_t res_size = filterAVX(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "genFilterSSE filterAVX Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_filter);
        std::free(aligned_res);
    }

    /// genFilterSSE filterSSE
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        uint8_t * aligned_filter = static_cast<uint8_t *>(std::aligned_alloc(align, size * sizeof(uint8_t)));
        std::vector<uint8_t> filter(aligned_filter, aligned_filter + size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilterSSE(&filter[0], &data[0], size);
        size_t res_size = filterSSE(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "genFilterSSE filterSSE Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_filter);
        std::free(aligned_res);
    }

    /// genFilterSSE filterMethod
    {
        RowWapper * aligned_res = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));
        std::vector<RowWapper> res(aligned_res, aligned_res + size);

        uint8_t * aligned_filter = static_cast<uint8_t *>(std::aligned_alloc(align, size * sizeof(uint8_t)));
        std::vector<uint8_t> filter(aligned_filter, aligned_filter + size);

        const auto start = std::chrono::high_resolution_clock::now();
        genFilterSSE(&filter[0], &data[0], size);
        size_t res_size = filterMethod(&res[0], &filter[0], size, &data[0]);

        const std::chrono::duration<double> diff =
                std::chrono::high_resolution_clock::now() - start;
        std::cout << "genFilterSSE filterMethod Time: " << std::fixed << std::setprecision(6) << diff.count()
                  << " sec " << "res_size: " << res_size << std::endl;

        std::free(aligned_filter);
        std::free(aligned_res);
    }

    std::free(aligned_data);
}
