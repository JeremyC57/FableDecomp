#include <cstdio>
#include <new>
static int g_strCopies = 0;
class CWideString { public:
    const wchar_t* m_pData;
    CWideString(const wchar_t* s) : m_pData(s) { }
    CWideString(const CWideString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
struct CEntry { long m_words[8]; };
struct CWideStringEntryPair {
    const CWideString first;
    CEntry second;
    CWideStringEntryPair(const wchar_t* s) : first(s) { }
};
void Construct_PairWideStringChecksumEntry_00a60950(CWideStringEntryPair* p, const CWideStringEntryPair& value)
{
    if (p) new (p) CWideStringEntryPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CWideStringEntryPair src(L"file.big");
    int i;
    for (i = 0; i < 8; ++i) src.second.m_words[i] = 5 * i + 1;
    static double storage[8];
    CWideStringEntryPair* dst = (CWideStringEntryPair*)(void*)storage;
    Construct_PairWideStringChecksumEntry_00a60950(dst, src);
    if (g_strCopies != 1 || dst->first.m_pData != src.first.m_pData) { std::printf("00a60950_TEST FAIL\n"); return 1; }
    for (i = 0; i < 8; ++i) if (dst->second.m_words[i] != 5 * i + 1) { std::printf("00a60950_TEST FAIL\n"); return 1; }
    Construct_PairWideStringChecksumEntry_00a60950(0, src);
    if (g_strCopies != 1) { std::printf("00a60950_TEST FAIL\n"); return 1; }
    std::printf("00a60950_TEST PASS\n");
    return 0;
}