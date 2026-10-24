// Behaviour test for 0x00769810 UninitFillN_TwoStringRecord_00769810: fill n {CCharString, CCharString, long x3}
// records (both strings copy-constructed, i.e. their shared data gains a reference, then three words copied).
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct SStringData { long m_refs; const char* m_text; };
static int g_stringCopies = 0;
struct CCharString {
    SStringData* m_pStringData;
    CCharString() : m_pStringData(0) { }
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; if (m_pStringData) ++m_pStringData->m_refs; }
};
struct STwoStringRecord {
    CCharString m_first;
    CCharString m_second;
    long        m_8;
    long        m_c;
    long        m_10;
    STwoStringRecord() : m_8(0), m_c(0), m_10(0) { }
};

static STwoStringRecord* UninitFillN(STwoStringRecord* first, unsigned int n, const STwoStringRecord& x, const __false_type&)
{
    STwoStringRecord* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) STwoStringRecord(x);
    return cur;
}

int main()
{
    SStringData s1 = { 1, "alpha" };
    SStringData s2 = { 1, "beta" };
    STwoStringRecord x;
    x.m_first.m_pStringData = &s1; x.m_second.m_pStringData = &s2;
    x.m_8 = 8; x.m_c = 12; x.m_10 = 16;
    STwoStringRecord dst[4];
    __false_type tag;
    STwoStringRecord* r = UninitFillN(dst, 4, x, tag);
    bool ok = r == dst + 4 && g_stringCopies == 8 && s1.m_refs == 5 && s2.m_refs == 5;
    for (int k = 0; k < 4; ++k)
        if (dst[k].m_first.m_pStringData != &s1 || dst[k].m_second.m_pStringData != &s2
            || dst[k].m_8 != 8 || dst[k].m_c != 12 || dst[k].m_10 != 16) ok = false;
    if (UninitFillN(dst, 0, x, tag) != dst || g_stringCopies != 8) ok = false;
    if (!ok) { std::printf("00769810_TEST FAIL\n"); return 1; }
    std::printf("00769810_TEST PASS\n");
    return 0;
}