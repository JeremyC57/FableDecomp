#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

static int g_copies = 0;
struct SRecord12 {
    int m_a, m_b, m_c;                                  // 32-bit longs in retail
    SRecord12(int a, int b, int c) : m_a(a), m_b(b), m_c(c) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { ++g_copies; }
};
struct SKeyRecord32 {                                   // 0x20 bytes
    int       m_0, m_4, m_8;
    SRecord12 m_record;
    int       m_18, m_1c;
    SKeyRecord32(int k) : m_0(k), m_4(k + 1), m_8(k + 2), m_record(k * 10, k * 20, k * 30), m_18(-k), m_1c(k * k) { }
    SKeyRecord32(const SKeyRecord32& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18), m_1c(o.m_1c) { }
};
static bool RecordIs(const SKeyRecord32& r, int k)
{
    return r.m_0 == k && r.m_4 == k + 1 && r.m_8 == k + 2 && r.m_record.m_a == k * 10 && r.m_record.m_b == k * 20 &&
           r.m_record.m_c == k * 30 && r.m_18 == -k && r.m_1c == k * k;
}

// the row: std::__uninitialized_fill_n<SKeyRecord32*, unsigned int, SKeyRecord32>(..., __false_type)
static SKeyRecord32* __fastcall UninitializedFillN(SKeyRecord32* first, unsigned int n, const SKeyRecord32& x)
{
    SKeyRecord32* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SKeyRecord32(x);
    return cur;
}
int main()
{
    Check(sizeof(SKeyRecord32) == (unsigned int)0x20);
    SKeyRecord32* buf = (SKeyRecord32*)malloc(6 * sizeof(SKeyRecord32));
    memset(buf, 0, 6 * sizeof(SKeyRecord32));
    SKeyRecord32 v(4);
    g_copies = 0;
    SKeyRecord32* end = UninitializedFillN(buf + 1, 4, v);
    Check(end == buf + 5);
    Check(g_copies == 4);
    for (int k = 1; k < 5; ++k)
        Check(RecordIs(buf[k], 4));
    Check(buf[0].m_0 == 0 && buf[5].m_1c == 0);
    Check(UninitializedFillN(buf, 0, v) == buf && g_copies == 4);
    free(buf);

    if (g_fail) {
        std::printf("00a94950_TEST FAIL\n");
        return 1;
    }
    std::printf("00a94950_TEST PASS\n");
    return 0;
}