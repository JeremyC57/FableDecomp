// behaviour test for 0x00BBD580 _STL::uninitialized_fill_n over an 8-byte {bool, long} pair
#include <cstdio>
#include <new>

struct SPairByteLong {
    bool first;
    long second;
    SPairByteLong() : first(false), second(0) { }
    SPairByteLong(const SPairByteLong& o) : first(o.first), second(o.second) { }
};

static SPairByteLong* UninitFillN(SPairByteLong* first, unsigned int n, const SPairByteLong& x)
{
    SPairByteLong* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SPairByteLong(x);
    return cur;
}

int main()
{
    SPairByteLong buf[5];
    buf[3].second = 7;
    SPairByteLong v; v.first = true; v.second = 1234;
    SPairByteLong* e = UninitFillN(buf, 3, v);
    if (e != buf + 3) { std::printf("00bbd580_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (!buf[i].first || buf[i].second != 1234) { std::printf("00bbd580_TEST FAIL\n"); return 1; }
    if (buf[3].second != 7 || buf[3].first) { std::printf("00bbd580_TEST FAIL\n"); return 1; }
    if (UninitFillN(buf, 0, v) != buf) { std::printf("00bbd580_TEST FAIL\n"); return 1; }
    std::printf("00bbd580_TEST PASS\n");
    return 0;
}