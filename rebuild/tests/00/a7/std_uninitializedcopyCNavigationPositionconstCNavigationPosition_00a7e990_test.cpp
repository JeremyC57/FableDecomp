// Behaviour test for 0x00A7E990 UninitCopy_Record12_00a7e990: memberwise placement copy of 12-byte records.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRecord12 {
    long m_0, m_4, m_8;
    SRecord12() : m_0(0), m_4(0), m_8(0) { }
    SRecord12(const SRecord12& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8) { }
};

static SRecord12* UninitCopy(const SRecord12* first, const SRecord12* last, SRecord12* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord12(*first);
    return result;
}

int main()
{
    SRecord12 src[3];
    for (int i = 0; i < 3; ++i) { src[i].m_0 = i; src[i].m_4 = 10 * i; src[i].m_8 = -i; }
    SRecord12 dst[3];
    __false_type tag;
    SRecord12* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].m_0 != k || dst[k].m_4 != 10 * k || dst[k].m_8 != -k) ok = false;
    if (UninitCopy(src + 2, src + 2, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("00a7e990_TEST FAIL\n"); return 1; }
    std::printf("00a7e990_TEST PASS\n");
    return 0;
}