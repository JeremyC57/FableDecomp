// Behaviour test for 0x005E0CA0 UninitFillN_LongIVCountedPointer_005e0ca0: fill n {long, intrusive pointer} pairs.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRefCounted { long RefCount; SRefCounted() : RefCount(1) { } };
struct SIVCountedPointer {
    SRefCounted* P;
    SIVCountedPointer() : P(0) { }
    SIVCountedPointer(const SIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
struct SPair {
    long              first;
    SIVCountedPointer second;
    SPair() : first(0) { }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};

static SPair* UninitFillN(SPair* first, unsigned int n, const SPair& x, const __false_type&)
{
    SPair* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SPair(x);
    return cur;
}

int main()
{
    SRefCounted object;
    SPair x;
    x.first = 31; x.second.P = &object;
    SPair dst[3];
    __false_type tag;
    SPair* r = UninitFillN(dst, 3, x, tag);
    bool ok = r == dst + 3 && object.RefCount == 4;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first != 31 || dst[k].second.P != &object) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || object.RefCount != 4) ok = false;
    if (!ok) { std::printf("005e0ca0_TEST FAIL\n"); return 1; }
    std::printf("005e0ca0_TEST PASS\n");
    return 0;
}