#include <cstdio>
#include <new>

struct SRecord2C {
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long m_10;
    long m_14;
    long m_18;
    long m_1c;
    long m_20;
    long m_24;
    long m_28;
    SRecord2C() { m_00 = 0; m_04 = 0; m_08 = 0; m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0; m_1c = 0; m_20 = 0; m_24 = 0; m_28 = 0; }
    SRecord2C(const SRecord2C& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18), m_1c(o.m_1c), m_20(o.m_20), m_24(o.m_24), m_28(o.m_28) { }
};
struct __false_type { __false_type() { } };

static SRecord2C* UninitCopy(const SRecord2C* first, const SRecord2C* last, SRecord2C* result, const __false_type&)
{
    SRecord2C* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SRecord2C(*first);
    return cur;
}

int main()
{
    SRecord2C src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].m_00 = i * 100 + 1; src[i].m_04 = i * 100 + 2; src[i].m_08 = i * 100 + 3; src[i].m_0c = i * 100 + 4; src[i].m_10 = i * 100 + 5; src[i].m_14 = i * 100 + 6; src[i].m_18 = i * 100 + 7; src[i].m_1c = i * 100 + 8; src[i].m_20 = i * 100 + 9; src[i].m_24 = i * 100 + 10; src[i].m_28 = i * 100 + 11; }
    __false_type tag;
    SRecord2C* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int i = 0; i < 3; ++i) if (dst[i].m_00 != i * 100 + 1 || dst[i].m_04 != i * 100 + 2 || dst[i].m_08 != i * 100 + 3 || dst[i].m_0c != i * 100 + 4 || dst[i].m_10 != i * 100 + 5 || dst[i].m_14 != i * 100 + 6 || dst[i].m_18 != i * 100 + 7 || dst[i].m_1c != i * 100 + 8 || dst[i].m_20 != i * 100 + 9 || dst[i].m_24 != i * 100 + 10 || dst[i].m_28 != i * 100 + 11) ok = false;
    if (dst[3].m_00 != 0 || dst[3].m_28 != 0) ok = false;
    if (UninitCopy(src, src, dst + 2, tag) != dst + 2) ok = false;
    if (!ok) { std::printf("00f37cc0_TEST FAIL\n"); return 1; }
    std::printf("00f37cc0_TEST PASS\n");
    return 0;
}