#include <cstdio>
#include <new>
static int g_strCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
static int g_vecCopies = 0;
struct CDefInfoContainer {
    int* m_first; int* m_last; int* m_end;
    CDefInfoContainer(int* f, int* l) : m_first(f), m_last(l), m_end(l) { }
    CDefInfoContainer(const CDefInfoContainer& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end) { ++g_vecCopies; }
};
struct CULongDefClassInfoPair {
    const unsigned long first;
    CCharString m_name;
    long m_index;
    CDefInfoContainer m_defs;
    CULongDefClassInfoPair(unsigned long k, const char* s, long i, int* f, int* l) : first(k), m_name(s), m_index(i), m_defs(f, l) { }
};
void Construct_PairULongDefClassInfo_0047c250(CULongDefClassInfoPair* p, const CULongDefClassInfoPair& value)
{
    if (p) new (p) CULongDefClassInfoPair(value);   // VC null-tests the placement result itself
}
int main()
{
    int data[3] = { 1, 2, 3 };
    CULongDefClassInfoPair src(0xABCDu, "CThingDef", 17, data, data + 3);
    static double storage[8];
    CULongDefClassInfoPair* dst = (CULongDefClassInfoPair*)(void*)storage;
    Construct_PairULongDefClassInfo_0047c250(dst, src);
    if (g_strCopies != 1 || g_vecCopies != 1 || dst->first != 0xABCDu || dst->m_name.m_pData != src.m_name.m_pData
        || dst->m_index != 17 || dst->m_defs.m_first != data || dst->m_defs.m_last != data + 3) { std::printf("0047c250_TEST FAIL\n"); return 1; }
    Construct_PairULongDefClassInfo_0047c250(0, src);
    if (g_strCopies != 1 || g_vecCopies != 1) { std::printf("0047c250_TEST FAIL\n"); return 1; }
    std::printf("0047c250_TEST PASS\n");
    return 0;
}