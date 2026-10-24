// Behaviour test for 0x007688B0 UninitializedFillN_IVCountedPointer_007688b0: three-argument uninitialized_fill_n of
// intrusive counted pointers (each copy adds a reference).
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRefCounted { long RefCount; SRefCounted() : RefCount(1) { } };
struct SIVCountedPointer {
    SRefCounted* P;
    SIVCountedPointer() : P(0) { }
    SIVCountedPointer(const SIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};

static SIVCountedPointer* __uninitialized_fill_n(SIVCountedPointer* first, unsigned int n, const SIVCountedPointer& x, const __false_type&)
{
    SIVCountedPointer* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SIVCountedPointer(x);
    return cur;
}
static SIVCountedPointer* UninitializedFillN(SIVCountedPointer* first, unsigned int n, const SIVCountedPointer& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

int main()
{
    SRefCounted object;
    SIVCountedPointer x;
    x.P = &object;
    SIVCountedPointer dst[6];
    SIVCountedPointer* r = UninitializedFillN(dst, 6, x);
    bool ok = r == dst + 6 && object.RefCount == 7;
    for (int k = 0; k < 6; ++k)
        if (dst[k].P != &object) ok = false;
    SIVCountedPointer null;
    if (UninitializedFillN(dst, 3, null) != dst + 3 || dst[2].P != 0 || dst[3].P != &object || object.RefCount != 7) ok = false;
    if (UninitializedFillN(dst, 0, x) != dst || object.RefCount != 7) ok = false;
    if (!ok) { std::printf("007688b0_TEST FAIL\n"); return 1; }
    std::printf("007688b0_TEST PASS\n");
    return 0;
}