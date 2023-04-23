#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <thread>
#include "CountDownLatch.h"
#include <sys/event.h>
#include <stdlib.h>

//enum log_val_type {
//    app_log         = 1,
//    conf            = 2,
//    cluster_server  = 3,
//    log_pack        = 4,
//    snp_sync_req    = 5,
//    custom          = 999,
//};


//std::atomic<int> a(0);
//CountDownLatch c(20);
//
//void thread1()
//{
//    c.countDown();
//    c.await();
//    int x = a.fetch_add(1, std::memory_order_relaxed);
//    std::cout << std::to_string(x) + " " << std::endl;
//}

//void write(uint64_t a)
//size_t bytes_copied = 0;
//
///// Produces endless loop
//assert(!working_buffer.empty());
//
//while (bytes_copied < n)
//{
//nextIfAtEnd();
//size_t bytes_to_copy = std::min(static_cast<size_t>(working_buffer.end() - pos), n - bytes_copied);
//memcpy(pos, from + bytes_copied, bytes_to_copy);
//pos += bytes_to_copy;
//bytes_copied += bytes_to_copy;
//}

//int main(void)
//{
//    int snap_fd = ::open("test345.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
//    uint64_t aa = 5;
//    uint64_t aa1 = 8;
//    write(snap_fd, &aa, sizeof(uint64_t));
//    write(snap_fd, &aa1, sizeof(uint64_t));
//    return 0;

//    char time_str[128];
//    unsigned long log_last_index;
//    unsigned long last_log_term;
//    unsigned long object_id;
//
//    sscanf("snapshot_202110101010_111_111", "snapshot_%[^_]", time_str);
//
//    std::cout << time_str << " " << log_last_index << " " << last_log_term << " " << object_id << std::endl;

//
//    std::map<char, int> letter_counts {{'a', 27}, {'b', 3}, {'c', 1}};
//    std::cout << letter_counts['e'] << std::endl;
//    std::cout << letter_counts['x'] << std::endl;
//    std::cout << letter_counts['p'] << std::endl;
//    const char *s1 = "0123456789";
//    const char *s2 = "abcde";
//    int snap_fd = ::open("test333.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
//    uint8_t snap_version = 1;
//    ssize_t ret1 = pwrite(snap_fd, &snap_version, 1, 0);
//    if (ret1 != 1)
//        std::cout << "!!" << std::endl;
//    write(snap_fd, s1, strlen(s1));
//    char aa[11]{0};
//    ssize_t ret = pread(snap_fd, aa, 10, 0);
//    std::cout << ret << "----" << aa << std::endl;
//    ::fsync(snap_fd);
//    ::close(snap_fd);
//
//    int snap_version_fd = ::open("test333.txt", O_RDWR | O_CREAT, 0644);
//    lseek(snap_version_fd, 0, SEEK_SET);
//    uint8_t version;
//    ssize_t ret = read(snap_version_fd, &version, 1);
//    if (ret != 1)
//        std::cout << "!" << version << std::endl;
//    ::close(snap_version_fd);
//    std::cout << ": " << uint32_t(version) << std::endl;
//
//
//    snap_fd = ::open("test.txt", O_RDWR | O_CREAT, 0644);
//    write(snap_fd, s2, strlen(s2));
//    char aa1[11]{0};
//    ssize_t ret1 = pread(snap_fd, aa1, 10, 0);
//    std::cout << ret1 << "----" << aa1 << std::endl;

//    std::cout << sizeof(log_val_type) << std::endl;
//
//    std::cout << sizeof(char) << std::endl;
//    std::cout << sizeof(unsigned char) << std::endl;
//
//    int seg_fd = ::open("test.txt", O_RDWR);
//
//    const char *s1 = "0123456789";
//    const char *s2 = "abcde";
//
//    write(seg_fd, s1, strlen(s1));
//
////    int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
////    int fd = open("test.txt", O_RDWR, 0644);
//    /* if error */
//
//
//    ftruncate(seg_fd, 2);
//     lseek(seg_fd, 2, SEEK_SET);
//
//    write(seg_fd, s2, strlen(s2));
//
////    close(fd);
//
//
//    char aa[8];
//    ssize_t ret = pread(seg_fd, aa, 8, 0);
//    std::cout << ret << "----" << aa << std::endl;
//    return 0;
//}

//class A
//{
//public:
//    int * aa = new int(1);
//
////    A()
////    {
////        std::cout << " A()" << std::endl;
////    }
////
////    A(A && a)
////    {
////        std::cout << "move a" << std::endl;
////    }
//
//    A & operator=(const A & a)
//    {
//        std::cout << "ag a" << std::endl;
//        return *this;
//    }
//
//    A & operator= (A && a)
//    {
//        std::cout << "move a" << std::endl;
//        int * old = aa;
//        aa = a.aa;
//        delete old;
//        a.aa = nullptr;
//        return *this;
//    }
//};
//
//class B
//{
//public:
//    B & operator = (const B & a)
//    {
//        std::cout << "ag b" << std::endl;
//        return *this;
//    }
//};
//
//class C
//{
//    A a;
//    B b;
//};

//


void split(const std::string& s,
           std::vector<std::string>& sv,
           const char* delim = " ") {
    sv.clear();                                 // 1.
    char* buffer = new char[s.size() + 1];      // 2.
    buffer[s.size()] = '\0';
    std::copy(s.begin(), s.end(), buffer);      // 3.
    char* p = std::strtok(buffer, delim);       // 4.
    do {
        sv.push_back(p);                        // 5.
    } while ((p = std::strtok(nullptr, delim)));   // 6.
    delete[] buffer;
    return;
}


int main()
{
/// force_sync f
/// session_consistent f
/// 25:(34228,55464),50:(43659,71716),75:(44447,75426),100:(45349,75619),125:(45927,75149),150:(44748,75315),175:(43978,73885),200:(45218,74563)




/// force_sync f
/// session_consistent t
/// 25:(30436,45727),50:(41996,66861),75:(47870,75803),100:(51355,79278),125:(52498,83628),150:(54209,84797),175:(55247,85130),200:(57264,82467)


/// force_sync t
/// session_consistent f
/// 25:(1459,2380),50:(1439,2495),75:(1307,2422),100:(1333,2349),125:(1434,2171),150:(1229,2250),175:(1260,2335),200:(1314,2174)


/// force_sync t
/// session_consistent t
/// 25:(8332,14090),50:(14211,23696),75:(18989,33198),100:(23778,40958),125:(25283,46723),150:(28804,49005),175:(31271,54445),200:(29034,53147)


    std::string s("56538 86769 87383 87857 94194 94656 94246 92731 94370 91402 93156 92707 92232 92119 86394 55574 73526 90175 76799 85063 90498 90663 90165 92430 90269 88681 85669 87785 90124 85786 88979 87448");
    std::vector<std::string> sv;

    split(s, sv, " ");

    size_t size = sv.size();
    size_t sum = 0;
    for (std::vector<std::string>::const_iterator iter = sv.begin(); iter != sv.end(); ++iter)
    {
        sum += std::atoi(iter->c_str());
    }

    size_t avg = sum / size;

    std::cout << avg << std::endl;

//    std::round(std::nan(""));

//    A a = A();
//    A b = std::move(a);
//    C ccc;
//    C cc;
//    cc = std::move(ccc);


    /// 1875406717368 1875853829854
    /// 37946725251485
    /// 37946735250505
    /// 37946735251485
    /// 18446744073709540464
    /// 18446744073709551610
    /// 9223372036854775807

    /// 38028991328449
    /// 38028991996720
    /// 38029001328449
    /// 38029001330974


    /// 38029947491286
    /// 38029957488151
    /// 38029957491286
    /// 38029957491227 2021.11.09 15:24:24.285220
    ///     re         2021.11.09 15:24:24.281364
    ///     se         2021.11.09 15:24:24.287130
//    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() << std::endl;
//
//    int aaa = 0x13;
//    std::string_view s = "asdas";
//    std::string host = "localhost:9181";
//    std::string ip_or_hostname = host.substr(0, host.find_last_of(':'));
//    std::cout << ip_or_hostname << std::endl;
//    std::cout << host << std::endl;
////    int a = 10;
////    std::bitset<32> b0(a);
////
////    std::cout << b0 << std::endl;
//    u_int32_t ab = 1;
//    u_int32_t ba = 4;
//    u_int64_t concatenation = (static_cast<u_int64_t>(ab) << 32) | static_cast<u_int64_t>(ba);
//    std::cout << std::bitset<64>(concatenation) << std::endl;
//
//    int8_t a(-10);
//    uint64_t b(2);
//
//    int32_t c = -1;
//    std::cout << std::bitset<32>(c) << std::endl;
//
//    std::cout << std::bitset<8>(int8_t(a)) << std::endl;
//    std::cout << std::bitset<64>((uint64_t)a) << std::endl;
////    std::bitset<64> d(a);
////
////    std::cout << d << std::endl;
////    auto c = ((uint64_t)a) / b;
////    std::cout << "" << c << std::endl;
//
////    std::cout << uint64_t(99999999) / std::numeric_limits<double>::max() << std::endl;
////    std::cout << std::numeric_limits<double>::min() / std::numeric_limits<double>::max() << std::endl;
////    std::cout << std::numeric_limits<double>::max() / std::numeric_limits<double>::min() << std::endl;
////    std::cout << std::numeric_limits<double>::max() / std::numeric_limits<double>::lowest() << std::endl;
////    std::cout << "----------------" << std::endl;
////    std::cout << std::numeric_limits<double>::max() << std::endl;
////    std::cout << std::numeric_limits<double>::min() << std::endl;
////    std::cout << std::numeric_limits<double>::lowest() << std::endl;
////    std::cout << 1 / 0.0 << std::endl;
////
////    std::cout << "----------------" << std::endl;
////    std::cout << std::bitset<64>((uint64_t(3644248))) << std::endl; // 后16位 1001101101011000
////    std::cout << std::bitset<64>((uint64_t(3578712))) << std::endl; // 后16位 1001101101011000
////    std::cout << std::bitset<64>(uint64_t(3644248) >> 16) << std::endl; // 0000000000000000000000000000000000000000000000000000000000110111
////    std::cout << std::bitset<64>(uint64_t(3578712) >> 16) << std::endl; // 0000000000000000000000000000000000000000000000000000000000110110
//    std::cout << sizeof(ptrdiff_t) << std::endl;
//    uint8_t aa = 72;
//
//    std::cout << std::bitset<16>(uint8_t(aa << 2)) << std::endl;
//    std::cout << std::bitset<8>(uint8_t(aa << 2)) << std::endl;
//    std::cout << std::bitset<8>(uint8_t(uint8_t(aa << 2) >> 0)) << std::endl;
//
//    std::string str = "▇";
////    str.push_back(uint8_t(32));
//    std::cout << str.size() << std::endl;
//    std::cout << str << std::endl;
//    str.resize(1);
//    std::cout << str << std::endl;
}
