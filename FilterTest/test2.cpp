#include <iostream>
#include <random>
#include <chrono>


typedef struct Row
{
    int32_t a;
    int32_t b;
} Row;
// ((b >= 10 && b < 50) && * (a == 1000 || a == 2000 || a == 3000))

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

// 1000 1000 1000 1001
std::pair<int64_t, int64_t> find(uint64_t l_target, uint64_t r_target, const RowWapper * rows, int64_t begin, int64_t end)
{
    int64_t src_end = end;
    int64_t mid = -1;
    std::pair<int64_t, int64_t> res{-1, -1};
    int64_t pre_min_index = -1;
    int64_t last_max_index = -1;
    while (begin <= end)
    {
        if (l_target == rows[begin].c || (begin == end && l_target < rows[begin].c))
        {
            if (l_target == rows[begin].c)
                res.first = begin;
            
            if (begin == end && l_target < rows[begin].c)
                res.first = pre_min_index;

            // begin find right
            end = src_end;
            begin = res.first;
            while (begin <= end)
            {
                if (r_target == rows[end].c)
                {
                    res.second = end;
                }

                if (res.second != -1)
                    break;

                if (begin == end)
                    break;

                mid = (end - begin) / 2 + begin + 1;

                if (r_target < rows[mid].c)
                {
                    end = mid - 1;
                }
                else if (r_target >= rows[mid].c)
                {
                    last_max_index = mid;
                    begin = mid;
                }
                else
                {
                    break;
                }
            }

            if (res.second == -1)
                res.second = last_max_index;
        }

        if (res.first != -1 && res.second != -1)
            break;

        if (begin == end)
            break;


        mid = (end - begin) / 2 + begin;
        if (l_target <= rows[mid].c)
        {
            pre_min_index = mid;
            end = mid;
        }
        else if (l_target > rows[mid].c)
        {
            begin = mid + 1;
        }
    }

    return res;
}

int main(int, char ** argv)
{

    size_t size = atoi(argv[1]);
    [[maybe_unused]]size_t target_data_ratio = atoi(argv[2]);
    size_t align = atoi(argv[3]);

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
//    data[0].row.a = 999;
//    data[0].row.b = 47;
//    for (size_t i = 1; i < size - 1; ++i)
//    {
//        data[i].row.a = 1000;
//        data[i].row.b = 9 + i;
//    }
//    data[size - 1].row.a = 1001;
//    data[size - 1].row.b = 47;
    data[0].c = 1;
    data[1].c = 3;
    data[2].c = 5;
    data[3].c = 7;

    auto [left0, right0] = find(2, 4, &data[0], 0, size - 1);
    std::cout << left0 << ", " << right0 << std::endl;

    // 99947 100047 100047 100147
    //                          100009      100049
    auto [left1, right1] = find(target1.c, target1_end.c, &data[0], 0, size - 1);
    std::cout << left1 << ", " << right1 << std::endl;
    auto [left2, right2] = find(target2.c, target2_end.c, &data[0], 0, size - 1);
    std::cout << left2 << ", " << right2 << std::endl;
    auto [left3, right3] = find(target3.c, target3_end.c, &data[0], 0, size - 1);
    std::cout << left3 << ", " << right3 << std::endl;
}
