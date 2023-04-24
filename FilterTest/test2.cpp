#include <iostream>
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

void initTarget()
{
    target1.row.a = 1000;
    target1.row.b = 10;

    target1_end.row.a = 1000;
    target1_end.row.b = 49;

    target2.row.a = 2000;
    target2.row.b = 10;

    target2_end.row.a = 2000;
    target2_end.row.b = 49;

    target3.row.a = 3000;
    target3.row.b = 10;

    target3_end.row.a = 3000;
    target3_end.row.b = 49;
}

int main(int, char ** argv)
{

    size_t size = atoi(argv[1]);
    [[maybe_unused]]size_t target_data_ratio = atoi(argv[2]);
    size_t align = atoi(argv[3]);

    initTarget();

    RowWapper * aligned_data = static_cast<RowWapper *>(std::aligned_alloc(align, size * sizeof(Row)));

    /// gen data
    std::vector<RowWapper> data(aligned_data, aligned_data + size);
//    for (size_t i = 0; i < size; ++i)
//    {
//        if (gen_random1(target_data_ratio) == 1)
//        {
//            data[i].a = 1000;
//            data[i].b = 47;
//        }
//        else
//        {
//            *(reinterpret_cast<uint64_t *>(&data[i].a)) = gen_random();
//        }
//    }
    data[0].row.a = 1000;
    data[0].row.b = 10;

    for (size_t i = 1; i < size - 1; ++i)
    {
        data[i].row.a = 1000;
        data[i].row.b = 10 + i;
    }
    data[size - 1].row.a = 1001;
    data[size - 1].row.b = 47;

    auto [left1, right1] = find(target1.c, target1_end.c, &data[0], 0, size - 1);
    std::cout << left1 << ", " << right1 << std::endl;
    auto [left2, right2] = find(target2.c, target2_end.c, &data[0], 0, size - 1);
    std::cout << left2 << ", " << right2 << std::endl;
    auto [left3, right3] = find(target3.c, target3_end.c, &data[0], 0, size - 1);
    std::cout << left3 << ", " << right3 << std::endl;
}
