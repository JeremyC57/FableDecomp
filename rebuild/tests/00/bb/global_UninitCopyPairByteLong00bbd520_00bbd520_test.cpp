// Behaviour test for 0x00BBD520 UninitCopy_PairByteLong_00bbd520: placement copy of 8-byte {bool, long} pairs.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SPair {
    bool first;
    long second;
    SPair() : first(false), second(0) { }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};

static SPair* UninitCopy(const SPair* first, const SPair* last, SPair* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SPair(*first);
    return result;
}

int main()
{
    SPair src[4];
    for (int i = 0; i < 4; ++i) { src[i].first = (i & 1) != 0; src[i].second = 1000 * i - 1; }
    SPair dst[4];
    __false_type tag;
    SPair* r = UninitCopy(src, src + 4, dst, tag);
    bool ok = r == dst + 4;
    for (int k = 0; k < 4; ++k)
        if (dst[k].first != ((k & 1) != 0) || dst[k].second != 1000 * k - 1) ok = false;
    if (UninitCopy(src + 3, src + 3, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00bbd520_TEST FAIL\n"); return 1; }
    std::printf("00bbd520_TEST PASS\n");
    return 0;
}