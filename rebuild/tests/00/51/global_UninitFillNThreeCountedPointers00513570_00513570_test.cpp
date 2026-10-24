// Behaviour test for 0x00513570 UninitFillN_ThreeCountedPointers_00513570: fill n {long, 3 x CCountedPointer} records
// (each copy adds one reference to each of the three shared count blocks).
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
struct SThreeCountedPointers {
    long            first;
    SCountedPointer m_a;
    SCountedPointer m_b;
    SCountedPointer m_c;
    SThreeCountedPointers() : first(0) { }
};

static SThreeCountedPointers* UninitFillN(SThreeCountedPointers* first, unsigned int n, const SThreeCountedPointers& x, const __false_type&)
{
    SThreeCountedPointers* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SThreeCountedPointers(x);
    return cur;
}

int main()
{
    static int a, b, c;
    SCountBlock ba, bb;
    SThreeCountedPointers x;
    x.first = 77;
    x.m_a.Data = &a; x.m_a.Info = &ba;
    x.m_b.Data = &b; x.m_b.Info = &bb;
    x.m_c.Data = &c; x.m_c.Info = 0;
    SThreeCountedPointers dst[5];
    __false_type tag;
    SThreeCountedPointers* r = UninitFillN(dst, 5, x, tag);
    bool ok = r == dst + 5 && ba.m_references == 6 && bb.m_references == 6;
    for (int k = 0; k < 5; ++k)
        if (dst[k].first != 77 || dst[k].m_a.Data != &a || dst[k].m_a.Info != &ba || dst[k].m_b.Data != &b
            || dst[k].m_b.Info != &bb || dst[k].m_c.Data != &c || dst[k].m_c.Info != 0) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || ba.m_references != 6) ok = false;
    if (!ok) { std::printf("00513570_TEST FAIL\n"); return 1; }
    std::printf("00513570_TEST PASS\n");
    return 0;
}