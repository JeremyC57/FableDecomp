#include <cstdio>
#include <new>
static int g_strCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
struct CDateAndTime { long m_words[7]; };
struct CLongDateAndTimePair {
    long first;
    CDateAndTime second;
    CLongDateAndTimePair() { }
    CLongDateAndTimePair(const CLongDateAndTimePair& o) : first(o.first), second(o.second) { }
};
struct CCharStringLongDatePair {
    const CCharString first;
    CLongDateAndTimePair second;
    CCharStringLongDatePair(const char* s) : first(s) { }
};
void Construct_PairCharStringLongDateAndTime_0071f4e0(CCharStringLongDatePair* p, const CCharStringLongDatePair& value)
{
    if (p) new (p) CCharStringLongDatePair(value);   // VC null-tests the placement result itself
}
int main()
{
    CCharStringLongDatePair src("save");
    src.second.first = 77;
    int i;
    for (i = 0; i < 7; ++i) src.second.second.m_words[i] = 100 + i;
    static double storage[8];
    CCharStringLongDatePair* dst = (CCharStringLongDatePair*)(void*)storage;
    Construct_PairCharStringLongDateAndTime_0071f4e0(dst, src);
    if (g_strCopies != 1 || dst->first.m_pData != src.first.m_pData || dst->second.first != 77) { std::printf("0071f4e0_TEST FAIL\n"); return 1; }
    for (i = 0; i < 7; ++i) if (dst->second.second.m_words[i] != 100 + i) { std::printf("0071f4e0_TEST FAIL\n"); return 1; }
    Construct_PairCharStringLongDateAndTime_0071f4e0(0, src);
    if (g_strCopies != 1) { std::printf("0071f4e0_TEST FAIL\n"); return 1; }
    std::printf("0071f4e0_TEST PASS\n");
    return 0;
}