#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pChars; unsigned long m_length; };
static long Compare(const char* a, const char* b) { return std::strcmp(a, b); }
struct CCharString { CCharStringData* m_pStringData; };
static bool SameText(CCharStringData* a, CCharStringData* b)
{
    if (a == b) return true;
    if (!a || !b) return false;
    if (a->m_length != b->m_length) return false;
    return Compare(a->m_pChars, b->m_pChars) == 0;
}
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct SSymbolPair { CCharString first; CSymbolInfo second; };
struct CSymbolVectorMap { SSymbolPair* m_pFirst; SSymbolPair* m_pLast; SSymbolPair* m_pEnd; };
struct CSymbolMap {
    void* _padding_; CSymbolVectorMap SLONGMap;
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& group, long value) const
    {
        const SSymbolPair* end = SLONGMap.m_pLast;
        const SSymbolPair* it = SLONGMap.m_pFirst;
        if (it != end) {
            CCharStringData* key = group.m_pStringData;
            do {
                if (SameText(it->second.GroupName.m_pStringData, key) && it->second.Val == value)
                    return &it->second;
            } while (++it != end);
        }
        return 0;
    }
};
int main()
{
    CCharStringData a = { "door", 4 }, b = { "gate", 4 }, b2 = { "gate", 4 }, c = { "gates", 5 };
    SSymbolPair e[4];
    e[0].second.GroupName.m_pStringData = &a; e[0].second.Val = 1;
    e[1].second.GroupName.m_pStringData = &b; e[1].second.Val = 2;
    e[2].second.GroupName.m_pStringData = &b; e[2].second.Val = 3;
    e[3].second.GroupName.m_pStringData = 0;  e[3].second.Val = 4;
    CSymbolMap m; m.SLONGMap.m_pFirst = e; m.SLONGMap.m_pLast = e + 4; m.SLONGMap.m_pEnd = e + 4;
    CCharString k; k.m_pStringData = &b2;
    CCharString k2; k2.m_pStringData = &c;
    CCharString kn; kn.m_pStringData = 0;
    if (m.FindFirstSymbolWithVal(k, 3) != &e[2].second || m.FindFirstSymbolWithVal(k, 1) != 0
        || m.FindFirstSymbolWithVal(k2, 2) != 0 || m.FindFirstSymbolWithVal(kn, 4) != &e[3].second
        || m.FindFirstSymbolWithVal(kn, 1) != 0) { std::printf("009e50c0_TEST FAIL\n"); return 1; }
    m.SLONGMap.m_pLast = e; if (m.FindFirstSymbolWithVal(k, 2) != 0) { std::printf("009e50c0_TEST FAIL\n"); return 1; }
    std::printf("009e50c0_TEST PASS\n");
    return 0;
}
