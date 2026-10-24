// Behaviour test for 0x00A7E630 UninitCopy_PairU64Long_00a7e630: placement copy of pair<unsigned __int64, long>.
#include <cstdio>
#include <new>

#if defined(_MSC_VER)
typedef unsigned __int64 u64_t;
#else
typedef unsigned long long u64_t;
#endif

struct __false_type { __false_type() { } };
struct SPair {
    u64_t first;
    long  second;
    SPair() : first(0), second(0) { }
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
    SPair src[3];
    for (int i = 0; i < 3; ++i) { src[i].first = ((u64_t)(i + 1) << 32) | (u64_t)(0x100 + i); src[i].second = 7 - i; }
    SPair dst[3];
    __false_type tag;
    SPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && sizeof(SPair) == 16;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first != (((u64_t)(k + 1) << 32) | (u64_t)(0x100 + k)) || dst[k].second != 7 - k) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00a7e630_TEST FAIL\n"); return 1; }
    std::printf("00a7e630_TEST PASS\n");
    return 0;
}