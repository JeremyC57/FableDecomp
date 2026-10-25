// Behaviour test for 0x009E51C0 CSymbolMap::SymbolExists (links the source object): true for a mapped name, false for a name
// that falls between entries or past the end.
#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pChars; unsigned long m_length; };
class CCharString {
public:
    CCharStringData* m_pStringData;
    bool FastCompare(const CCharString& other) const;
};
bool CCharString::FastCompare(const CCharString& other) const { return std::strcmp(m_pStringData->m_pChars, other.m_pStringData->m_pChars) < 0; }
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct SSymbolPair { CCharString first; CSymbolInfo second; };
struct CSymbolVectorMap {
    SSymbolPair* m_pFirst; SSymbolPair* m_pLast; SSymbolPair* m_pEnd;
    SSymbolPair* LowerBound(const CCharString& key) const;
};
SSymbolPair* CSymbolVectorMap::LowerBound(const CCharString& key) const
{
    SSymbolPair* it = m_pFirst;
    while (it != m_pLast && it->first.FastCompare(key)) ++it;
    return it;
}
struct CSymbolMap {
    void* _padding_; CSymbolVectorMap SLONGMap;
    bool SymbolExists(const CCharString& name) const;
};
int main()
{
    CCharStringData n0 = { "apple", 5 }, n1 = { "kiwi", 4 }, n2 = { "pear", 4 }, q1 = { "kiwi", 4 }, q2 = { "lime", 4 }, q3 = { "zzz", 3 };
    SSymbolPair e[3];
    e[0].first.m_pStringData = &n0; e[0].second.Val = 10;
    e[1].first.m_pStringData = &n1; e[1].second.Val = 20;
    e[2].first.m_pStringData = &n2; e[2].second.Val = 30;
    CSymbolMap m; m.SLONGMap.m_pFirst = e; m.SLONGMap.m_pLast = e + 3; m.SLONGMap.m_pEnd = e + 3;
    CCharString k1; k1.m_pStringData = &q1;
    CCharString k2; k2.m_pStringData = &q2;
    CCharString k3; k3.m_pStringData = &q3;
    bool hit = m.SymbolExists(k1), miss = m.SymbolExists(k2), past = m.SymbolExists(k3);
    if (!(hit && !miss && !past)) { std::printf("009e51c0_TEST FAIL\n"); return 1; }
    std::printf("009e51c0_TEST PASS\n");
    return 0;
}
