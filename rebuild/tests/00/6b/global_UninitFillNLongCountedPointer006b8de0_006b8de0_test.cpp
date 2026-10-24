// Behaviour test for 0x006B8DE0 UninitFillN_LongCountedPointer_006b8de0: fill n pair<long, CCountedPointer<T> >.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SCountBlock { long m_references; SCountBlock() : m_references(1) { } };
struct SCountedPointer {
    void*        Data;
    SCountBlock* Info;
    SCountedPointer() : Data(0), Info(0) { }
    SCountedPointer(const SCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
};
struct SPair {
    long            first;
    SCountedPointer second;
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
    static int object;
    SCountBlock block;
    SPair x;
    x.first = -8; x.second.Data = &object; x.second.Info = &block;
    SPair dst[4];
    __false_type tag;
    SPair* r = UninitFillN(dst, 4, x, tag);
    bool ok = r == dst + 4 && block.m_references == 5;
    for (int k = 0; k < 4; ++k)
        if (dst[k].first != -8 || dst[k].second.Data != &object || dst[k].second.Info != &block) ok = false;
    SPair empty;
    if (UninitFillN(dst, 1, empty, tag) != dst + 1 || dst[0].second.Info != 0 || block.m_references != 5) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || block.m_references != 5) ok = false;
    if (!ok) { std::printf("006b8de0_TEST FAIL\n"); return 1; }
    std::printf("006b8de0_TEST PASS\n");
    return 0;
}