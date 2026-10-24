// Behaviour test for 0x005B149F UninitFillN_IVCountedPointer_005b149f: __uninitialized_fill_n of intrusive counted pointers.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRefCounted { long RefCount; SRefCounted() : RefCount(1) { } };
struct SIVCountedPointer {
    SRefCounted* P;
    SIVCountedPointer() : P(0) { }
    SIVCountedPointer(const SIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};

static SIVCountedPointer* UninitFillN(SIVCountedPointer* first, unsigned int n, const SIVCountedPointer& x, const __false_type&)
{
    SIVCountedPointer* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SIVCountedPointer(x);
    return cur;
}

int main()
{
    SRefCounted object;
    SIVCountedPointer x;
    x.P = &object;
    SIVCountedPointer dst[4];
    __false_type tag;
    SIVCountedPointer* r = UninitFillN(dst, 4, x, tag);
    bool ok = r == dst + 4 && object.RefCount == 5;
    for (int k = 0; k < 4; ++k)
        if (dst[k].P != &object) ok = false;
    SIVCountedPointer null;
    if (UninitFillN(dst + 1, 2, null, tag) != dst + 3 || dst[0].P != &object || dst[1].P != 0 || dst[2].P != 0 || object.RefCount != 5) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || object.RefCount != 5) ok = false;
    if (!ok) { std::printf("005b149f_TEST FAIL\n"); return 1; }
    std::printf("005b149f_TEST PASS\n");
    return 0;
}