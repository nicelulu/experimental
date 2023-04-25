#include <iostream>
#include <set>
#include "Row.h"


/*
 * Find ranges >= l_target and <= r_target in the target array
 */
std::pair<int64_t, int64_t> find(uint64_t l_target, uint64_t r_target, const RowWapper * rows, int64_t begin, int64_t end)
{
    std::pair<int64_t, int64_t> res{-1, -1};
    int64_t src_end = end;
    while (begin <= end)
    {
        int64_t mid = (end - begin) / 2 + begin; /// Tighten the right side, and when there are only 2 numbers, the mid should be left.
        if (rows[mid].c == l_target)
        {
            end = mid;

            if (end == begin)
                break;
        }
        else if (rows[mid].c > l_target)
        {
            end = mid - 1;
        }
        else if (rows[mid].c < l_target)
        {
            begin = mid + 1;
        }
    }

    res.first = begin;
    end = src_end;
    int64_t last_min = begin;
    while (begin <= end)
    {
        int64_t mid = end - (end - begin) / 2; /// Tighten the left side, when there are only two numbers, the mid should be on the right.
        if (rows[mid].c == r_target)
        {
            begin = mid;

            if (end == begin)
                break;
        }
        else if (rows[mid].c > r_target)
        {
            end = mid - 1;
        }
        else if (rows[mid].c < r_target)
        {
            last_min = mid;
            begin = mid + 1;
        }
    }
    res.second = rows[begin].c == r_target ? begin : last_min;

    return res;
}

int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        std::cout << "Param size error. First param is data size, second param is target data ratio, third parame is memory align size." << std::endl;
        return 0;
    }

    size_t size = atoi(argv[1]);
    [[maybe_unused]]size_t target_data_ratio = atoi(argv[2]);
    size_t align = atoi(argv[3]);

    RowWapper * aligned_data = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(RowWapper)));

    /// gen data
    std::vector<RowWapper> data(aligned_data, aligned_data + size);

    for (size_t i = 0; i < size; ++i)
    {
        if (gen_random1(target_data_ratio) == 1)
        {
            data[i].row.a = 1000;
            data[i].row.b = 47;
        }
        else
        {
            data[i].c = gen_random();
        }
    }

    std::multiset<RowWapper> row_set;

    for (size_t i = 0; i < size; ++i)
    {
        row_set.insert(data[i]);
    }

    for (auto row : row_set)
    {
        std::cout << row.row.a << ", " << row.row.b << std::endl;
    }

    std::free(aligned_data);
}
