#include <cstdio>
#include <new>

struct SRecord1C {
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long m_10;
    long m_14;
    long m_18;
    SRecord1C() { m_00 = 0; m_04 = 0; m_08 = 0; m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0; }
    SRecord1C(const SRecord1C& o) : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18) { }
};
struct __false_type { __false_type() { } };

static SRecord1C* UninitCopy(const SRecord1C* first, const SRecord1C* last, SRecord1C* result, const __false_type&)
{
    SRecord1C* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SRecord1C(*first);
    return cur;
}

int main()
{
    SRecord1C src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].m_00 = i * 100 + 1; src[i].m_04 = i * 100 + 2; src[i].m_08 = i * 100 + 3; src[i].m_0c = i * 100 + 4; src[i].m_10 = i * 100 + 5; src[i].m_14 = i * 100 + 6; src[i].m_18 = i * 100 + 7; }
    __false_type tag;
    SRecord1C* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int i = 0; i < 3; ++i) if (dst[i].m_00 != i * 100 + 1 || dst[i].m_04 != i * 100 + 2 || dst[i].m_08 != i * 100 + 3 || dst[i].m_0c != i * 100 + 4 || dst[i].m_10 != i * 100 + 5 || dst[i].m_14 != i * 100 + 6 || dst[i].m_18 != i * 100 + 7) ok = false;
    if (dst[3].m_00 != 0 || dst[3].m_18 != 0) ok = false;
    if (UninitCopy(src, src, dst + 2, tag) != dst + 2) ok = false;
    if (!ok) { std::printf("00b8eed0_TEST FAIL\n"); return 1; }
    std::printf("00b8eed0_TEST PASS\n");
    return 0;
}