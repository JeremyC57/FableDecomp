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

struct SRecordVector { SKeyRecord32* start; SKeyRecord32* finish; SKeyRecord32* eos; };
// the row: vector<SKeyRecord32>::_M_insert_overflow(pos, x, __false_type, fill_len, atend)
static void InsertOverflow(SRecordVector& v, SKeyRecord32* pos, const SKeyRecord32& x, unsigned int fillLen, bool atEnd)
{
    unsigned int oldSize = (unsigned int)(v.finish - v.start);
    unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    SKeyRecord32* newStart = len ? (SKeyRecord32*)malloc(len * sizeof(SKeyRecord32)) : 0;
    SKeyRecord32* newFinish = newStart;
    for (SKeyRecord32* p = v.start; p != pos; ++p, ++newFinish)
        new (newFinish) SKeyRecord32(*p);
    if (fillLen == 1) {
        new (newFinish) SKeyRecord32(x);
        ++newFinish;
    } else
        for (unsigned int n = fillLen; n > 0; --n, ++newFinish)
            new (newFinish) SKeyRecord32(x);
    if (!atEnd)
        for (SKeyRecord32* q = pos; q != v.finish; ++q, ++newFinish)
            new (newFinish) SKeyRecord32(*q);
    if (v.start)
        free(v.start);
    v.start = newStart;
    v.finish = newFinish;
    v.eos = newStart + len;
}
int main()
{
    SRecordVector v = { 0, 0, 0 };
    InsertOverflow(v, v.finish, SKeyRecord32(1), 1, true);
    Check(v.finish - v.start == 1 && v.eos - v.start == 1);
    InsertOverflow(v, v.finish, SKeyRecord32(2), 1, true);
    InsertOverflow(v, v.finish, SKeyRecord32(3), 1, true);
    Check(v.finish - v.start == 3 && v.eos - v.start == 4);
    InsertOverflow(v, v.start + 1, SKeyRecord32(7), 5, false);   // [1, 7 x5, 2, 3]: capacity 3 + 5
    Check(v.finish - v.start == 8 && v.eos - v.start == 8);
    Check(RecordIs(v.start[0], 1) && RecordIs(v.start[6], 2) && RecordIs(v.start[7], 3));
    for (int k = 1; k < 6; ++k)
        Check(RecordIs(v.start[k], 7));
    free(v.start);

    if (g_fail) {
        std::printf("00a96b30_TEST FAIL\n");
        return 1;
    }
    std::printf("00a96b30_TEST PASS\n");
    return 0;
}