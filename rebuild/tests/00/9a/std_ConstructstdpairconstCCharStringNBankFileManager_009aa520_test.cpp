// Behaviour test for 0x009AA520 std::_Construct<pair<const CCharString, NBankFileManager::CContainedBank>, ..>: placement copy of one ContainedBanks value (the key through its copy constructor once, the CContainedBank words); a null destination does nothing.
#include <cstdio>
#include <new>
static int g_strCopies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
class CContainedBank { public: unsigned long m_words[5]; };   // DataVersion, EntryCount, InfoOffset, InfoSize, Alignment
struct CCharStringBankPair {
    const CCharString first;
    CContainedBank second;
    CCharStringBankPair(const char* s) : first(s) { }
};
void _Construct(CCharStringBankPair* p, const CCharStringBankPair& value)
{
    if (p) new (p) CCharStringBankPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CCharStringBankPair src("bank");
    int i;
    for (i = 0; i < 5; ++i) src.second.m_words[i] = 9 - i;
    static double storage[8];
    CCharStringBankPair* dst = (CCharStringBankPair*)(void*)storage;
    _Construct(dst, src);
    if (g_strCopies != 1 || dst->first.m_pData != src.first.m_pData) { std::printf("009aa520_TEST FAIL\n"); return 1; }
    for (i = 0; i < 5; ++i) if (dst->second.m_words[i] != 9 - i) { std::printf("009aa520_TEST FAIL\n"); return 1; }
    _Construct(0, src);
    if (g_strCopies != 1) { std::printf("009aa520_TEST FAIL\n"); return 1; }
    std::printf("009aa520_TEST PASS\n");
    return 0;
}