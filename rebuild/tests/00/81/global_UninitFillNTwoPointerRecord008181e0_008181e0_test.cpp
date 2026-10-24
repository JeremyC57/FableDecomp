// Behaviour test for 0x008181E0 UninitFillN_TwoPointerRecord_008181e0: fill n 28-byte records holding two intrusive
// counted pointers (both gain one reference per copy) plus plain words and a bool.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRefCounted { long m_vtablePlaceholder; long RefCount; SRefCounted() : m_vtablePlaceholder(0), RefCount(1) { } };
struct SIVCountedPointer {
    SRefCounted* P;
    SIVCountedPointer() : P(0) { }
    SIVCountedPointer(const SIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
struct STwoPointerRecord {
    long              m_0;
    SIVCountedPointer m_first;
    SIVCountedPointer m_second;
    long              m_c;
    long              m_10;
    bool              m_14;
    long              m_18;
    STwoPointerRecord() : m_0(0), m_c(0), m_10(0), m_14(false), m_18(0) { }
};

static STwoPointerRecord* UninitFillN(STwoPointerRecord* first, unsigned int n, const STwoPointerRecord& x, const __false_type&)
{
    STwoPointerRecord* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) STwoPointerRecord(x);
    return cur;
}

int main()
{
    SRefCounted p1, p2;
    STwoPointerRecord x;
    x.m_0 = 5; x.m_first.P = &p1; x.m_second.P = &p2; x.m_c = 9; x.m_10 = -1; x.m_14 = true; x.m_18 = 1234;
    STwoPointerRecord dst[3];
    __false_type tag;
    STwoPointerRecord* r = UninitFillN(dst, 3, x, tag);
    bool ok = r == dst + 3 && p1.RefCount == 4 && p2.RefCount == 4;
    for (int k = 0; k < 3; ++k)
        if (dst[k].m_0 != 5 || dst[k].m_first.P != &p1 || dst[k].m_second.P != &p2 || dst[k].m_c != 9
            || dst[k].m_10 != -1 || !dst[k].m_14 || dst[k].m_18 != 1234) ok = false;
    x.m_second.P = 0;
    if (UninitFillN(dst, 1, x, tag) != dst + 1 || dst[0].m_second.P != 0 || p1.RefCount != 5 || p2.RefCount != 4) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || p1.RefCount != 5) ok = false;
    if (!ok) { std::printf("008181e0_TEST FAIL\n"); return 1; }
    std::printf("008181e0_TEST PASS\n");
    return 0;
}