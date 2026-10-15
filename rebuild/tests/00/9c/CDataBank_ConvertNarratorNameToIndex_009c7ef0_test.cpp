// Behaviour test for 0x009C7EF0 CDataBank::ConvertNarratorNameToIndex: equal data blocks or equal (length, text) match;
// a missing name or an empty array gives -1.
#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pChars; unsigned int m_length; };
struct CCharString { CCharStringData* m_pData; };
static bool Eq(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pData; CCharStringData* b = sb.m_pData;
    if (a == b) return true;
    if (!a || !b) return false;
    if (a->m_length != b->m_length) return false;
    return std::strcmp(a->m_pChars, b->m_pChars) == 0;
}
struct CDataBank {
    CCharString* m_pFirst; CCharString* m_pLast;
    unsigned int Size() const { return (unsigned int)(m_pLast - m_pFirst); }
    long ConvertNarratorNameToIndex(const CCharString& name) const
    {
        unsigned int i = 0;
        if (i < Size()) {
            CCharString key = name;
            const CCharString* it = m_pFirst;
            do {
                if (Eq(*it, key)) return (long)i;
                ++i; ++it;
            } while (i < Size());
        }
        return -1;
    }
};
int main()
{
    CCharStringData d0 = { "HERO", 4 }, d1 = { "GUARD", 5 }, d2 = { "TRADER", 6 }, probe = { "TRADER", 6 }, other = { "NOBODY", 6 };
    CCharString arr[4]; arr[0].m_pData = &d0; arr[1].m_pData = 0; arr[2].m_pData = &d1; arr[3].m_pData = &d2;
    CDataBank bank; bank.m_pFirst = arr; bank.m_pLast = arr + 4;
    CCharString k; k.m_pData = &probe;
    if (bank.ConvertNarratorNameToIndex(k) != 3) { std::printf("009c7ef0_TEST FAIL\n"); return 1; }
    k.m_pData = &d1; if (bank.ConvertNarratorNameToIndex(k) != 2) { std::printf("009c7ef0_TEST FAIL\n"); return 1; }
    k.m_pData = 0; if (bank.ConvertNarratorNameToIndex(k) != 1) { std::printf("009c7ef0_TEST FAIL\n"); return 1; }
    k.m_pData = &other; if (bank.ConvertNarratorNameToIndex(k) != -1) { std::printf("009c7ef0_TEST FAIL\n"); return 1; }
    bank.m_pLast = arr; k.m_pData = &d0; if (bank.ConvertNarratorNameToIndex(k) != -1) { std::printf("009c7ef0_TEST FAIL\n"); return 1; }
    std::printf("009c7ef0_TEST PASS\n");
    return 0;
}