#include <cstdio>
#include <new>
static int g_strCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
struct SThreeWords { long m_a, m_b, m_c; };
struct CCharStringBankPair {
    const CCharString first;
    SThreeWords m_range;
    long m_first;
    long m_second;
    CCharStringBankPair(const char* s) : first(s) { }
};
void Construct_PairCharStringContainedBank_008eba50(CCharStringBankPair* p, const CCharStringBankPair& value)
{
    if (p) new (p) CCharStringBankPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CCharStringBankPair src("bank");
    src.m_range.m_a = 1; src.m_range.m_b = 2; src.m_range.m_c = 3; src.m_first = 4; src.m_second = 5;
    static double storage[8];
    CCharStringBankPair* dst = (CCharStringBankPair*)(void*)storage;
    Construct_PairCharStringContainedBank_008eba50(dst, src);
    if (g_strCopies != 1 || dst->first.m_pData != src.first.m_pData || dst->m_range.m_a != 1 || dst->m_range.m_b != 2
        || dst->m_range.m_c != 3 || dst->m_first != 4 || dst->m_second != 5) { std::printf("008eba50_TEST FAIL\n"); return 1; }
    Construct_PairCharStringContainedBank_008eba50(0, src);
    if (g_strCopies != 1) { std::printf("008eba50_TEST FAIL\n"); return 1; }
    std::printf("008eba50_TEST PASS\n");
    return 0;
}