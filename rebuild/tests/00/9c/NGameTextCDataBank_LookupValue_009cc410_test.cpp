#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace NGameText {
struct SKeyEntry { unsigned long m_key; void* m_value; };
struct CKeyEntryVectorMap {
    SKeyEntry* m_pFirst; SKeyEntry* m_pLast; SKeyEntry* m_pEnd;
    SKeyEntry* LowerBound(const unsigned long& key) { SKeyEntry* it = m_pFirst; while (it != m_pLast && it->m_key < key) ++it; return it; }
    SKeyEntry* End() { return m_pLast; }
    SKeyEntry* Find(const unsigned long& key) { SKeyEntry* it = LowerBound(key); return (it == End() || key < it->m_key) ? End() : it; }
};
class CDataBank { public:
    unsigned char _pad00[0xd0]; CKeyEntryVectorMap m_entries;
    void* LookupValue(unsigned long key) { SKeyEntry* it = m_entries.Find(key); if (it != m_entries.End()) return it->m_value; return 0; }
};
}
int main()
{
    int a, b, c; NGameText::SKeyEntry entries[3]; entries[0].m_key = 10; entries[0].m_value = &a; entries[1].m_key = 20; entries[1].m_value = &b; entries[2].m_key = 30; entries[2].m_value = &c;
    NGameText::CDataBank bank; bank.m_entries.m_pFirst = entries; bank.m_entries.m_pLast = entries + 3; bank.m_entries.m_pEnd = entries + 3;
    if (bank.LookupValue(10) != &a || bank.LookupValue(20) != &b || bank.LookupValue(30) != &c || bank.LookupValue(25) != 0 || bank.LookupValue(31) != 0 || bank.LookupValue(0) != 0) { std::printf("009cc410_TEST FAIL\n"); return 1; }
    bank.m_entries.m_pLast = entries;
    if (bank.LookupValue(10) != 0) { std::printf("009cc410_TEST FAIL\n"); return 1; }
    std::printf("009cc410_TEST PASS\n"); return 0;
}