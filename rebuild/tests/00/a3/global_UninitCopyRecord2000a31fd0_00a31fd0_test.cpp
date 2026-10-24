// Behaviour test for 0x00A31FD0 UninitCopy_Record20_00a31fd0: memberwise placement copy of 20-byte records with a nested record.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SRecord12 {
    long m_a, m_b, m_c;
    SRecord12() : m_a(0), m_b(0), m_c(0) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};
struct SRecord20 {
    long      m_0;
    SRecord12 m_record;
    long      m_10;
    SRecord20() : m_0(0), m_10(0) { }
    SRecord20(const SRecord20& o) : m_0(o.m_0), m_record(o.m_record), m_10(o.m_10) { }
};

static SRecord20* UninitCopy(const SRecord20* first, const SRecord20* last, SRecord20* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord20(*first);
    return result;
}

int main()
{
    SRecord20 src[3];
    for (int i = 0; i < 3; ++i) { src[i].m_0 = i; src[i].m_record.m_a = 10 + i; src[i].m_record.m_b = 20 + i; src[i].m_record.m_c = 30 + i; src[i].m_10 = 40 + i; }
    SRecord20 dst[3];
    __false_type tag;
    SRecord20* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].m_0 != k || dst[k].m_record.m_a != 10 + k || dst[k].m_record.m_b != 20 + k || dst[k].m_record.m_c != 30 + k || dst[k].m_10 != 40 + k) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00a31fd0_TEST FAIL\n"); return 1; }
    std::printf("00a31fd0_TEST PASS\n");
    return 0;
}